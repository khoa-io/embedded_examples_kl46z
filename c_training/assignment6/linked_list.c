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
    memset(dataArray, 0, dataSize * arraySize);

    for (i = 0; i < arraySize; ++i)
    {
        pList->nodeArray[i].id = i;
    }

    return LIST_ERR_NONE;
}

int32_t list_AllocateNode(linked_list_t *pList, node_t **pNode)
{
    int32_t i = 0;
    int32_t errCode = LIST_ERR_FULL;

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
    (*pNode)->pNext = NULL;
    (*pNode)->pData = NULL;
}

int32_t list_Insert(linked_list_t *pList, void *pData)
{
    int32_t i = 0;

    int32_t errCode = LIST_ERR_NONE;

    /* Point to inserted node. Can be NULL. */
    node_t *pNode;
    /* Temporary pointer. */
    node_t *pTmp;

    /* Check if list is full. */
    if (pList->currentSize >= pList->arraySize)
    {
        return LIST_ERR_FULL;
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

    if (pList->pHead == NULL)
    {
        pList->pHead = pNode;
    }
    else
    {
        pTmp = pList->pHead;
        pNode->pNext = pTmp;
        pList->pHead = pNode;
    }

    return errCode;
}

int32_t list_Remove(linked_list_t *pList, void *pData)
{
    int32_t errCode = LIST_ERR_NOT_FOUND;

    /* Point to current working node. */
    node_t *pCurr = NULL;
    /* Point to previous working node. */
    node_t *pPrev = NULL;

    if (pList->pHead == NULL)
    {
        errCode = LIST_ERR_EMPTY;
        return errCode;
    }

    /* Search for the node which has given data and remove. */
    for (pCurr = pList->pHead;
         pCurr != NULL;
         pPrev = pCurr, pCurr = pCurr->pNext)
    {
        if (memcmp(pData, pCurr->pData, pList->dataSize) != 0)
        {
            /* Not found yet!*/
            continue;
        }

        // Found!
        if (pPrev == NULL)
        {
            /* Found pCurr is pHead. Remove node: */
            pList->pHead = pCurr->pNext;
        }
        else
        {
            /* Found pCurr is not pHead. Remove current */
            pPrev->pNext = pCurr->pNext;
        }

        /* Decrease list's size */
        pList->currentSize--;
        list_FreeNode(&pCurr);

        errCode = LIST_ERR_NONE;
    }

    return errCode;
}
