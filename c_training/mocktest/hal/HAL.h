/*******************************************************************************
 * Copyright 2017 Hoàng Văn Khoa <hoangvankhoa@outlook.com>                    *
 *                                                                             *
 * Permission is hereby granted, free of charge, to any person obtaining a     *
 * copy of this software and associated documentation files (the "Software"),  *
 * to deal in the Software without restriction, including without limitation   *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
 * and/or sell copies of the Software, and to permit persons to whom the       *
 * Software is furnished to do so, subject to the following conditions:        *
 *                                                                             *
 * The above copyright notice and this permission notice shall be included in  *
 * all copies or substantial portions of the Software.                         *
 *                                                                             *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS*
 * IN THE SOFTWARE.                                                            *
 ******************************************************************************/

#ifndef _HAL_H_
#define _HAL_H_

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Open a FAT12/16 file system. You should use function fat_check_fs to
 * check if the file system is FAT12/16 file system first.
 *
 * @param path [in] Path to the input file system.
 * @param fsp [out] Point to FAT12/16 file system data structure.
 *
 * @return Return error code. Read Error codes section for more information.
 */
INT kmc_open_fs(char *path, fat16_fs_t *fsp);

/*!
 * @brief Close an open FAT12/16 file system.
 *
 * @param fsp Point to an open FAT12/16 file system.
 *
 * @return Return error code. Read Error codes section for more information.
 */
INT kmc_close_fs(fat16_fs_t *fsp);

/*!
 * @brief Read and copy data on sector to buffer.
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
 * @return Return the number of read bytes.
 */
INT kmc_read_multi_sector(ULONG index, UINT num, UCHAR *buff);

#endif /* _HAL_H_ */
