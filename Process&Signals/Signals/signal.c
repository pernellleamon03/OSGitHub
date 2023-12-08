/* hello_signal.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile int flag = 0;

void handler(int signum)
{ // signal handler
    printf("Hello World!\n");
    flag = 1;
}

int main(int argc, char *argv[])
{
    signal(SIGALRM, handler); // register handler to handle SIGALRM

    while (1)
    {
        alarm(1); // Schedule a SIGALRM for 1 second
        pause();   // Wait for the signal to be delivered
        printf("Turing was right!\n");
        flag = 0;
    }

    return 0; // never reached
}
