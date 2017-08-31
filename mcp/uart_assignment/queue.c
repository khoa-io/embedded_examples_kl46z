/*!
 * @file queue.c Define queue_t type's functions.
 * @author KhoaHV1
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "queue.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*!
 * @brief Calculate offset of an item has index i in `items` array.
 *
 * @param i Index
 * @param q Pointer to queue
 *
 * @return Return offset.
 */
#define OFF(i, q) ((i)*QUEUE_MAX_ITEM_SIZE)

/*!
 * @brief Calculate offset of top item in `items` array.
 *
 * @param q Pointer to queue.
 *
 * @return Return offset of top item.
 */
#define OFF_TOP(q) OFF((q)->top, (q))

/*!
 * @brief Calculate offset of bottom item in `items` array.
 *
 * @param q Pointer to queue.
 *
 * @return Return offset of bottom item.
 */
#define OFF_BOT(q) OFF((q)->bot, (q))

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

uint32_t QUEUE_push(queue_item_t **item)
{
    /* Return code */
    uint32_t rc = QUEUE_ERR_NONE;

    if (QUEUE_isFull(&g_queue))
    {
        rc = QUEUE_ERR_FULL;
        return rc;
    }

    *item = &g_queue.items[g_queue.top];

    /* Increase top */
    ++(g_queue.top);
    g_queue.top = g_queue.top % QUEUE_MAX_ITEM_NUM;
    /* Increase size */
    ++(g_queue.sz);

    return rc;
}

uint32_t QUEUE_pop(queue_item_t **item)
{
    /* Return code */
    uint32_t rc = QUEUE_ERR_NONE;

    if (QUEUE_isEmpty(&g_queue))
    {
        rc = QUEUE_ERR_EMPTY;
        return rc;
    }

    *item = &g_queue.items[g_queue.bot];

    /* Increase bot */
    ++(g_queue.bot);
    g_queue.bot = g_queue.bot % QUEUE_MAX_ITEM_NUM;
    /* Decrease size */
    --(g_queue.sz);

    return rc;
}
