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
    /* ID of this node. Usages of ID are various. */
    int32_t id;

    /* Point to data. Empty node has pData == NULL. */
    void *pData;

    /* Point to next node. */
    struct node_t *pNext;
} node_t;

#endif /* _NODE_H_ */
