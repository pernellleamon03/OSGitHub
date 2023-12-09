// C program for implementation of Simulation 
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"


// Function to find the waiting time for all  
// processes
// Function to find waiting time for Round Robin scheduling
void findWaitingTimeRR(ProcessType plist[], int n, int quantum)
{
    int rem_bt[n]; // Remaining burst time for each process
    for (int i = 0; i < n; i++)
        rem_bt[i] = plist[i].bt;

    int t = 0; // Current time

    // Keep traversing all processes while all processes are not done
    while (1)
    {
        int done = 1; // Flag to check if all processes are done

        // Traverse all processes
        for (int i = 0; i < n; i++)
        {
            // If the process is not done yet
            if (rem_bt[i] > 0)
            {
                done = 0; // There is a pending process

                // If remaining burst time is greater than quantum
                if (rem_bt[i] > quantum)
                {
                    t += quantum; // Increment time by quantum
                    rem_bt[i] -= quantum;
                }
                else
                {
                    t += rem_bt[i]; // Increment time by remaining burst time
                    plist[i].wt = t - plist[i].bt; // Calculate waiting time
                    rem_bt[i] = 0; // This process is over
                }
            }
        }

        // If all processes are done, exit the loop
        if (done == 1)
            break;
    }
}

// Function to find waiting time for Shortest Job First (SJF)
void findWaitingTimeSJF(ProcessType plist[], int n)
{
    int remaining_time[n]; // Remaining burst time for each process
    for (int i = 0; i < n; i++)
        remaining_time[i] = plist[i].bt;

    int completed = 0; // Counter for completed processes
    int t = 0;        // Current time

    // Traverse until all processes get completely executed
    while (completed < n)
    {
        int min_index = -1;
        int min_bt = INT_MAX;

        // Find process with minimum remaining time at every time lap
        for (int i = 0; i < n; i++)
        {
            if (plist[i].art <= t && remaining_time[i] < min_bt && remaining_time[i] > 0)
            {
                min_index = i;
                min_bt = remaining_time[i];
            }
        }

        // If no process is found, increment time
        if (min_index == -1)
            t++;
        else
        {
            // Reduce remaining time by 1
            remaining_time[min_index]--;

            // If remaining time becomes 0, increment completion counter
            if (remaining_time[min_index] == 0)
            {
                completed++;
                plist[min_index].wt = t - plist[min_index].art - plist[min_index].bt;
            }

            // Increment time lap by one
            t++;
        }
    }
}

// Function to find the waiting time for all  
// processes 
void findWaitingTime(ProcessType plist[], int n)
{ 
    // waiting time for first process is 0, or the arrival time if not 
    plist[0].wt = 0 +  plist[0].art; 
  
    // calculating waiting time 
    for (int  i = 1; i < n ; i++ ) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
} 
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
// Function to sort the Process acc. to priority
int my_comparer(const void *this, const void *that)
{ 
  
    /*  
     * 1. Cast this and that into (ProcessType *)
     * 2. return 1 if this->pri < that->pri
     */ 
  
    return 1;
} 

//Function to calculate average time 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTime(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time 
void findavgTimePriority( ProcessType plist[], int n) 
{ 
  
   /*
    * 1- Sort the processes (i.e. plist[]), burst time and priority according to the priority.
    * 2- Now simply apply FCFS algorithm.
    */
  
    //Display processes along with all details 
    printf("\n*********\nPriority\n");
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn  
    // around time 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
  // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
  
  // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 