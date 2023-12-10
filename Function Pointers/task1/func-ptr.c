// func-ptr.c

#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "util.h"

#define DEBUG 0 // change this to 1 to enable verbose output

/**
 * Signature for a function pointer that can compare
 * You need to cast the input into its actual
 * type and then compare them according to your
 * custom logic
 */
typedef int (*Comparer)(const void *a, const void *b);

/**
 * compares 2 processes
 * You can assume:
 * - Process ids will be unique
 * - No 2 processes will have the same arrival time
 */
int my_comparer(const void *this, const void *that)
{
    Process *process_this = (Process *)this;
    Process *process_that = (Process *)that;

    // Custom logic for comparing processes
    if (process_this->priority > process_that->priority)
    {
        return -1; // Compare based on priority (descending order)
    }
    else if (process_this->priority < process_that->priority)
    {
        return 1; // Compare based on priority (descending order)
    }
    else
    {
        // If priorities are equal, compare based on arrival time (ascending order)
        if (process_this->arrival_time < process_that->arrival_time)
        {
            return -1;
        }
        else if (process_this->arrival_time > process_that->arrival_time)
        {
            return 1;
        }
        else
        {
            return 0; // Equal in both priority and arrival time
        }
    }
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        fprintf(stderr, "Usage: ./func-ptr <input-file-path>\n");
        fflush(stdout);
        return 1;
    }

    /*******************/
    /* Parse the input */
    /*******************/
    FILE *input_file = fopen(argv[1], "r");
    if (!input_file)
    {
        fprintf(stderr, "Error: Invalid filepath\n");
        fflush(stdout);
        return 1;
    }

    Process *processes = parse_file(input_file);

    /*******************/
    /* sort the input  */
    /*******************/
    Comparer process_comparer = &my_comparer;

#if DEBUG
    int i = 0;
    while (i < P_SIZE)
    {
        printf("%d (%d, %d) ",
               processes[i].pid,
               processes[i].priority, processes[i].arrival_time);
        i++;
    }
    printf("\n");
#endif

    qsort(processes, P_SIZE, sizeof(Process), process_comparer);

    /**************************/
    /* print the sorted data  */
    /**************************/

    int k = 0;
    while (k < P_SIZE)
    {
        printf("%d (%d, %d)\n",
               processes[k].pid,
               processes[k].priority, processes[k].arrival_time);
        k++;
    }
    fflush(stdout);
    fflush(stderr);

    /************/
    /* clean up */
    /************/
    free(processes);
    fclose(input_file);
    return 0;
}