#ifndef _ASSIGNMENT_2_H_
#define _ASSIGNMENT_2_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct
{
    /* A table (array) stores values of matrix's elements */
    float **table;
    /* Number of rows of the above table */
    uint32_t nRows;
    /* Number of columns of the above table */
    uint32_t nCols;
} matrix_t;

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief Allocates memory to store a matrix_t variable.
 *
 * @param nRows Number of rows.
 * @param nCols Numer of columns.
 *
 * @return Returns a pointer point to allocated matrix_t variable, or NULL if
 * it cannot allocate memory. Allocated matrix has "empty" table (value of each
 * element is 0).
 */
matrix_t * matrix_make(uint32_t nRows, uint32_t nCols);

/*!
 * @brief De-allocates memory which stores a matrix.
 *
 * @param mat A pointer point to existing matrix.
 */
void *matrix_free(matrix_t *mat);

/*!
 * @brief Check if there is a available multiplication of two matrices.
 * See https://en.wikipedia.org/wiki/Matrix_(mathematics)#Matrix_multiplication
 * for more information.
 *
 * @param left The left matrix. It cannot be NULL.
 * @param right The right matrix. It cannot be NULL.
 *
 * @return Returns true if there is a multiplication available, or false if
 * there is no multiplication available.
 */
bool matrix_is_multipliable(matrix_t *left, matrix_t *right);

/*!
 * @brief Do calculate the multiplication of two matrices. You should use
 * matrix_is_multipliable function to ensure there is a available multiplication of
 * two matrices.
 * See https://en.wikipedia.org/wiki/Matrix_(mathematics)#Matrix_multiplication
 * for more information.
 *
 * @param left The left matrix. It cannot be NULL.
 * @param right The right matrix. It cannot be NULL.
 *
 * @return Returns a pointer to the matrix product or NULL if it's not enough
 * memory for allocating new matrix.
 */
matrix_t *matrix_multiply(matrix_t *left, matrix_t *right);

#endif /* _ASSIGNMENT_2_H_ */
