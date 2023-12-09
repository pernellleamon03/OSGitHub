// pipes_processes3.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <argument>\n", argv[0]);
        return 1;
    }

    int fd1[2]; // Used to store two ends of the first pipe
    int fd2[2]; // Used to store two ends of the second pipe

    char *grep_arg = argv[1];

    pid_t p1;

    if (pipe(fd1) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }
    if (pipe(fd2) == -1)
    {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    p1 = fork();

    if (p1 < 0)
    {
        fprintf(stderr, "fork Failed");
        return 1;
    }

    // Parent process
    else if (p1 > 0)
    {
        close(fd1[0]); // Close reading end of the first pipe

        // Write input string and close writing end of the first pipe
        write(fd1[1], grep_arg, strlen(grep_arg) + 1);
        close(fd1[1]);

        // Wait for the child to finish
        wait(NULL);

        close(fd2[1]); // Close writing end of the second pipe

        // Read and print the sorted string from the second pipe
        char sorted_str[100];
        read(fd2[0], sorted_str, 100);
        printf("Sorted Output:\n%s\n", sorted_str);
        close(fd2[0]);
    }

    // Child process
    else
    {
        close(fd1[1]); // Close writing end of the first pipe

        // Read the argument from the first pipe
        char grep_str[100];
        read(fd1[0], grep_str, 100);

        // Concatenate a fixed string with it
        char fixed_str[] = "scores";
        strcat(grep_str, fixed_str);

        // Execute grep command and pipe the output to sort
        dup2(fd2[1], STDOUT_FILENO); // Redirect stdout to the writing end of the second pipe
        close(fd2[0]);               // Close reading end of the second pipe

        execlp("grep", "grep", grep_str, NULL);

        close(fd1[0]); // Close reading end of the first pipe
        close(fd2[1]); // Close writing end of the second pipe

        exit(0);
    }

    return 0;
}
