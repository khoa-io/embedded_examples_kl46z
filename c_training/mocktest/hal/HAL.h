#ifndef _HAL_H_
#define _HAL_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef INT
#define INT int
#endif /* INT */

#ifndef ULONG
#define ULONG unsigned long
#endif /* ULONG */

#ifndef UCHAR
#define UCHAR unsigned char
#endif /* UCHAR */

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief Read and copysdata on sector to buffer.
 *
 * @param index Sector's index.
 * @param buff Destination buffer where data is copied to.
 *
 * @return Return the number of read bytes.
 */
INT kmc_read_sector(ULONG index, UCHAR *buff);

/*!
 * @brief Read multiple sectors and copy data to buffer.
 *
 * @param index First sector's index.
 * @param num Number of sector.
 * @param buff Destination buffer where data is copied to.
 *
 * @return Return thue number of read bytes.
 */
INT kmc_read_multi_sector(ULONG index, UINT num, UCHAR *buff);

#endif /* _HAL_H_ */
