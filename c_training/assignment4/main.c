#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "node.h"
#include "linked_list.h"
#include "student.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Max is 200 students. */
#define MAX_SIZE 200

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void displayList(linked_list_t *pList);

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(int argc, char *argv[])
{
    node_t nodeArray[MAX_SIZE];
    student_t dataArray[MAX_SIZE];

    student_t student;

    linked_list_t list;

    int32_t errCode = LIST_ERR_NONE;

    node_t *pNode = NULL;

    errCode = list_Init(&list, nodeArray, dataArray, MAX_SIZE, sizeof(student_t));

    if (errCode != LIST_ERR_NONE)
    {
        printf("Error code: 0x%x\n", errCode);
        return 0;
    }

    errCode = list_AllocateNode(&list, &pNode);

    if (errCode != LIST_ERR_NONE)
    {
        printf("Error code: 0x%x\n", errCode);
        return 0;
    }

    /* Insert new data */
    student.id = 0;
    memcpy(student.name, "Nguyen Van A", 13);
    errCode = list_Insert(&list, &student);
    if (errCode != LIST_ERR_NONE)
    {
        printf("Error code: 0x%x\n", errCode);
        return 0;
    }
    student.id = 1;
    memcpy(student.name, "Nguyen Van B", 13);
    errCode = list_Insert(&list, &student);
    if (errCode != LIST_ERR_NONE)
    {
        printf("Error code: 0x%x\n", errCode);
        return 0;
    }
    student.id = 2;
    memcpy(student.name, "Nguyen Van C", 13);
    errCode = list_Insert(&list, &student);
    if (errCode != LIST_ERR_NONE)
    {
        printf("Error code: 0x%x\n", errCode);
        return 0;
    }
    student.id = 3;
    memcpy(student.name, "Nguyen Van D", 13);
    errCode = list_Insert(&list, &student);
    if (errCode != LIST_ERR_NONE)
    {
        printf("Error code: 0x%x\n", errCode);
        return 0;
    }

    /* Review inserted data */
    printf("List after inserted:\n");
    displayList(&list);

    /* Remove data */
    student.id = 1;
    memcpy(student.name, "Nguyen Van B", 13);
    errCode = list_Remove(&list, &student);
    if (errCode != LIST_ERR_NONE)
    {
        printf("Error code: 0x%x\n", errCode);
        return 0;
    }
    printf("After removed Nguyen Van B:\n");
    displayList(&list);

    /* Insert new data */
    student.id = 10;
    memcpy(student.name, "Le Thi XYZ", 11);
    errCode = list_Insert(&list, &student);
    if (errCode != LIST_ERR_NONE)
    {
        printf("Error code: 0x%x\n", errCode);
        return 0;
    }

    /* Review inserted data */
    printf("List after inserted:\n");
    displayList(&list);

    return 0;
}

void displayList(linked_list_t *pList)
{
    node_t *pCurr = pList->pHead;

    for (; pCurr; pCurr = pCurr->pNext)
    {
        printf("ID: %d. ", ((student_t *)pCurr->pData)->id);
        printf("Name: %s\n", ((student_t *)pCurr->pData)->name);
    }
}
