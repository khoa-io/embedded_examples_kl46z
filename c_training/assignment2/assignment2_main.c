#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "assignment2.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Prompt user for matrix.
 *
 * @return Returns a pointer point to matrix or NULL if user doesn't input valid
 * matrix.
 */
static matrix_t *inputMatrix();

/*!
 * @brief Print matrix to screen.
 *
 * @param mat A pointer point to existing matrix. It cannot be NULL.
 *
 */
static void displayMatrix(matrix_t * mat);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static void displayMatrix(matrix_t * mat)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < mat->nRows; ++i)
    {
        for (j = 0; j < mat->nCols; ++j)
        {
            printf(" %.2f", mat->table[i][j]);
        }
        printf("\n");
    }
}

static matrix_t *inputMatrix()
{
    /* Indexing variables*/
    int32_t i = 0;
    int32_t j = 0;

    /* Number of rows and columns */
    uint32_t nRows = 0;
    uint32_t nCols = 0;

    /* Prompt user input data for this matrix */
    matrix_t * mat;

    printf("Number of rows: ");
    scanf("%d", &nRows);

    printf("Number of columns: ");
    scanf("%d", &nCols);

    mat = matrix_make(nRows, nCols);

    if (!mat)
    {
        return NULL;
    }

    printf("Enter values: \n");
    for (i = 0; i < mat->nRows; ++i)
    {
        for (j = 0; j < mat->nCols; ++j)
        {
            printf("A[%d][%d]: ", i, j);
            scanf("%f", &mat->table[i][j]);
        }
    }

    return mat;
}

int main(int argc, char *argv[])
{
    matrix_t *a = NULL;
    matrix_t *b = NULL;

    bool multipliable = false;

    /* Matrix product */
    matrix_t *c = NULL;

    printf("Input matrix A: \n");
    a = inputMatrix();
    if (!a)
    {
        printf("Cannot create matrix A!\n");
        return 0;
    }

    printf("Input matrix B: \n");
    b = inputMatrix();
    if (!b)
    {
        printf("Cannot create matrix A!\n");
        return 0;
    }

    printf("Review matrix A:\n");
    displayMatrix(a);
    printf("Review matrix B:\n");
    displayMatrix(b);

    if (matrix_is_multipliable(a, b))
    {
        multipliable = true;
        printf("AB exist! It is: \n");
        c = matrix_multiply(a, b);
        displayMatrix(c);
    }

    if (matrix_is_multipliable(b, a))
    {
        multipliable = true;
        printf("BA exist! It is: \n");
        c = matrix_multiply(b, a);
        displayMatrix(c);
    }

    if (!multipliable)
    {
        printf("Neither AB nor BA exist!\n");
    }

    matrix_free(a);
    matrix_free(b);
    matrix_free(c);

    return 0;
}
