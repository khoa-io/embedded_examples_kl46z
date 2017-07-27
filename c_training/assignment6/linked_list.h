#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Error code show that nothing bad happended */
#define ERR_SUCCESS 0x00

/* Error code show that something bad happended and we don't know what it is */
#define ERR_GENERIC_FAILURE 0x01

/* Error code show that cannot insert new element to current list due to
current size of the list reached max value */
#define ERR_REACHED_MAX_SIZE 0x02

/* Error code show that cannot insert new element due to this list doesn't
allow duplicated element */
#define ERR_DUPLICATED 0x03

/*!
 * @brief Single linked list's node.
 * Recommend: uses pArray and pData allocated in stack instead of heap.
 */
typedef struct node_t
{
    /* Point to real data. Empty node has pData == NULL */
    void *pData;

    /* Point to next node, which is always in nodeArray */
    struct node_t *pNext;
} node_t;

/*!
 * @brief Represent a single linked list
 */
typedef struct linked_list_t
{
    /* Pointer point to the first element of this list. If pHead == NULL,
    this list has no element. */
    node_t *pHead;

    /* All nodes of this is list are stored in this array */
    node_t *nodeArray;

    /* All data of all nodes of this list are stored in this array */
    void *dataArray;

    /* nodeArray and dataArray has the same size. This is it. */
    size_t arraySize;

    /* Size of data of a node in bytes */
    size_t dataSize;

    /* Point to default value of data of a node. If a element of a dataArray has
    default value then that element is not belong with any node. */
    void *defaultValue;
} linked_list_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Initialize linked list with data of the first element.
 *
 * @param pList [in/out] Non-null pointer.
 * @param pData [in] Point to data of the first element.
 */
void list_Init(linked_list_t *pList, void *pData);

/*!
 * @brief Insert new element into list at the front of it.
 *
 * @param pList [in] Non-null pointer point to the list.
 * @param pData [in] Point to data. It cannot be NULL.
 *
 * @return Returns error code:
 * - ERR_SUCCESS: No error
 * - ERR_REACHED_MAX_SIZE: The list reached max size and cannot insert new
 * element into it anymore.
 */
uint8_t list_Insert(linked_list_t *pList, void *pData);

#endif /* _LINKED_LIST_H_ */
