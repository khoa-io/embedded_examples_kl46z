#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

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
    uint32_t error = ERR_SUCCESS;

    int16_t stamp = 0xffff;

    node_t nodeArray[MAX_SIZE];
    employee_t employeeArray[MAX_SIZE];

    employee_t defaultValueEmployee;
    employee_t testValues[MAX_SIZE];

    linked_list_t list;

    employee_DefaultValue(&defaultValueEmployee);

    list.pHead = NULL;
    list.nodeArray = nodeArray;
    list.dataArray = employeeArray;
    list.arraySize = MAX_SIZE;
    list.dataSize = sizeof(employee_t);
    list.defaultValue = &defaultValueEmployee;

    testValues[0].id = 1;
    testValues[0].class = CLASS_A;
    testValues[0].gpa = 5;
    testValues[0].stamp = stamp--;
    list.defaultValue = &defaultValueEmployee;

    employee_DefaultValue(&testValues[1]);

    list_Init(&list, &testValues[0]);

    printf("%d\n", employee_IsEqual(&defaultValueEmployee, &testValues[0]));
    printf("%d\n", employee_IsEqual(&defaultValueEmployee, &testValues[1]));

    return 0;
}