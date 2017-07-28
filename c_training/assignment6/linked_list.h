/*!
 * @file linked_list.h Defines linked_list_t type which is used in both
 * Assignment 4 and Assignment 6.
 */

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Error codes: */
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

/*!
 * @brief Represent single linked list data structure. It is suitable for both
 * dynamic and static memory allocation.
 */
typedef struct linked_list_t
{
    /* Pointer point to the first node of the list. Empty list has no node,
    therefore pHead == NULL. */
    node_t *pHead;

    /* All nodes of the list are stored in nodeArray. ID of a node is its index
    in nodeArray. This array can be fragmented. */
    node_t *nodeArray;

    /* All data of all nodes of the list are stored dataArray. Each node with
    ID have only one explicit data item in dataArray which is locate by
    node's ID. In other words, node and its data item have the same index
    in their array, this index is node's ID.
    This array can be fragmented because nodeArray can be fragmented. */
    void *dataArray;

    /* nodeArray and dataArray has the same size: arraySize. */
    int32_t arraySize;

    /* Number of current "non-empty" node. */
    int32_t currentSize;

    /* Size of data of a node in bytes. */
    int32_t dataSize;
} linked_list_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Initialize pList with given data.
 *
 * @param pList [in,out] Non-null pointer point to a linked_list_t struct.
 * @param nodeArray [in] An array stores node of the list.
 * @param dataArray [in] An array stores node's data.
 * @param arraySize [in] Size of two above arrays.
 * @param dataSize [in] Size of data of a node.
 *
 * @return Error code.
 */
int32_t list_Init(linked_list_t *pList,
                  node_t *nodeArray, void *dataArray,
                  int32_t arraySize, int32_t dataSize);

/*!
 * @brief Find an empty node in the linked_list_t::nodeArray.
 *
 * @param pList [in] Non-null pointer point to a linked_list_t struct.
 * @param pNode [out] Empty node which was found.
 *
 * @return Returns error code.
 */

int32_t list_AllocateNode(linked_list_t *pList, node_t **pNode);

/*!
 * @brief Free a node then mark it as empty.
 *
 * @param pNode [in] Non-empty node to free.
 *
 * @return Returns error code.
 */
int32_t list_FreeNode(node_t **pNode);

/*!
 * @brief Insert at the front of the list.
 *
 * @param pList [in,out] Non-null pointer point to a linked_list_t struct.
 * @param pData [in] Non-null pointer point to node's data.
 *
 * @return Returns error code.
 */
int32_t list_Insert(linked_list_t *pList, void *pData);

/*!
 * @brief Remove an item from list.
 *
 * @param pList [in,out] Non-null pointer point to a linked_list_t struct.
 * @param pData [in] Non-null pointer point to data of the node you want to
 * remove.
 *
 */
int32_t list_Remove(linked_list_t *pList, void *pData);

#endif /* _LINKED_LIST_H_ */
