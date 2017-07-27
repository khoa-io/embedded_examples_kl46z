#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>

#include "ass3_ex2.h"

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
void mat_InputSize(mat_t *pmat)
{
    scanf("%d", &pmat->sz[0]);
    scanf("%d", &pmat->sz[1]);
}

void mat_InputData(mat_t *pmat, int32_t **pdat)
{
    int32_t i = 0;
    int32_t j = 0;

    pmat->pdat = pdat;

    for (i = 0; i < pmat->sz[0]; ++i)
    {
        for (j = 0; j < pmat->sz[1]; ++j)
        {
            scanf("%d", &pmat->pdat[i][j]);
        }
    }
}

void mat_Display(mat_t *pmat)
{
    int32_t i = 0;
    int32_t j = 0;

    for (i = 0; i < pmat->sz[0]; ++i)
    {
        for (j = 0; j < pmat->sz[1]; ++j)
        {
            printf(" %d", pmat->pdat[i][j]);
        }
        printf("\n");
    }
}

bool mat_IsAddable(mat_t *pa, mat_t *pb)
{
    return pa->sz[0] == pb->sz[0] && pa->sz[1] == pb->sz[1];
}

bool mat_IsMultipliable(mat_t *pa, mat_t *pb)
{
    return pa->sz[1] == pb->sz[0];
}

void mat_Sum(mat_t *pa, mat_t *pb, mat_t *psum)
{
    int32_t i = 0;
    int32_t j = 0;

    psum->sz[0] = pa->sz[0];
    psum->sz[1] = pa->sz[1];

    for (i = 0; i < pa->sz[0]; ++i)
    {
        for (j = 0; j < pa->sz[1]; ++j)
        {
            psum->pdat[i][j] = pa->pdat[i][j] + pb->pdat[i][j];
        }
    }
}

void mat_Product(mat_t *pa, mat_t *pb, mat_t *pproduct)
{
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;

    pproduct->sz[0] = pa->sz[0];
    pproduct->sz[1] = pb->sz[1];

    for (i = 0; i < pproduct->sz[0]; ++i)
    {
        for (j = 0; j < pproduct->sz[1]; ++j)
        {
            /* Init value of each element */
            pproduct->pdat[i][j] = 0;

            for (k = 0; k < pa->sz[1]; ++k)
            {
                pproduct->pdat[i][j] += pa->pdat[i][k] * pb->pdat[k][j];
            }
        }
    }
}