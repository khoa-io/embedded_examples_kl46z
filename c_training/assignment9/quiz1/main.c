#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Let n = 0. MASK(i, j) set bits of n which has index in range [i, j].
 * For example: MASK(1, 3) = 0xe.
 *
 * @param i Start index. Type uint32_t.
 * @param j End index. Type uint32_t.
 *
 * @return Returns the number which has bits from i to j have been set.
 */
#define MASK(i, j) ((UINT32_MAX >> (31 - (j))) & ~((1U << (i)) - 1))

/*!
 * @brief Let n is an uint32_t number. GET(n, i, j) get bits which has index in
 * range [i, j].
 *
 * @param n An uint32_t number.
 * @param i Start index. Type uint32_t.
 * @param j End index. Type uint32_t.
 *
 * @return Returns the value of queried bits.
 */
#define GET(n, i, j) ((n)&MASK((i), (j)))

/*!
 * @brief Mask of two bit zones.
 *
 * @param i Start index.
 * @param j End index.
 * @param k Number of shift bits.
 * @param sh Shift left (<<) or shift right (>>).
 *
 * @return Returns mask of two difference zones.
 */
#define MASK_TWO(i, j, k, sh) ((MASK((i), (j)) sh(k)) + MASK((i), (j)))

/*!
 * @brief Shift only bits have index in range [i, j] by k.
 *
 * @param n The number will be shifted.
 * @param i Start index.
 * @param j End index.
 * @param k Shift k bits.
 * @param sh Shift operation: left (<<) or right (>>).
 *
 * @return The result of this operation.
 */
#define SHIFT(n, i, j, k, sh) ((GET((n), (i), (j)) sh(k)) + (n & (~MASK_TWO((i), (j), (k), sh))))

/*!
 * @brief Shift left only bits have index in range [i, j] by k.
 *
 * @param n The number will be shifted.
 * @param i Start index.
 * @param j End index.
 * @param k Shift k bits.
 *
 * @return The result of this operation.
 */
#define SHIFT_LEFT(n, i, j, k) SHIFT((n), (i), (j), (k), <<)

/*!
 * @brief Shift right only bits have index in range [i, j] by k.
 *
 * @param n The number will be shifted.
 * @param i Start index.
 * @param j End index.
 * @param k Shift k bits.
 *
 * @return The result of this operation.
 */
#define SHIFT_RIGHT(n, i, j, k) SHIFT((n), (i), (j), (k), >>)

/*!
 * @brief Set IRQC of PCR to n.
 *
 * @param pcr Current PCR.
 * @param n The value of IQRC you want to set.
 *
 * @return Returns new PCR with IRQC set to n.
 */
#define SET_IRQC(pcr, n) (((pcr) & ~MASK(16, 19)) + (n << 16))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void testShiftMacros(uint32_t n, uint32_t i, uint32_t j, uint32_t k);

/*******************************************************************************
 * Code
 ******************************************************************************/
void main()
{
    uint32_t i = 0;
    uint32_t j = 0;

    uint32_t n = 0x28B001U;

    uint32_t result = 0x360001;

    for (i = 0; i < 32; ++i)
    {
        for (j = 0; j < 32; ++j)
        {
            printf("MASK(%2d, %2d) = 0x%.8x\n", i, j, MASK(i, j));
        }
    }

    testShiftMacros(0x28B001U, 12U, 15U, 5U);
    testShiftMacros(0x1928B001U, 5U, 15U, 12U);

    printf("%X\n", SET_IRQC(0, 3));
    printf("\n");
}

void testShiftMacros(uint32_t n, uint32_t i, uint32_t j, uint32_t k)
{
    printf("SHIFT_LEFT (0x%.8X, %d, %d, %d) = 0x%.8X\n",
           n, i, j, k, SHIFT_LEFT(n, i, j, k));
    printf("SHIFT_RIGHT(0x%.8X, %d, %d, %d) = 0x%.8X\n",
           n, i, j, k, SHIFT_RIGHT(n, i, j, k));
}
