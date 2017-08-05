#include <stdio.h>
#include <stdint.h>

/*******************************************************************************
 * Definitions
 *******************************************************************************/
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
 * @brief Get 16 MSB of a number.
 *
 * @param n An uint32_t number.
 *
 * @return Returns 16 MSB.
 */
#define HIGH(n) (((uint32_t) (n)) >> 16)

/*!
 * @brief Get 16 LSB of a number.
 *
 * @param n An uint32_t number.
 *
 * @return Returns 16 LSB.
 */
#define LOW(n) ((n) & MASK(0, 15))

/*!
 * @brief Get i-th bit.
 *
 * @param n An uint32_t number.
 * @param i Position of the bit.
 *
 * @param Returns value of i-th bit.
 */
#define GET(n, i) (((n) >> (i)) & 1U)

/*!
 * @brief Set i-th bit.
 *
 * @param n An uint32_t number.
 * @param i Position of the bit.
 *
 * @return Returns the number with i-th bit has been set.
 */
#define SET(n, i) ((n) + MASK((i), (i)))

/*!
 * @brief Get i-th byte.
 *
 * @param n An uint32_t number.
 * @param i Position of the byte.
 *
 * @return Returns value of i-th byte.
 */
#define GET_BYTE(n, i) (((n) >> (8*(i))) & 0xff)

/*******************************************************************************
* Prototypes
 ******************************************************************************/

/*!
 * @brief Swap i-th bit and (31 - i) th bit of an uint32_t number.
 * For example: 0b11010101011101100001101101110101 => 
 * 0b10101110110110000110111010101011.
 *
 * @param u An uint32_t number.
 *
 * @return Returns an uint32_t number with swapped bits.
 */
uint32_t swapBit(uint32_t n);


/*!
 * @brief Swap i-th byte and (31 - i) th byte of an uint32_t number.
 * For example: 0xABCD => 0XCDAB0000, 0x0A0B0C0D => 0x0D0C0B0A.
 *
 * @param u An uint32_t number.
 *
 * @return Returns an uint32_t number with swapped bytes.
 */
uint32_t swapByte(uint32_t n);

void main()
{
    /*uint32_t n = 0xD5761B75;*/
    uint32_t n = 0xABCD;
    /*uint32_t n = 0x0A0B0C0D;*/

    printf("n           = %X\n", n);
    printf("swapBit(n)  = %X\n", swapBit(n));
    printf("swapByte(n) = %X\n", swapByte(n));
}

uint32_t swapBit(uint32_t n)
{
    /* Indexing variable */
    uint32_t i = 0;

    /* Temporary variable */
    uint32_t t = 0;

    /* Return value */
    uint32_t r = 0;

    /* Copy 16 LSB of n to r. */
    for (i = 0; i < 16; ++i)
    {
        t = GET(n, i);
        if (t)
        { 
            r = SET(r, 15 - i);
        }
    }
    /* Turn 16 LSB into 16 MSB. */
    r = r << 16;

    /* Copy 16 MSB of n to (LSB) of r*/
    for (i = 16; i < 32; ++i)
    {
        t = GET(n, i);
        if (t)
        {
            r = SET(r, 31 - i);
        }
    }

    return r;
}

uint32_t swapByte(uint32_t n)
{
    /* Indexing variable */
    uint32_t i = 0;

    /* Return value */
    uint32_t r = 0;

    /* Array to access r like an uint8_t array. */
    uint8_t *array = &r;

    /* Swap bytes. */
    for (i = 0; i < 4; ++i)
    {
        array[3 - i] = GET_BYTE(n, i);
    }

    return r;
}
