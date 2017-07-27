#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "linked_list.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void list_Init(linked_list_t * pList, void *pData)
{
    /* Indexing variable */
    int32_t i = 0;

    /* Fill nodeArray with empty nodes. */
    memset(pList->nodeArray, 0, pList->arraySize * sizeof(node_t));

    /* Fill dataArray with default value. */
    for (i = 0; i < pList->arraySize; ++i)
    {
        memcpy((int8_t *)pList->dataArray + i * pList->dataSize, pList->defaultValue, pList->dataSize);
    }

    /* Init first element with the given data */
    pList->nodeArray[0].pData = pData;

    pList->pHead = &pList->nodeArray[0];
}

uint8_t list_Insert(linked_list_t *pList, void *pData)
{
    /* Not implement yet */
    /* Indexing variable */
    int32_t i = 0;

    return ERR_SUCCESS;
}