/*!
 * @file queue.h Declare queue ADT used in assignment.
 * @author KhoaHV1
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

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
#define QUEUE_MAX_ITEM_NUM (4)

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

/*!
 * @brief Check if queue is empty or not.
 *
 * @param q Pointer to queue.
 *
 * @return Return true if queue is empty. Return false if queue is not empty.
 */
#define QUEUE_isEmpty(q) ((q)->sz <= 0)

/*!
 * @brief Check if queue is full or not.
 *
 * @param q Pointer to queue.
 *
 * @return Return true if queue is full. Return false if queue is not full.
 */
#define QUEUE_isFull(q) ((q)->sz >= QUEUE_MAX_ITEM_NUM)

/*******************************************************************************
 * APIs
 ******************************************************************************/

/*!
 * @brief Get the pointer to the top of queue and increase index of top. Use
 * the pointer to push data to queue.
 *
 * @param item [in,out] Point to item at top.
 */
uint32_t QUEUE_push(queue_item_t **item);

/*!
 * @brief Pop an item from bottom of queue by get the pointer points to the item
 * at bottom.
 *
 * @param item  [in,out] Point to item at bottom.
 *
 * @return Error code. See Error Code section.
 */
uint32_t QUEUE_pop(queue_item_t **item);

#endif /* _QUEUE_H_ */