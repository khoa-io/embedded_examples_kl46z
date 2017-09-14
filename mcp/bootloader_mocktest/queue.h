/*!
 * @file queue.h Declare queue ADT used in assignment.
 * @author KhoaHV1
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*******************************************************************************
 * Error Code
 ******************************************************************************/

/* No error */
#define QUEUE_ERR_NONE 0x0U
/* Queue is full */
#define QUEUE_ERR_FULL 0x01U
/* Queue is empty */
#define QUEUE_ERR_EMPTY 0x02U
/* Unknown error */
#define QUEUE_ERR_UNKNOWN 0xffffffff

/* Minimum size of the item's data array queue_item::dat and minimum value of
 * queue_item::sz */
#define QUEUE_ITEM_EMPTY_SIZE (0)
/* Maximum size of the item's data array queue_item::dat and maximum value of
 * queue_item::sz */
#define QUEUE_ITEM_MAX_SIZE (64)
#define QUEUE_MAX_ITEM_NUM (128)

/*******************************************************************************
 * Structures and Types
 ******************************************************************************/

/*!
 * @brief Represent Queue's item.
 */
struct queue_item
{
    /* Item's data */
    uint8_t dat[QUEUE_ITEM_MAX_SIZE];

    /* Item's size */
    uint8_t sz;
};

/*!
 * @brief Represent Queue ADT.
 */
struct queue
{
    /* Index of top in array `items`. Items are pushed to top */
    uint8_t top;

    /* Index of bottom in array `items`. Items are popped from bottom */
    uint8_t bot;

    /* Current size */
    uint8_t sz;

    /* Store all items */
    struct queue_item items[QUEUE_MAX_ITEM_NUM];
};

typedef struct queue_item queue_item_t;
typedef struct queue queue_t;

/*******************************************************************************
 * Macros On Queue's Item
 ******************************************************************************/

/*!
 * @brief Add a byte to item's data array. This macro doesn't check if item is
 * full before the addition.
 *
 * @param item [in,out] A queue's item.
 * @param b    [in]     Byte to add.
 */
#define QUEUE_itemAddByte(item, b) ((item).dat[(item).sz++] = (b))

/*!
 * @brief Check if an item is empty or not.
 *
 * @param item A queue's item.
 *
 * @param Return true if item is empty. Return false if item is not empty.
 */
#define QUEUE_itemIsEmpty(item) ((item).sz == QUEUE_ITEM_EMPTY_SIZE)

/*!
 * @brief Check if an item is full or not.
 *
 * @param item A queue's item.
 *
 * @return Return true if item is full. Return false if item is not full yet.
 */
#define QUEUE_itemIsFull(item) ((item).sz >= QUEUE_ITEM_MAX_SIZE)

/*!
 * @brief Get the last byte in queue_item::dat array.
 *
 * @param item A queue's item.
 *
 * @return Return the last byte in queue_item::dat array.
 */
#define QUEUE_itemLastByte(item) ((item).dat[(item).sz - 1])

/*******************************************************************************
 * APIs On Queue
 ******************************************************************************/

/*!
 * @brief Check if queue is empty or not.
 *
 * @return Return true if queue is empty. Return false if queue is not empty.
 */
bool QUEUE_isEmpty();

/*!
 * @brief Check if queue is full or not.
 *
 * @return Return true if queue is full. Return false if queue is not full.
 */
bool QUEUE_isFull();

/*!
 * @brief Get the pointer to the item at top.
 *
 * @param top [in,out] Point to the item at top.
 */
uint32_t QUEUE_top(queue_item_t **top);

/*!
 * @brief Increase the index of top. The actual push operation is combination of
 * QUEUE_top() and QUEUE_push().
 */
void QUEUE_push();

/*!
 * @brief Get the pointer to the item at bottom.
 *
 * @param bot [in,out] Point to the item at bottom.
 */
uint32_t QUEUE_bot(queue_item_t **bot);

/*!
 * @brief Increase the index of bottom. The actual pop operation is combination
 * of QUEUE_bot() and QUEUE_pop().
 */
void QUEUE_pop();

#endif /* _QUEUE_H_ */
