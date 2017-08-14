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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>

#include "fat/fat.h"
#include "util.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BUFF_SIZE 13

/*******************************************************************************
 * Macros to time from fat_frec_t
 ******************************************************************************/

#define GET_HOUR(t) (((uint16_t)(t)) >> 11)
#define GET_MIN(t) ((((uint16_t)(t)) >> 5) & 0x3F)
#define GET_SEC(t) ((((uint16_t)(t)) & 0x1F) << 1)

#define GET_YEAR(t) ((((uint16_t)(t)) >> 9) + 1980)
#define GET_MON(t) ((((uint16_t)(t)) >> 5) & 0xF)
#define GET_DAY(t) (((uint16_t)(t)) & 0x1F)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Print file size to stdout.
 *
 * @param record Point to record;
 */
void util_print_file_sz(fat_frec_t *record);

/*******************************************************************************
 * Code
 ******************************************************************************/

void util_print_file_record(fat_frec_t *record)
{
    /* File name */
    int8_t name[13] = {0};

    /* Print last modified date */
    printf("%.2uh%.2um%.2us, %.2u-%.2u-%.4u\t", GET_HOUR(record->modified_time),
           GET_MIN(record->modified_time), GET_SEC(record->modified_time),
           GET_DAY(record->modified_date), GET_MON(record->modified_date),
           GET_YEAR(record->modified_date));

    /* Print created date */
    printf("%.2uh%.2um%.2us, %.2u-%.2u-%.4u\t", GET_HOUR(record->created_time),
           GET_MIN(record->created_time), GET_SEC(record->created_time),
           GET_DAY(record->created_date), GET_MON(record->created_date),
           GET_YEAR(record->created_date));

    if (!(record->attrs & ATTR_DIRECTORY))
    {
        util_print_file_sz(record);
    }
    else
    {
        /* Not yet implemented */
        printf("\t\t");
    }

    util_get_file_name(record, name);

    /* TODO remove when done. */
    if (record->attrs & ATTR_LONG_NAME)
    {
        memcpy(name, record->name, 11);
        printf("Long File Name: %ls", (wchar_t *)name);
    }
    else
    {
        printf("%s\n", name);
    }
}

void util_get_file_name(fat_frec_t *record, int8_t *name)
{
    /* Indexing on record-name */
    int32_t i = 0;
    /* Indexing on buff */
    int32_t j = 0;

    for (; i < 8; ++i)
    {
        if (record->name[i] != 0x20)
        {
            name[j++] = record->name[i];
        }
    }

    if (!(record->attrs & ATTR_DIRECTORY))
    {
        name[j++] = '.';

        for (; i < 11; ++i)
        {
            if (record->name[i])
            {
                name[j++] = record->name[i];
            }
        }
    }
}

void util_print_file_sz(fat_frec_t *record)
{
    if (record->file_sz < 1024)
    {
        printf("%5.5u B\t\t", record->file_sz);
        return;
    }

    if (record->file_sz < (1024 * 1024))
    {
        printf("%5.2f KB\t\t", record->file_sz * 1.0 / 1024);
        return;
    }

    if (record->file_sz < (1024 * 1024 * 1024))
    {
        printf("%5.2f MB\t\t", record->file_sz * 1.0 / 1024 / 1024);
        return;
    }
}
