#ifndef _ASS3_EX2_H_
#define _ASS3_EX2_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SUCCESS 0
#define ERROR 1

typedef struct mat_t
{
    /* Numbers of rows and columns of matrix. Empty matrix */
    int32_t sz[2];
    /* Points to data */
    int32_t **pdat;
} mat_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Prompt user for matrix's sizes from standard input.
 *
 * @param pmat  [out]   Pointer point to matrix.
 */
void mat_InputSize(mat_t *pmat);

/*!
 * @brief Prompt user for matrix's data from standard input.
 *
 * @param pmat  [out]   Pointer point to matrix.
 * @param pdat   [in]   Pointer point to matrix's data. Need for static
 * allocation of matrix's data.
 */
void mat_InputData(mat_t *pmat, int32_t **pdat);

/*!
 * @brief Print matrix to standard output.
 *
 * @param pmat  Pointer point to matrix.
 */
void mat_Display(mat_t *pmat);

/*!
 * @brief Check if sum matrix A+B exists.
 *
 * @param pa Pointer point to matrix A.
 * @param pb Pointer point to matrix B.
 *
 * @return Returns true if A+B exists. Returns false if A+B doesn't exist.
 */
bool mat_IsAddable(mat_t *pa, mat_t *pb);

/*!
 * @brief Check if product matrix AB exists. A is the left matrix and  B is the
 * right matrix.
 * See more: https://en.wikipedia.org/wiki/Matrix_multiplication.
 *
 * @param pa Pointer point to matrix A.
 * @param pb Pointer point to matrix B.
 *
 * @return Returns true if AB exists. Returns false if AB doesn't exist.
 */
bool mat_IsMultipliable(mat_t *pa, mat_t *pb);

/*!
 * @brief Calculate sum matrix of two matrices.
 * See more: https://en.wikipedia.org/wiki/Matrix_addition.
 *
 * @param pa    [in]    Pointer point to matrix A.
 * @param pb    [in]    Pointer point to matrix B.
 * @param psum  [out]   Pointer point to matrix A+B.
*/
void mat_Sum(mat_t *pa, mat_t *pb, mat_t *psum);

/*!
 * @brief Calculate product matrix AB. A is the left matrix and B is the right
 * matrix.
 *
 * @param pa        [in]    Pointer point to matrix A.
 * @param pb        [in]    Pointer point to matrix B.
 * @param pproduct  [out]   Pointer point to matrix AB.
 */
void mat_Product(mat_t *pa, mat_t *pb, mat_t *pproduct);

#endif /* _ASS3_EX2_H_ */
