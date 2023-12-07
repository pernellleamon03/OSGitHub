#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_CMD_LINE_LEN 1024
#define MAX_CMD_LINE_ARGS 128

int is_timer_set = 0;
bool is_background = false;
char *custom_builtins[] = {"cd", "pwd", "echo", "export", NULL};

void set_custom_timer(int seconds) {
    is_timer_set = 1;
    alarm(seconds);
}

void cancel_custom_timer() {
    is_timer_set = 0;
    alarm(0);
}

void handle_custom_SIGINT(int signum) {
    if (!is_background) {
        printf("\n");
        char current_path[MAX_CMD_LINE_LEN];
        if (getcwd(current_path, sizeof(current_path)) != NULL) {
            printf("%s> ", current_path);
        } else {
            perror("getcwd");
        }
        fflush(stdout);
    }
}

void handle_custom_SIGALARM(int signum) {
    if (is_timer_set) {
        printf("\nTimer has expired. Terminating process.\n");
        kill(0, SIGINT);
    }
}

void execute_custom_command(char **arguments) {
    pid_t pid = fork();
    if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        execvp(arguments[0], arguments);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        if (!is_background) {
            set_custom_timer(10);
            int status;
            while (waitpid(pid, &status, 0) == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }
            cancel_custom_timer();
        }
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

bool handle_custom_builtins(char **arguments) {
    int i = 0;
    while (custom_builtins[i] != NULL) {
        if (strcmp(arguments[0], custom_builtins[i]) == 0) {
            if (strcmp(arguments[0], "cd") == 0) {
                if (arguments[1] == NULL) {
                    chdir(getenv("HOME"));
                } else {
                    if (chdir(arguments[1]) == -1) {
                        perror("chdir");
                    }
                }
            } else if (strcmp(arguments[0], "pwd") == 0) {
                char current_path[MAX_CMD_LINE_LEN];
                if (getcwd(current_path, sizeof(current_path)) != NULL) {
                    printf("%s\n", current_path);
                } else {
                    perror("getcwd");
                }
            } else if (strcmp(arguments[0], "echo") == 0) {
                int j = 1;
                while (arguments[j] != NULL) {
                    printf("%s ", arguments[j]);
                    j++;
                }
                printf("\n");
            } else if (strcmp(arguments[0], "export") == 0) {
                if (arguments[1] != NULL) {
                    if (putenv(arguments[1]) != 0) {
                        perror("putenv");
                    }
                }
            }
            return true;
        }
        i++;
    }
    return false;
}

void execute_commands_with_pipes(char **cmd_set1, char **cmd_set2) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }

    pid_t pid1 = fork();

    if (pid1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        execvp(cmd_set1[0], cmd_set1);
        perror("execvp (command 1)");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();

    if(pid2 == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        execvp(cmd_set2[0], cmd_set2);
        perror("execvp (command 2)");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

bool handle_pipe_commands(char **arguments) {
    char *token;
    char *cmd_set1[MAX_CMD_LINE_ARGS];
    char *cmd_set2[MAX_CMD_LINE_ARGS];
    int i = 0;
    int mode = 0;

    while ((token = strtok(arguments, "|")) != NULL) {
        if (mode == 0) {
            cmd_set1[i] = strdup(token);
            mode = 1;
        } else {
            cmd_set2[i] = strdup(token);
            mode = 0;
        }
        i++;
    }
    cmd_set1[i] = NULL;
    cmd_set2[i] = NULL;

    if (cmd_set1[0] != NULL && cmd_set2[0] != NULL) {
        return true;
    }

    return false;
}

int main() {
    char cmd_line[MAX_CMD_LINE_LEN];
    char *arguments[MAX_CMD_LINE_ARGS];
    signal(SIGINT, handle_custom_SIGINT);
    signal(SIGALRM, handle_custom_SIGALARM);

    while (true) {
        size_t line_size = 0;
        ssize_t read_size = getline(&cmd_line, &line_size, stdin);
        if (read_size == -1) {
            perror("getline");
            break;
        }
        if (read_size <= 1) {
            continue;
        }

        char *token = strtok(cmd_line, " \t\r\n");
        int i = 0;
        while (token != NULL) {
            arguments[i] = strdup(token);
            token = strtok(NULL, " \t\r\n");
            i++;
        }
        arguments[i] = NULL;

        if (strcmp(arguments[0], "exit") == 0) {
            break;
        } else if (handle_pipe_commands(arguments)) {
        } else if (handle_custom_builtins(arguments)) {
        } else {
            execute_custom_command(arguments);
        }

        for (int j = 0; j < i; j++) {
            free(arguments[j]);
        }
    }

    free(cmd_line);
    return 0;
}
