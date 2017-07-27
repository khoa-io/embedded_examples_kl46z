#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "linked_list.h"
#include "employee.h"

int32_t employee_CompareGpa(employee_t *pEmployee0, employee_t *pEmployee1)
{
    int32_t val = pEmployee0->gpa - pEmployee1->gpa;
    if (val != 0)
    {
        return val;
    }

    val = pEmployee0->class - pEmployee1->class;
    if (val != 0)
    {
        return val;
    }

    val = pEmployee0->stamp - pEmployee1->stamp;

    return val;
}

int8_t employee_IsDuplicated(employee_t *pEmployee0, employee_t *pEmployee1)
{
    return pEmployee0->id == pEmployee1->id;
}

int8_t employee_IsEqual(employee_t *pEmployee0, employee_t *pEmployee1)
{
    /* Indexing variable */
    int32_t i = 0;

    /* Cast employee to an array of int8_t, then compare two array. */

    int8_t *p0 = (int8_t *) pEmployee0;
    int8_t *p1 = (int8_t *) pEmployee1;

    for (i = 0; i < sizeof(employee_t); ++i)
    {
        if (p0[i] != p1[i])
        {
            return false;
        }
    }

    return true;
}

void employee_data(employee_t *pEmployee)
{
    pEmployee->id = 0xFFFFFFFF;
    pEmployee->class = 0xFF;
    pEmployee->gpa = 0xFF;
    pEmployee->stamp = 0;
}

uint8_t employee_InsertToList(linked_list_t *pList, employee_t *pEmployee)
{
    bool isDuplicated = false;

    /* Indexing variable */
    int32_t i = 0;

    employee_t data;

    /* Point to current working node. For iterating. */
    node_t *pCurr = NULL;

    /* Point to the node which is located before the node we want to insert. */
    node_t *pPrev = NULL;

    employee_data(&data);

    /* Find an entry of pList->dataArray which is available */
    for (i = 0; i < pList->arraySize; ++i)
    {
        if (employee_IsEqual(*pList->dataArray[i], &data))
        {

        }
    }

    for (pCurr = pList->pHead; pCurr != NULL; pCurr = pCurr->pNext)
    {
        if (employee_IsDuplicated(pEmployee, (employee_t *)pCurr->pData))
        {
            return ERR_DUPLICATED;
        }

        if (employee_CompareGpa(pEmployee, (employee_t *)pCurr->pData) >= 0)
        {

        }

        pPrev = pCurr;
    }

    return ERR_SUCCESS;
}
