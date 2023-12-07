#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

int main() {
    srand(time(NULL)); // seed random number generator
    pid_t firstChild, secondChild;
    int status;

    firstChild = fork(); // creates the process and stores PID in firstChild

    if (firstChild == -1) {
        perror("Fork process failed");
        return -1;
    } else if (firstChild == 0) {
        // Entering the first child process and executing the code below
        int randomIterations = rand() % 31;
        int iterationCount = 0, sleepTime;

        do {
            printf("Child PID:%d is going to sleep!\n", getpid());
            sleepTime = rand() % 11; // sleep time is between 0 and 10
            sleep(sleepTime);
            printf("Child PId:%d is awake!\nWhere is my Parent: %d\n", getpid(), getppid());
            iterationCount++;
        } while (iterationCount < randomIterations);

        printf("Child PId:%d has completed and is now exiting\n", getpid());
        exit(0);
    } else {
        secondChild = fork(); // creates the process and stores PID in secondChild

        if (secondChild == -1) {
            perror("Fork process failed");
            return -1;
        } else if (secondChild == 0) {
            // Entering the second child process and executing the code below
            int randomIterations = rand() % 31;
            int iterationCount = 0, sleepTime;

            do {
                printf("Child PID:%d is going to sleep!\n", getpid());
                sleepTime = rand() % 11; // sleep time is between 0 and 10
                sleep(sleepTime);
                printf("Child PId:%d is awake!\nWhere is my Parent: %d\n", getpid(), getppid());
                iterationCount++;
            } while (iterationCount < randomIterations);

            printf("Child PId:%d has completed and is now exiting\n", getpid());
            exit(0);
        } else {
            // Entering the parent process and executing the code below
            wait(&status);
            printf("Child PId:%d has completed\n", firstChild);
            wait(&status);
            printf("Child PId:%d has completed\n", secondChild);
        }
    }

    return 0;
}
