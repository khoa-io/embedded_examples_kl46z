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

/*!
 * @file util.h Declaration of utilites functions.
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "fat/fat.h"

/*******************************************************************************
 * Error codes
 ******************************************************************************/
/* No error */
#define UTIL_ERROR_NONE 0x0

/* Unknown error */
#define UTIL_ERROR_UNKNOWN 0xFFFFFFFF

/*******************************************************************************
 * APIs
 ******************************************************************************/

/*!
 * @brief Print file record to stdout.
 *
 * @param record File record.
 */
void util_print_file_record(fat_file_record_t *record);

/*!
 * @brief Get file name from a file record.
 *
 * @param record [int] File record.
 * @param name [out] Store name of the file. This function treats as the param
 * is large enough to store file name.
 */
void util_get_file_name(fat_file_record_t *record, int8_t *name);

#endif /* _UTIL_H_ */
