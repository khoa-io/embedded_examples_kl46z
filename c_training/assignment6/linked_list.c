/*!
 * @file linked_list.c Defines functions work on linked_list_t type which is
 * used in both Assignment 4 and Assignment 6.
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "node.h"
#include "linked_list.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

int32_t list_Init(linked_list_t *pList,
                  node_t *nodeArray, void *dataArray,
                  int32_t arraySize, int32_t dataSize)
{
    int32_t i;

    pList->pHead = NULL;
    pList->nodeArray = nodeArray;
    pList->dataArray = dataArray;
    pList->arraySize = arraySize;
    pList->currentSize = 0;
    pList->dataSize = dataSize;

    memset(nodeArray, 0, sizeof(node_t) * arraySize);
    /* memset(dataArray, 0, dataSize * arraySize); */

    for (i = 0; i < arraySize; ++i)
    {
        pList->nodeArray[i].id = i;
    }

    return LIST_ERR_NONE;
}

int32_t list_AllocateNode(linked_list_t *pList, node_t **pNode)
{
    /* Indexing variable. */
    int32_t i = 0;

    /* Error code which this function returns. */
    int32_t errCode = LIST_ERR_FULL;

    /* Iterating and searching for an empty node. */
    for (i = 0; i < pList->arraySize; ++i)
    {
        if (pList->nodeArray[i].pData == NULL)
        {
            errCode = LIST_ERR_NONE;
            *pNode = &pList->nodeArray[i];
            break;
        }
    }

    /* Reset to default value */
    if (errCode == LIST_ERR_NONE)
    {
        (*pNode)->pNext = NULL;
    }

    return errCode;
}

int32_t list_FreeNode(node_t **pNode)
{
    /* Empty node has no next item. */
    (*pNode)->pNext = NULL;
    /* Empty node has no data. */
    (*pNode)->pData = NULL;
}

int32_t list_Insert(linked_list_t *pList, void *pData)
{
    /* Indexing variable. */
    int32_t i = 0;

    /* Error code which this function returns. */
    int32_t errCode = LIST_ERR_NONE;

    /* Pointer points to the node which will be inserted. */
    node_t *pNode = NULL;

    /* Temporary pointer. */
    node_t *pTmp = NULL;

    /* Check if this list is full. */
    if (pList->currentSize >= pList->arraySize)
    {
        errCode = LIST_ERR_FULL;
        return errCode;
    }

    errCode = list_AllocateNode(pList, &pNode);

    if (errCode != LIST_ERR_NONE)
    {
        return errCode;
    }

    /* Copy data to list's storage. */
    pNode->pData = pList->dataArray + pNode->id * pList->dataSize;
    memcpy(pNode->pData, pData, pList->dataSize);

    /* Increase size of the list */
    pList->currentSize++;

    /* Check if list is empty. */
    if (pList->pHead == NULL)
    {
        /* List is empty. */
        pList->pHead = pNode;
    }
    else
    {
        /* List is not empty. Insert to front. */
        pTmp = pList->pHead;
        pNode->pNext = pTmp;
        pList->pHead = pNode;
    }

    return errCode;
}

int32_t list_Remove(linked_list_t *pList, void *pData)
{
    /* Error code which will be returned. */
    int32_t errCode = LIST_ERR_NOT_FOUND;

    /* Pointer points to current working node. */
    node_t *pCurr = NULL;

    /* Pointer points to previous node of working node. */
    node_t *pPrev = NULL;

    if (pList->pHead == NULL)
    {
        /* This list is empty! */
        errCode = LIST_ERR_EMPTY;
        return errCode;
    }

    /* Searching for the node which has given data and remove it.
    If there are more than one node were found, remove all those nodes. */
    for (pCurr = pList->pHead; pCurr; pPrev = pCurr, pCurr = pCurr->pNext)
    {
        /* Compare data and node's data. If they are equal then we found it. */
        if (memcmp(pData, pCurr->pData, pList->dataSize) != 0)
        {
            /* Not found yet!*/
            continue;
        }

        /* We found an item here and pCurr points to it. Next, check if pCurr
        is pHead or not. */
        if (pPrev == NULL)
        {
            /* pCurr is pHead. */
            pList->pHead = pCurr->pNext;
        }
        else
        {
            /* pCurr is not pHead. */
            pPrev->pNext = pCurr->pNext;
        }

        /* Decrease list's size. */
        pList->currentSize--;

        /* Mark removed as empty node. */
        list_FreeNode(&pCurr);

        errCode = LIST_ERR_NONE;
    }

    return errCode;
}
