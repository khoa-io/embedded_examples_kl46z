#ifndef _NODE_H_
#define _NODE_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Represent single linked list's node.
 */
typedef struct node_t
{
    /* ID of this node. Usages of ID are various (Read linked_list_t
    documentation for more information).
    Every node has ID, even empty node. */
    int32_t id;

    /* Pointer points to data. Empty node has no data (pData == NULL). */
    void *pData;

    /* Pointer points to next node. Empty node has no next node (pNext == NULL),
    but the opposite isn't true. */
    struct node_t *pNext;
} node_t;

#endif /* _NODE_H_ */
