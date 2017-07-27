/*!
 * Assignment 1 - Entry point
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "assignment1.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NUMBER_OF_ELEMENTS 20

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
    int32_t a[NUMBER_OF_ELEMENTS] = {0};
    int32_t sum = 0;
    float avg = 0;

    printf("Enter array (1 element in 1 line): ");
    sum = array_intput_and_sum(a, NUMBER_OF_ELEMENTS);

    printf("Review your array:");
    array_display(a, NUMBER_OF_ELEMENTS);
    printf("\n");

    avg = ((float)sum) / NUMBER_OF_ELEMENTS;
    printf("Avarage = %.2f\n", avg);

    printf("Numer of elements which have value smaller than avarage: %d.\n",
           array_count(a, NUMBER_OF_ELEMENTS, avg));

    array_sort(a, NUMBER_OF_ELEMENTS);
    printf("Sorted array: ");
    array_display(a, NUMBER_OF_ELEMENTS);
    printf("\n");

    printf("\n");

    return 0;
}
