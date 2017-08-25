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
#define OFF(i, q) ((i) * (q)->itemSz)

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
 * Code
 ******************************************************************************/

void QUEUE_init(queue_t *queue, void *items, int32_t arrSz, int32_t itemSz)
{
    queue->items = items;
    queue->arrSz = arrSz;
    queue->itemSz = itemSz;

    queue->top = 0;
    queue->bot = 0;

    queue->sz = 0;
}

uint32_t QUEUE_push(queue_t *queue, void *item)
{
    /* Return code */
    uint32_t rc = QUEUE_ERR_NONE;

    if (QUEUE_isFull(queue))
    {
        rc = QUEUE_ERR_FULL;
        return rc;
    }

    memcpy(((uint8_t *)queue->items) + OFF_TOP(queue), item, queue->itemSz);
    /* Increase top */
    queue->top = ++(queue->top) % queue->arrSz;
    /* Increase size */
    ++(queue->sz);

    return rc;
}

uint32_t QUEUE_pop(queue_t *queue, void *item)
{
    /* Return code */
    uint32_t rc = QUEUE_ERR_NONE;

    if (QUEUE_isEmpty(queue))
    {
        rc = QUEUE_ERR_EMPTY;
        return rc;
    }

    memcpy(item, ((uint8_t *)queue->items) + OFF_BOT(queue), queue->itemSz);
    /* Increase bot */
    queue->bot = ++(queue->bot) % queue->arrSz;
    /* Decrease size */
    --(queue->sz);

    return rc;
}
