/*!
 * Assignment 3 - Exercise 2 - Entry point
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "ass3_ex2.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* The max elements of each matrix */
// #define MAT_SIZE 65535
#define MAT_SIZE 100

/* The number of matrices */
#define N 3

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(int argc, char *argv[])
{
    /* Indexing variables */
    int32_t i = 0;
    int32_t j = 0;

    /* Array of matrices (in order): matrix A, matrix B and result matrix */
    mat_t mat[N];

    /* Existences (in order) of A+B, AB and BA*/
    bool existence[N] = {false};

    /* Data of three matrices (in order) above */
    int32_t dat[N][MAT_SIZE][MAT_SIZE] = {0};

    /* Array of pointers point to matrices data */
    void *p[N][MAT_SIZE] = {NULL};

    /* Initialize array of pointers */
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < MAT_SIZE; ++j)
        {
            /* Each pointer points to data of a rows */
            p[i][j] = &dat[i][j];
        }
    }

    /* Static allocate result matrix */
    mat[2].pdat = (int32_t **)p[2];

    /* Input sizes of matrices A and B */
    mat_InputSize(&mat[0]);
    mat_InputSize(&mat[1]);

    /* Input data of matrices A and B*/
    mat_InputData(&mat[0], (int32_t **)p[0]);
    mat_InputData(&mat[1], (int32_t **)p[1]);

    if (mat_IsAddable(&mat[0], &mat[1]))
    {
        existence[0] = true;
        printf("Matrix A, Matrix B can add together\n");
    }
    else
    {
        printf("Matrix A, Matrix B cant add together\n");
    }

    if (mat_IsMultipliable(&mat[0], &mat[1]))
    {
        existence[1] = true;
        printf("Matrix A, Matrix B can multi together\n");
    }
    else
    {
        printf("Matrix A, Matrix B cant multi together\n");
    }

    if (mat_IsMultipliable(&mat[1], &mat[0]))
    {
        existence[2] = true;
        printf("Matrix B, Matrix A can multi together\n");
    }
    else
    {
        printf("Matrix B, Matrix A cant multi together\n");
    }

    printf("Matrix A:\n");
    mat_Display(&mat[0]);
    printf("Matrix B:\n");
    mat_Display(&mat[1]);

    printf("Matrix A + Matrix B:");
    if (existence[0])
    {
        mat_Sum(&mat[0], &mat[1], &mat[2]);
        printf("\n");
        mat_Display(&mat[2]);
    }
    else
    {
        printf(" N/A\n");
    }

    printf("Matrix A * Matrix B:");
    if (existence[1])
    {
        mat_Product(&mat[0], &mat[1], &mat[2]);
        printf("\n");
        mat_Display(&mat[2]);
    }
    else
    {
        printf(" N/A\n");
    }

    printf("Matrix B * Matrix A:");
    if (existence[2])
    {
        mat_Product(&mat[1], &mat[0], &mat[2]);
        printf("\n");
        mat_Display(&mat[2]);
    }
    else
    {
        printf(" N/A\n");
    }

    printf("\n");
    return 0;
}
