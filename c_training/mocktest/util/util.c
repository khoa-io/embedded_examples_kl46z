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

#include "fat/fat.h"
#include "util/util.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BUFF_SIZE 13

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
 * @brief Print file name to stdout.
 *
 * @param pRecord Point to record;
 */
void util_print_file_name(fat_file_record_t *pRecord);

/*!
 * @brief Print file size to stdout.
 *
 * @param pRecord Point to record;
 */
void util_print_file_size(fat_file_record_t *pRecord);

/*!
 * @brief Print file record to stdout.
 *
 * @param pRecord Point to record;
 */
void util_print_file_record(fat_file_record_t *pRecord);

/*******************************************************************************
 * Code
 ******************************************************************************/

void util_print_file_record(fat_file_record_t *pRecord)
{
    /* Print last modified date */
    printf("%.2uh%.2um%.2us, %.2u-%.2u-%.4u\t",
           GET_HOUR(pRecord->modified_time),
           GET_MIN(pRecord->modified_time),
           GET_SEC(pRecord->modified_time),
           GET_DAY(pRecord->modified_date),
           GET_MON(pRecord->modified_date),
           GET_YEAR(pRecord->modified_date));

    /* Print created date */
    printf("%.2uh%.2um%.2us, %.2u-%.2u-%.4u\t",
           GET_HOUR(pRecord->created_time),
           GET_MIN(pRecord->created_time),
           GET_SEC(pRecord->created_time),
           GET_DAY(pRecord->created_date),
           GET_MON(pRecord->created_date),
           GET_YEAR(pRecord->created_date));

    if (!(pRecord->attrs & ATTR_DIRECTORY))
    {
        util_print_file_size(pRecord);
    }
    else
    {
        /* Not yet implemented */
        printf("\t");
    }

    util_print_file_name(pRecord);

    printf("\n");
}

/* Anh Hải comment hàm này ko thực sự cần thiết với Long File Name */
void util_print_file_name(fat_file_record_t *pRecord)
{
    /* Indexing on pRecord-name */
    int32_t i = 0;
    /* Indexing on buff */
    int32_t j = 0;

    uint8_t buff[13] = {0};

    for (; i < 8; ++i)
    {
        if (pRecord->name[i] != 0x20)
        {
            buff[j++] = pRecord->name[i];
        }
    }

    if (!(pRecord->attrs & ATTR_DIRECTORY))
    {
        buff[j++] = '.';

        for (; i < 11; ++i)
        {
            if (pRecord->name[i])
            {
                buff[j++] = pRecord->name[i];
            }
        }
    }
    printf("%s", buff);
}

void util_print_file_size(fat_file_record_t *pRecord)
{
    if (pRecord->file_size < 1024)
    {
        printf("%4.u B\t", pRecord->file_size);
        return;
    }

    if (pRecord->file_size < (1024 * 1024))
    {
        printf("%4.2f KB\t", pRecord->file_size * 1.0 / 1024);
        return;
    }

    if (pRecord->file_size < (1024 * 1024 * 1024))
    {
        printf("%4.2f MB\t", pRecord->file_size * 1.0 / 1024 / 1024);
        return;
    }
}

int32_t util_ls(fat16_fs_t *fsp, char *dir)
{
    /* Return code */
    int32_t ret = UTIL_ERROR_NONE;

    /* Indexing variable */
    int32_t i = 0;

    /* Total of listed files. */
    int32_t total = 0;

    /* Current working file record */
    fat_file_record_t record;

    printf("Created\t\t\tLast modified\t\tSize\tName\n");

    if (dir)
    {
        /* Not yet implemented */
        ret = UTIL_ERROR_UNKNOWN;
        return ret;
    }

    /* Listing in root directory */
    fseek(fsp->fp, fsp->root_dir_off, SEEK_SET);

    for (i = 0; i < fsp->header.root_entries_count; ++i)
    {
        ret = fread((void *)&record, FAT_FILE_RECORD_SIZE, 1, fsp->fp);

        if (ret <= 0)
        {
            ret = UTIL_ERROR_UNKNOWN;
            return ret;
        }

        if (record.name[0] == 0xE5 || record.name[0] == 0x00)
        {
            continue;
        }

        if (record.attrs & ATTR_VOLUME_ID)
        {
            continue;
        }

        ++total;

        util_print_file_record(&record);
    }
    printf("Total %u files and directories.\n", total);

    return ret;
}
