#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "assignment1.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define IS_ODD(n) (n) & 1
#define IS_EVEN(n) !((n)&1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
int32_t array_intput_and_sum(int32_t array[], uint8_t size)
{
    int8_t i = 0;
    int32_t sum = 0;

    for (i = 0; i < size; ++i)
    {
        scanf("%d", &array[i]);
        sum += array[i];
    }

    return sum;
}

int8_t array_count(int32_t array[], uint8_t sz, float avg)
{
    int8_t i = 0;
    int8_t count = 0;

    for (i = 0; i < sz; ++i)
    {
        if (array[i] < avg)
        {
            ++count;
        }
    }

    return count;
}

void array_display(int32_t array[], uint8_t size)
{
    uint8_t i = 0;

    for (i = 0; i < size; ++i)
    {
        printf(" %d", array[i]);
    }
    printf(".");
}

void array_sort(int32_t array[], uint8_t sz)
{
    int8_t i = 0;
    int8_t j = sz - 1;
    int32_t tmp = 0;

    for (; i <= j; ++i)
    {
        if (!IS_EVEN(array[i]))
        {
            continue;
        }

        for (; i < j; --j)
        {
            if (IS_ODD(array[j]))
            {
                tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
                break;
            }
        }
    }
}
