#ifndef _ASSIGNMENT1_H_
#define _ASSIGNMENT1_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Prompt user for array. Also calculate sum value.
 * 
 * @param array Allocated array.
 * @param size The number of elements of array.
 *
 * @return Returns sum value.
 */
int32_t array_intput_and_sum(int32_t array[], uint8_t size);

/*!
 * @brief Print all elements of array.
 *
 * @param array Pointer to array.
 * @param size The number of elements of array.
 */
void array_display(int32_t array[], uint8_t size);

/*!
 * @brief Count the number of elements in an array which are smaller than the avarage value.
 *
 * @param array An interger array up to 20 elements.
 * @param sz Size of above array.
 * @param avg The avarage value.
 *
 * @return The number of elements in an array which are smaller than the avarage value.
 */
int8_t array_count(int32_t array[], uint8_t sz, float avg);

/*!
 * @brief Sort an integer array such that: odd values are in the left and even values are in the 
 * right.
 *
 * @param array An integer array up to 20 elements.
 * @param sz Size of above array.
 */
void array_sort(int32_t array[], uint8_t sz);

#endif /* _ASSIGNMENT1_H_ */