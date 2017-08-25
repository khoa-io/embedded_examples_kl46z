/*!
 * @file queue.h Declare queue ADT.
 * @author KhoaHV1
 */

#ifndef _QUEUE_H_
#define _QUEUE_H_

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

/*******************************************************************************
 * Structures and Types
 ******************************************************************************/

/*!
 * @brief Represent Queue ADT.
 * Specification of this kind of queue:
 * - There're 2 head: top and bottom.
 * - Push to top and pop from bottom.
 * - Full queue: `top < bot`.
 * - Maximum size: 256.
 */
struct queue
{
    /* Index of the top item of queue in `items` array. */
    uint8_t top;

    /* Index of the bottom item of queue in `items` array. */
    uint8_t bot;

    /* An array store all items of the queue. */
    void *items;

    /* Size of an item */
    uint32_t itemSz;

    /* Size of `items` array */
    uint32_t arrSz;

    /* Current size of queue */
    uint32_t sz;
} queue;

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
#define QUEUE_isEmpty(q) ((q)->sz == 0)

/*!
 * @brief Check if queue is full or not.
 *
 * @param q Pointer to queue.
 *
 * @return Return true if queue is full. Return false if queue is not full.
 */
#define QUEUE_isFull(q) ((q)->sz >= (q)->arrSz)

/*******************************************************************************
 * APIs
 ******************************************************************************/

/*!
 * @brief Initialize queue.
 *
 * @param queue    [in,out] Non-null pointer.
 * @param items    [in]     Point to data array.
 * @param arrSz    [in]     Size of items array.
 * @param itemSz   [in]     Size of an item.
 *
 */
void QUEUE_init(queue_t *queue, void *items, int32_t arrSz, int32_t itemSz);

/*!
 * @brief Push an item to top of queue.
 *
 * @param queue Non-null pointer.
 * @param item  Point to item.
 *
 * @return Error code. See Error Code section.
 */
uint32_t QUEUE_push(queue_t *queue, void *item);

/*!
 * @brief Pop an item from bottom of queue.
 *
 * @param queue [in]  Non-null pointer.
 * @param item  [out] Non-null pointer to store item data.
 *
 * @return Error code. See Error Code section.
 */
uint32_t QUEUE_pop(queue_t *queue, void *item);

#endif /* _QUEUE_H_ */