#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define SHM_SIZE 2 * sizeof(int)

void ParentProcess(int *);
void ChildProcess(int *);

int main()
{
    int ShmID;
    int *ShmPTR;
    pid_t pid;
    int status;

    // Create shared memory
    ShmID = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (ShmID < 0)
    {
        fprintf(stderr, "*** shmget error (server) ***\n");
        exit(1);
    }

    ShmPTR = (int *)shmat(ShmID, NULL, 0);
    if (*ShmPTR == -1)
    {
        fprintf(stderr, "*** shmat error (server) ***\n");
        exit(1);
    }

    // Initialize shared variables
    ShmPTR[0] = 0; // BankAccount
    ShmPTR[1] = 0; // Turn

    // Fork a child process
    pid = fork();

    if (pid < 0)
    {
        fprintf(stderr, "*** fork error ***\n");
        exit(1);
    }
    else if (pid == 0)
    {
        // Child process
        ChildProcess(ShmPTR);
        exit(0);
    }
    else
    {
        // Parent process
        ParentProcess(ShmPTR);

        // Wait for the child to complete
        wait(&status);

        // Detach and remove shared memory
        shmdt((void *)ShmPTR);
        shmctl(ShmID, IPC_RMID, NULL);
    }

    return 0;
}

void ParentProcess(int *SharedMem)
{
    srand(time(NULL));

    int i = 0;
    while (i < 25)
    {
        sleep(rand() % 6); // Sleep for a random amount of time (0-5 seconds)

        int account = SharedMem[0];

        while (SharedMem[1] != 0)
        {
            // Do nothing while it's not the parent's turn
        }

        if (account <= 100)
        {
            int balance = rand() % 101;

            if (balance % 2 == 0)
            {
                SharedMem[0] += balance;
                printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, SharedMem[0]);
            }
            else
            {
                printf("Dear old Dad: Doesn't have any money to give\n");
            }

            account = SharedMem[0];
        }
        else
        {
            printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
        }

        SharedMem[1] = 1;
        i++;
    }
}

void ChildProcess(int *SharedMem)
{
    srand(time(NULL));

    int i = 0;
    while (i < 25)
    {
        sleep(rand() % 6); // Sleep for a random amount of time (0-5 seconds)

        int account = SharedMem[0];

        while (SharedMem[1] != 1)
        {
            // Do nothing while it's not the child's turn
        }

        int balance = rand() % 51;

        printf("Poor Student needs $%d\n", balance);

        if (balance <= account)
        {
            SharedMem[0] -= balance;
            printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, SharedMem[0]);
        }
        else
        {
            printf("Poor Student: Not Enough Cash ($%d)\n", account);
        }

        account = SharedMem[0];

        SharedMem[1] = 0;
        i++;
    }
}
