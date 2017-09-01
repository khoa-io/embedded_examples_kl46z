/*!
 * @file queue.c Define queue_t type's functions.
 * @author KhoaHV1
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "queue.h"

/*******************************************************************************
 * Global variables
 ******************************************************************************/

static queue_t g_queue = {
    .top = 0,
    .bot = 0,
    .sz = 0
};

/*******************************************************************************
 * Code
 ******************************************************************************/

bool QUEUE_isEmpty()
{
    return g_queue.sz <= 0;
}

bool QUEUE_isFull()
{
    return g_queue.sz >= QUEUE_MAX_ITEM_NUM;
}

uint32_t QUEUE_top(queue_item_t **top)
{
    /* Return code */
    uint32_t rc = QUEUE_ERR_NONE;

    if (QUEUE_isFull())
    {
        rc = QUEUE_ERR_FULL;
        return rc;
    }

    *top = &g_queue.items[g_queue.top];

    return rc;
}

void QUEUE_push()
{
    /* Increase top */
    ++(g_queue.top);
    g_queue.top = g_queue.top % QUEUE_MAX_ITEM_NUM;
    /* Increase size */
    ++(g_queue.sz);
}

uint32_t QUEUE_bot(queue_item_t **bot)
{
    /* Return code */
    uint32_t rc = QUEUE_ERR_NONE;

    if (QUEUE_isEmpty())
    {
        rc = QUEUE_ERR_EMPTY;
        return rc;
    }

    *bot = &g_queue.items[g_queue.bot];
    
    return rc;
}

void QUEUE_pop()
{
    /* Increase bot */
    ++(g_queue.bot);
    g_queue.bot = g_queue.bot % QUEUE_MAX_ITEM_NUM;
    /* Decrease size */
    --(g_queue.sz);
}
