// Collaborated with Pernell Laemon & Michael Johnson Jr
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

// Sudoku board. Modify this to test your checker!
// 0 == empty cell; 1-9 is the filled in digit.
int sudokuBoard[SIZE][SIZE] = {
    {6, 3, 9, 5, 7, 4, 1, 8, 2},
    {5, 4, 1, 8, 2, 9, 3, 7, 6},
    {7, 8, 2, 6, 1, 3, 9, 5, 4},
    {1, 9, 8, 4, 6, 7, 5, 2, 3},
    {3, 6, 5, 9, 8, 2, 4, 1, 7},
    {4, 2, 7, 1, 3, 5, 8, 6, 9},
    {9, 5, 6, 7, 4, 8, 2, 3, 1},
    {8, 1, 3, 2, 9, 6, 7, 4, 5},
    {2, 7, 4, 3, 5, 1, 6, 9, 8},
};

bool rowCheck[SIZE];
bool colCheck[SIZE];
bool boxCheck[SIZE];

void printSudokuBoard(int sudokuBoard[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf("%5d", sudokuBoard[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void *checkRow(void *args) {
    int occurrence[SIZE];
    int i = 0;
    for (i = 0; i < SIZE; i++)
    {
        occurrence[i] = 0;
    }
    int row = *((int *)args);

    for (i = 0; i < SIZE; i++)
    {
        occurrence[sudokuBoard[i][row] - 1] += 1;
    }

    rowCheck[row] = true;
    for (i = 0; i < SIZE; i++)
    {
        if (occurrence[i] != 1)
        {
            rowCheck[row] = false;
        }
    }
}

void *checkCol(void *args)
{
    int occurrence[SIZE];
    int i = 0;
    for (i = 0; i < SIZE; i++)
    {
        occurrence[i] = 0;
    }
    int col = *((int *)args);

    for (i = 0; i < SIZE; i++)
    {
        occurrence[sudokuBoard[col][i] - 1] += 1;
    }

    colCheck[col] = true;
    for (i = 0; i < SIZE; i++)
    {
        if (occurrence[i] != 1)
        {
            colCheck[col] = false;
        }
    }
}

void *checkBox(void *args)
{
    int occurrence[SIZE];
    int i = 0;
    for (i = 0; i < SIZE; i++)
    {
        occurrence[i] = 0;
    }
    int box = *((int *)args);
    int j = 0;
    int x = (box / 3) * 3;
    int y = (box % 3) * 3;

    for (int i = 0; i < 3; i++)
    {

        for (int j = 0; j < 3; j++)
        {
            occurrence[sudokuBoard[i + x][j + y] - 1] += 1;
        }
    }

    boxCheck[box] = true;
    for (i = 0; i < SIZE; i++)
    {
        if (occurrence[i] != 1)
        {
            boxCheck[box] = false;
        }
    }
}

int main()
{
    // 1. Print the board.
    printf("Sudoku Board:\n");
    printSudokuBoard(sudokuBoard);

    pthread_t rowThreads[SIZE];
    pthread_t colThreads[SIZE];
    pthread_t boxThreads[SIZE];
    int i = 0;

    for (i = 0; i < SIZE; i++)
    {
        int *a = malloc(sizeof(int));
        int *b = malloc(sizeof(int));
        int *c = malloc(sizeof(int));
        *a = i;
        *b = i;
        *c = i;
        pthread_create(&rowThreads[i], NULL, checkRow, a);
        pthread_create(&colThreads[i], NULL, checkCol, b);
        pthread_create(&boxThreads[i], NULL, checkBox, c);
    }
    for (i = 0; i < SIZE; i++)
    {
        pthread_join(rowThreads[i], NULL);
        pthread_join(colThreads[i], NULL);
        pthread_join(boxThreads[i], NULL);
    }

    // 2. Print the results.
    printf("Results:\n");
    bool allRowsPassed = true;
    printf("Rows:\n");
    for (int i = 0; i < SIZE; i++)
    {
        if (!rowCheck[i])
        {
            printf("Row %i did not pass\n", i);
            allRowsPassed = false;
        }
    }
    if (allRowsPassed)
    {
        printf("All rows passed!\n");
    }

    bool allColsPassed = true;
    printf("Cols:\n");
    for (int i = 0; i < SIZE; i++)
    {
        if (!colCheck[i])
        {
            printf("Col %i did not pass\n", i);
            allColsPassed = false;
        }
    }
    if (allColsPassed)
    {
        printf("All cols passed!\n");
    }

    bool allBoxesPassed = true;
    printf("Boxes:\n");
    for (int i = 0; i < SIZE; i++)
    {
        if (!boxCheck[i])
        {
            printf("Box %i did not pass\n", i);
            allBoxesPassed = false;
        }
    }
    if (allBoxesPassed)
    {
        printf("All boxes passed!\n");
    }
    return 0;
}