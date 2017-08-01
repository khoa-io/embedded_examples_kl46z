/*!
 * @file linked_list.h Defines linked_list_t type which is used in both
 * Assignment 4 and Assignment 6.
 */

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************/
/* linked_list_t's error codes: */
/* No error. */
#define LIST_ERR_NONE 0
/* List was full. */
#define LIST_ERR_FULL 0x01
/* List was empty. */
#define LIST_ERR_EMPTY 0x02
/* Not found such item. */
#define LIST_ERR_NOT_FOUND 0x03
/* Duplicated item */
#define LIST_ERR_DUPLICATED 0x04
/* Unknown error. */
#define LIST_ERR_UNKNOWN 0xFFFFFFFF
/******************************************************************************/

/*!
 * @brief Represent single linked list data structure. All nodes and data are
 * stored in arrays.
 */
typedef struct linked_list_t
{
    /* Pointer points to the first node of the list. Empty list has no node,
    therefore pHead == NULL. */
    node_t *pHead;

    /* All nodes of the list are stored in nodeArray. ID of a node is its index
    in nodeArray. This array can be fragmented but we can ignore it. */
    node_t *nodeArray;

    /* All data of all nodes of the list are stored in dataArray.
    If node has index i (in nodeArray) then node's data has index i
    (in dataArray) too. In other words, node and its data item have the same
    index in their array, and index is node's ID.
    This array can be fragmented because nodeArray can be fragmented but we can
    ignore it. */
    void *dataArray;

    /* Two arrays above (nodeArray and dataArray) have the same size
    (arraySize). */
    int32_t arraySize;

    /* Number of "non-empty" node. An empty node has no data, therefore, we can
    assign new data to it, and currentSize increases. If we "un-assign" data of
    a node, currentSize decreases.*/
    int32_t currentSize;

    /* Size of data of a node in bytes. It's used to locate node data. */
    int32_t dataSize;
} linked_list_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Initialize list from info you provided. After initialization, pList
 * points to an empty list.
 *
 * @param pList [in,out] Non-null pointer points to a linked_list_t struct.
 * @param nodeArray [in] An array stores nodes of the list.
 * @param dataArray [in] An array stores node's data.
 * @param arraySize [in] Size of two above arrays.
 * @param dataSize [in] Size of data of a node.
 *
 * @return Error code. See linked_list_t's error codes.
 */
int32_t list_Init(linked_list_t *pList,
                  node_t *nodeArray, void *dataArray,
                  int32_t arraySize, int32_t dataSize);

/*!
 * @brief Find an empty node in the linked_list_t::nodeArray.
 * Refer to node_t's documentation for more information about empty node.
 *
 * @param pList [in] Non-null pointer points to a linked_list_t struct.
 * @param pNode [out] Empty node which was found.
 *
 * @return Error code. See linked_list_t's error codes.
 */

int32_t list_AllocateNode(linked_list_t *pList, node_t **pNode);

/*!
 * @brief Reset node. In other words, this function marks a node as empty node.
 *
 * @param pNode [in] Non-empty node to free.
 *
 * @return Error code. See linked_list_t's error codes.
 */
int32_t list_FreeNode(node_t **pNode);

/*!
 * @brief Insert an item at the front of the list.
 *
 * @param pList [in,out] Non-null pointer points to a linked_list_t struct.
 * @param pData [in] Non-null pointer points to node's data.
 *
 * @return Error code. See linked_list_t's error codes.
 */
int32_t list_Insert(linked_list_t *pList, void *pData);

/*!
 * @brief Search for an item and remove it from list.
 *
 * @param pList [in,out] Non-null pointer points to a linked_list_t struct.
 * @param pData [in] Non-null pointer points to data of the node you want to
 * remove.
 *
 */
int32_t list_Remove(linked_list_t *pList, void *pData);

#endif /* _LINKED_LIST_H_ */
