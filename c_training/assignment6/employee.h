#ifndef _EMPLOYEE_H_
#define _EMPLOYEE_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define CLASS_A ('A')
#define CLASS_B ('B');

typedef struct
{
    /* Employee's ID. ID is unique. */
    int32_t id;
    /* Employee's class from fresher course. Values: 'A' or 'B' */
    int8_t class;
    /* Employee's GPA from fresher course. */
    int8_t gpa;
    /* Show if data was inserted to list. */
    /* Second purpose: padding. */
    uint16_t isInserted;
} employee_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Compare two employee's GPA. Let gpa0 is the first employee's GPA,
 * gpa1 is the second employee's GPA, value(x) is the arithmetic value of GPA:
 * - gpa0 > gpa1 if value(gpa0) > value(gpa1)
 * - if value(gpa0) == value(gpa1):
 *      - gpa0 > gpa1 if first employee is belong to class A.
 *      - gpa0 > gpa1 if two employees are belong to the same class and first
 * employee is inserted first and second employee is inserted latter.
 *
 * @param pEmployee0 Non-null pointer point to data of the first employee.
 * @param pEmployee1 Non-null pointer point to data of the second employee.
 *
 * @returns Let gpa0 is the first employee's GPA, gpa1 is the second employee's
 * GPA, return value is ret:
 * - ret < 0 => gpa0 < gpa1
 * - ret == 0 => gpa0 == gpa1
 * - ret > 0 => gpa0 > gpa1
 */
int32_t employee_CompareGpa(employee_t *pEmployee0, employee_t *pEmployee1);

/*!
 * @brief Check if two employee have the same ID.
 *
 * @param pEmployee0 Non-null pointer point to data of the first employee.
 * @param pEmployee1 Non-null pointer point to data of the second employee.
 *
 * @returns Returns true if 2 employee have the same ID. Returns false if 2
 * employee have differences IDs.
 */
int8_t employee_IsDuplicated(employee_t *pEmployee0, employee_t *pEmployee1);

/*!
 * @brief Check if two employee have the same data.
 *
 * @param pEmployee0 Non-null pointer point to data of the first employee.
 * @param pEmployee1 Non-null pointer point to data of the second employee.
 *
 * @returns Returns true if 2 employee have the same data. Returns false if 2
 * employee have differences data.
 */
int8_t employee_IsEqual(employee_t *pEmployee0, employee_t *pEmployee1);

/*!
 * @brief Generate default value for an employee
 *
 * @param pEmployee [out] Point to variable which will store default value.
 */
void employee_DefaultValue(employee_t *pEmployee);

/*!
 * @brief Insert an employee to the list of employees. If list is not full or
 * there is no employee has the same ID with new employee then new employee will
 * be inserted into list such that: the order of employees in the list is the
 * order of GPA from high to low. This function uses employee_IsDuplicated
 * function to check duplication and employee_CompareGpa function to compare
 * GPA.
 *
 * @param pList [in,out] Non-null pointer point to the list of employees.
 * @param pEmployee [in,out] Non-null pointer oint to the employee you want to
 * insert. If it was inserted, pEmployee->isInserted == true;
 *
 * @return Returns error code:
 * - LIST_ERR_NONE: No error
 * - LIST_ERR_FULL: The list reached max size and cannot insert new
 * item into it anymore.
 * - LIST_ERR_DUPLICATED: There is an employee in the list has ID which equals with
 * the ID of the employee you want to insert into the list.
 */
int32_t employee_InsertToList(linked_list_t *pList, employee_t *pEmployee);

/*!
 * @brief Remove an employee from list if we knew it's data.
 *
 * @param pList [in,out] Non-null pointer point to the list of employees.
 * @param pEmployee [in] Non-null pointer oint to the employee you want to
 * insert.
 *
 * @retrun Return error code.
 */
int32_t employee_RemoveFromList(linked_list_t *pList, employee_t *pEmployee);

/*!
 * @brief Print all employees data to standard output.
 *
 * @param pList Non-null pointer point to the list of employees.
 */
void employee_DisplayList(linked_list_t *pList);

/*!
 * @brief Print employee's data to standard outputl
 *
 * @param pEmployee Non-null pointer point to employee's data.
 */
void employee_Display(employee_t *pEmployee);

#endif /* _EMPLOYEE_H_ */
