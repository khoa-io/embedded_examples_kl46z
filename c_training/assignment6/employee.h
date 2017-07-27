#ifndef _EMPLOYEE_H_
#define _EMPLOYEE_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define CLASS_A 0x02
#define CLASS_B 0x01;

typedef struct
{
    /* Employee's ID. ID is unique. */
    int32_t id;
    /* Employee's class from fresher course. */
    int8_t class;
    /* Employee's GPA from fresher course. */
    int8_t gpa;
    /* Inserted stamp. The sooner employee was inserted, the higher value which
    stamp has. There are no 2 employee has the same value. */
    uint16_t stamp;
} employee_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Compare two employee's GPA.
 *
 * @param pEmployee0 Non-null pointer point to data of the first employee.
 * @param pEmployee1 Non-null pointer point to data of the second employee.
 *
 * @returns Let gpa0 is the first employee's GPA,  gpa1* is the second
 * employee's GPA, return value is val;
 * - val < 0 => gpa0 < gpa1
 * - val == 0 => gpa0 == gpa1
 * - val > 0 => gpa0 > gpa1
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
 * @brief Insert an employee to the list of employees.
 *
 * @param pList [in/out] Point to the list of employees.
 * @param pEmployee [in] Point to the employee you want to insert.
 *
 * @return Returns error code:
 * - ERR_SUCCESS: No error
 * - ERR_REACHED_MAX_SIZE: The list reached max size and cannot insert new
 * element into it anymore.
 * - ERR_DUPLICATED: There is an employee in the list has ID which equals with
 * the ID of the employee you want to insert into the list.
 */
uint8_t employee_InsertToList(linked_list_t *pList, employee_t *pEmployee);

#endif /* _EMPLOYEE_H_ */
