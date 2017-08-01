#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "node.h"
#include "linked_list.h"
#include "employee.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAX_SIZE 20U

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(int argc, char *argv[])
{
    /* Indexing variables */
    int32_t i = 0;

    /* Store error code */
    uint32_t error = LIST_ERR_NONE;

    /* Stores all nodes struct */
    node_t nodeArray[MAX_SIZE];
    /* Store all nodes data */
    employee_t dataArray[MAX_SIZE];

    /* List of employees */
    linked_list_t list;

    /* Purpose: create differences value for testing */
    employee_t employee;

    /* Initialize default value for each item of dataArray */
    for (i = 0; i < MAX_SIZE; ++i)
    {
        dataArray[i].id = 0xFFFFFFFF;
        dataArray[i].class = 0xFF;
        dataArray[i].gpa = 0xFF;
    }

    /* Initialize list of employees */
    error = list_Init(&list, nodeArray, dataArray, MAX_SIZE, sizeof(employee_t));
    if (error)
    {
        printf("Cannot create list. Error code: 0x%x\n", error);
        return 0;
    }

    /* Test insert new item. No error should happen. */
    employee.id = 0;
    employee.class = CLASS_A;
    employee.gpa = 5;
    error = employee_InsertToList(&list, &employee);
    if (error)
    {
        printf("Cannot insert to list. Error code: 0x%x\n", error);
    }
    printf("After inserted e-%d:\n", employee.id);
    employee_DisplayList(&list);
    printf("\n");

    /* Test insert new item. No error should happen. */
    employee.id = 1;
    employee.class = CLASS_A;
    employee.gpa = 2;
    error = employee_InsertToList(&list, &employee);
    if (error)
    {
        printf("Cannot  insert to list. Error code: 0x%x\n", error);
    }
    printf("After inserted e-%d:\n", employee.id);
    employee_DisplayList(&list);
    printf("\n");

    /* Test insert to list. No error should happen. */
    employee.id = 2;
    employee.class = CLASS_B;
    employee.gpa = 2;
    error = employee_InsertToList(&list, &employee);
    if (error)
    {
        printf("Cannot insert to list. Error code: 0x%x\n", error);
    }
    printf("After inserted e-%d:\n", employee.id);
    employee_DisplayList(&list);
    printf("\n");

    /* Test remove existing item. No error should happen */
    employee.id = 1;
    employee.class = CLASS_A;
    employee.gpa = 2;
    error = employee_RemoveFromList(&list, &employee);
    if (error)
    {
        printf("Cannot remove from list. Error code: 0x%x\n", error);
    }
    printf("After removed e-%d:\n", employee.id);
    employee_DisplayList(&list);
    printf("\n");

    /* Test remove non-existing item. */
    /* LIST_ERR_NOT_FOUND(0x03) should be return */
    employee.id = 1000;
    error = employee_RemoveFromList(&list, &employee);
    if (error)
    {
        printf("Cannot remove from list. Error code: 0x%x\n", error);
    }
    printf("After removed e-%d:\n", employee.id);
    employee_DisplayList(&list);
    printf("\n");

    /* Test insert new item. No error should happen. */
    employee.id = 3;
    employee.class = CLASS_B;
    employee.gpa = 5;
    error = employee_InsertToList(&list, &employee);
    if (error)
    {
        printf("Cannot insert to list. Error code: 0x%x\n", error);
    }
    printf("After inserted e-%d:\n", employee.id);
    employee_DisplayList(&list);
    printf("\n");

    /* Test insert new item which has the same ID with one of item in */
    /* the list. If error == 0x04 then PASS. */
    employee.id = 3;
    employee.class = CLASS_A;
    employee.gpa = 1;
    error = employee_InsertToList(&list, &employee);
    if (error)
    {
        printf("Cannot insert to list. Error code: 0x%x\n", error);
    }
    printf("After inserted e-%d:\n", employee.id);
    employee_DisplayList(&list);
    printf("\n");

    /* Test insert number of items more than MAX_SIZE.
    When inserting some item, the error may be LIST_ERR_DUPLICATED (0x04).
    When inserting e-20, the error should be LIST_ERR_FULL (0x01) */
    for (i = 0; i < MAX_SIZE + 1; ++i)
    {
        employee.id = i;

        if ((i & 1) == 0)
        {
            /* If i is even, class is 'A' */
            employee.class = 'A';
        }
        else
        {
            /* If i is odd, class is 'B */
            employee.class = 'B';
        }

        error = employee_InsertToList(&list, &employee);
        if (error)
        {
            printf("Cannot insert e-%d to list. Error code: 0x%x\n", i, error);
        }
        else
        {
            printf("Inserted e-%d to list.\n", i);
        }
    }
    printf("After inserted MAX_SIZE + 1 employees:\n");
    employee_DisplayList(&list);
    printf("\n");

    /* Remove dataArray[12] */
    i = 12;
    error = employee_RemoveFromList(&list, &dataArray[i]);
    if (error)
    {
        printf("Cannot remove e-%d from list. Error code: 0x%x\n",
               dataArray[i].id, error);
    }
    else
    {
        printf("Removed e-%d from list.\n", dataArray[i].id);
    }
    printf("After removed e-%d:\n", i);
    employee_DisplayList(&list);
    printf("\n");

    /* Remove all employees */
    for (i = 0; i < MAX_SIZE; ++i)
    {
        error = employee_RemoveFromList(&list, &dataArray[i]);
        if (error)
        {
            printf("Cannot remove e-%d from list. Error code: 0x%x\n",
                   dataArray[i].id, error);
        }
        else
        {
            printf("Removed e-%d from list.\n", dataArray[i].id);
        }
    }
    printf("After remoed all element employees:\n");
    employee_DisplayList(&list);
    printf("\n");

    return 0;
}
