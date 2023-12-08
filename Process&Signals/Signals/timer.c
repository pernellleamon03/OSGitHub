/* timer.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile int alarms = 0;

void alarmHandler(int signum)
{
    alarms++;
    printf("Hello World!\n");
}

void interruptHandler(int signum)
{
    printf("\nTotal execution time: %d seconds\n", alarms);
    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGALRM, alarmHandler);  // register handler for SIGALRM
    signal(SIGINT, interruptHandler); // register handler for SIGINT

    while (1)
    {
        alarm(1);  // Schedule a SIGALRM for 1 second
        pause();   // Wait for the signal to be delivered
    }

    return 0; // never reached
}