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
#define QUEUE_ERR_NONE 0
/* Queue is full */
#define QUEUE_ERR_FULL 0x01
/* Queue is empty */
#define QUEUE_ERR_EMPTY 0x02
/* Unknown error */
#define QUEUE_ERR_UNKNOWN 0xffffffff

#define QUEUE_MAX_ITEM_SIZE (128U)
#define QUEUE_MAX_ITEM_NUM (8)

/*******************************************************************************
 * Structures and Types
 ******************************************************************************/

/*!
 * @brief Represent Queue's element.
 */
struct queue_item
{
    /* Item's data */
    uint8_t dat[QUEUE_MAX_ITEM_SIZE];

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
 * Macros (Fake Functions)
 ******************************************************************************/

/*******************************************************************************
 * APIs
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