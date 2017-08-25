
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

#define ARR_SZ 5

int main()
{
    queue_t q;
    long arr[ARR_SZ] = { 0 };

    long t = 0;
    long u = 0;

    uint32_t rc = 0;

    QUEUE_init(&q, arr, ARR_SZ, sizeof(long));

    u = 15;
    rc = QUEUE_push(&q, &u);
    if (rc != QUEUE_ERR_NONE)
    {
        printf("Error code: 0x%X.\n", rc);
    }
    printf("Pushed %ld.\n", u);
    printf("=========\n");

    rc = QUEUE_pop(&q, &t);
    if (rc != QUEUE_ERR_NONE)
    {
        printf("Error code: 0x%X.\n", rc);
    }
    printf("Pop. t = %ld.\n", t);
    printf("=========\n");

    rc = QUEUE_pop(&q, &t);
    if (rc != QUEUE_ERR_NONE)
    {
        printf("Error code: 0x%X.\n", rc);
    }
    printf("=========\n");

    for (u = 1; u < 2 + ARR_SZ; ++u)
    {
        rc = QUEUE_push(&q, &u);
        if (rc != QUEUE_ERR_NONE)
        {
            printf("Error code: 0x%X.\n", rc);
        }
        else
        {
            printf("Pushed %ld.\n", u);
        }
    }
    printf("=========\n");

    for (u = 1; u < 2 + ARR_SZ; ++u)
    {
        rc = QUEUE_pop(&q, &t);
        if (rc != QUEUE_ERR_NONE)
        {
            printf("Error code: 0x%X.\n", rc);
        }
        else
        {
            printf("Pop. t = %ld.\n", t);
        }
    }
    printf("=========\n");

    for (u = 1; u < 2 + ARR_SZ; ++u)
    {
        rc = QUEUE_push(&q, &u);
        rc = QUEUE_push(&q, &u);
        if (rc != QUEUE_ERR_NONE)
        {
            printf("Error code: 0x%X.\n", rc);
        }
        else
        {
            printf("Pushed %ld.\n", u);
        }

        rc = QUEUE_pop(&q, &t);
        if (rc != QUEUE_ERR_NONE)
        {
            printf("Error code: 0x%X.\n", rc);
        }
        else
        {
            printf("Pop. t = %ld.\n", t);
        }
    }
    printf("=========\n");

    return 0;
}
