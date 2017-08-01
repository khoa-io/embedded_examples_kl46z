#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "node.h"
#include "linked_list.h"
#include "employee.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

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
        /* If value(gpa0) == value(gpa1) then we say: */
        /* The employee who is belong to class A has larger GPA */
        return -val;
    }

    /* If value(gpa0) == value(gpa1), class0 == class 1, then we say: */
    /* The employee who was inserted first has larger GPA */
    val = pEmployee0->isInserted - pEmployee1->isInserted;

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

    int8_t *p0 = (int8_t *)pEmployee0;
    int8_t *p1 = (int8_t *)pEmployee1;

    for (i = 0; i < sizeof(employee_t); ++i)
    {
        if (p0[i] != p1[i])
        {
            return false;
        }
    }

    return true;
}

int32_t employee_RemoveFromList(linked_list_t *pList, employee_t *pEmployee)
{
    return list_Remove(pList, pEmployee);
}

int32_t employee_InsertToList(linked_list_t *pList, employee_t *pEmployee)
{
    /* Error code which this function returns. */
    int32_t errCode = LIST_ERR_NONE;

    /* If new employee has GPA larger than current employee in loop then
    isGpaLarger == true */
    bool isGpaLarger = false;

    /* Indexing variable */
    int32_t i = 0;

    /* points to inserted node.*/
    node_t *pNode = NULL;

    /* points to current working node. For iterating purpose. */
    node_t *pCurr = NULL;

    /* points to the node which is located before the node we want to insert. */
    node_t *pPrev = NULL;

    /* Check if list is full. */
    if (pList->currentSize >= pList->arraySize)
    {
        errCode = LIST_ERR_FULL;
        return errCode;
    }

    /* Search for an empty node. */
    errCode = list_AllocateNode(pList, &pNode);
    if (errCode != LIST_ERR_NONE)
    {
        return errCode;
    }

    /* Copy data to list's storage. */
    pNode->pData = pList->dataArray + pNode->id * pList->dataSize;
    memcpy(pNode->pData, pEmployee, pList->dataSize);
    ((employee_t *)pNode->pData)->isInserted = false;

    /* Increase size of the list. If it is duplicated then we decrease size */
    /* latter. */
    pList->currentSize++;

    /* Check if list is empty. */
    if (pList->pHead == NULL)
    {
        pList->pHead = pNode;
        return errCode;
    }

    /* Searching for suitable position for insertion. */
    for (pCurr = pList->pHead; pCurr; pPrev = pCurr, pCurr = pCurr->pNext)
    {
        if (employee_IsDuplicated(pNode->pData, pCurr->pData))
        {
            /* There is an employee has the same ID in list => abort */
            errCode = LIST_ERR_DUPLICATED;
            pList->currentSize--;

            /* Reset node's state to empty so we can insert data latter */
            pNode->pData = NULL;
            pNode->pNext = NULL;
            break;
        }

        /* If new employee has GPA larger than current employee then we */
        /* insert new employee before current employee. */
        isGpaLarger = employee_CompareGpa(pNode->pData, pCurr->pData) >= 0;

        if (isGpaLarger && pPrev == NULL)
        {
            /* pCurr points to first employee in list */
            pNode->pNext = pCurr;
            pList->pHead = pNode;
            break;
        }

        if (isGpaLarger && pPrev != NULL)
        {
            /* pCurr doesn't points to first employee in list */
            pNode->pNext = pCurr;
            pPrev->pNext = pNode;
            break;
        }
    }

    if (errCode != LIST_ERR_NONE)
    {
        /* It will be duplicated if we insert new node. */
        return errCode;
    }

    if (!isGpaLarger)
    {
        /* If its GPA is not larger any node then insert to last. */
        pPrev->pNext = pNode;
        pNode->pNext = NULL;
    }

    /* New item was inserted */
    ((employee_t *)pNode->pData)->isInserted = true;
    pEmployee->isInserted = true;

    return errCode;
}

void employee_DisplayList(linked_list_t *pList)
{
    node_t *pCurr = pList->pHead;

    printf("List of %d employees:\n", pList->currentSize);

    for (; pCurr != NULL; pCurr = pCurr->pNext)
    {
        employee_Display((employee_t *)pCurr->pData);
    }
}

void employee_Display(employee_t *p)
{
    printf("Employee %d, class %c, GPA %d\n", p->id, p->class, p->gpa);
}
