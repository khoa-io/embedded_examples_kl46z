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

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
matrix_t * matrix_make(uint32_t nRows, uint32_t nCols)
{
    int32_t i = 0;
    matrix_t * mat = calloc(1, sizeof(matrix_t));
    if (!mat)
    {
        return NULL;
    }

    mat->nRows = nRows;
    mat->nCols = nCols;

    mat->table = (float **) calloc(nRows, sizeof(float *));
    if (!mat->table)
    {
        free(mat);
        return NULL;
    }

    for (i = 0; i < nRows; ++i)
    {
        mat->table[i] = (float *) calloc(nCols, sizeof(float));
        if (!mat->table[i])
        {
            mat = matrix_free(mat);
            return NULL;
        }
    }

    return mat;
}

void *matrix_free(matrix_t * mat)
{
    int32_t i = 0;
    int32_t j = 0;

    if (!mat)
    {
        return;
    }

    for (i = 0; i < mat->nRows; ++i)
    {
        free(mat->table[i]);
    }

    free(mat->table);
    free(mat);
}

bool matrix_is_multipliable(matrix_t *left, matrix_t *right)
{
    return left->nCols == right->nRows;
}

matrix_t *matrix_multiply(matrix_t *left, matrix_t *right)
{
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;

    matrix_t * product = matrix_make(left->nRows, right->nCols);
    if (!product)
    {
        return NULL;
    }

    for (i = 0; i < product->nRows; ++i)
    {
        for (j = 0; j < product->nCols; ++j)
        {
            for (k = 0; k < left->nCols; ++k)
            {
                product->table[i][j] += left->table[i][k] * right->table[k][j];
            }
        }
    }

    return product;
}
