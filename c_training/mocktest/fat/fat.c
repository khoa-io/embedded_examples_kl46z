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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hal/HAL.h"
#include "fat.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Let default sector size is 512 bytes. So default buffer size is 512 bytes too
 */
#define BUFF_SIZE 512

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/*******************************************************************************
 * APIs
 ******************************************************************************/

int32_t fat16_open_fs(char *path, fat16_fs_t *fs)
{
    /* Temporary variable to store return code from functions */
    int32_t ret = FAT_ERROR_NONE;

    /* Buffer to read first 512 bytes */
    uint8_t buff[BUFF_SIZE];

    ret = kmc_open(path, &fs->dev);

    if (ret || fs->dev->status != HAL_STATUS_OPEN)
    {
        ret = FAT_ERROR_CANNOT_OPEN;
        return ret;
    }

    /* Read first 512 bytes then initialize FAT12/16 header */
    ret = fs->dev->read_single_sector(fs->dev, 0, buff);
    if (ret < BUFF_SIZE)
    {
        ret = FAT_ERROR_CANNOT_OPEN;
        return ret;
    }

    memcpy(&fs->header, buff, sizeof(fat16_header_t));

    fs->dev->sector_size = fs->header.sector_size;

    fs->fat_off = fs->header.reserved_sectors_count;
    fs->fat_size = fs->header.fat_size;
    fs->root_dir_off = fs->fat_off + fs->header.fats_count * fs->fat_size;
    /* root_entries_count * 32 always devide sector_size */
    fs->root_dir_size = fs->header.root_entries_count * 32 / fs->header.sector_size;
    fs->data_off = fs->root_dir_off + fs->root_dir_size;

    return FAT_ERROR_NONE;
}

int32_t fat16_close_fs(fat16_fs_t *fs)
{
    /* Return code */
    int32_t ret = FAT_ERROR_NONE;

    if (fs->dev == NULL)
    {
        ret = FAT_ERROR_NOT_OPEN;
        return ret;
    }

    fs->dev->close(fs->dev);
    fs->dev = NULL;

    if (ret != HAL_ERROR_NONE)
    {
        ret = FAT_ERROR_CANNOT_CLOSE;
    }
    else
    {
        ret = FAT_ERROR_NONE;
    }

    return ret;
}

DWORD fat_get_cluster_off(fat16_fs_t *fs, DWORD cluster_num)
{
    return ((cluster_num - 2) * fs->header.cluster_size) + fs->data_off;
}

int32_t fat16_read_folder(fat16_fs_t *fs,
                          DWORD off,
                          fat_file_record_t *records,
                          int32_t max,
                          int32_t *total)
{
    /* Return code */
    int32_t rc = FAT_ERROR_NONE;

    /* Stores data read from disk */
    uint8_t *buff = NULL;

    /* Indexing variable */
    int32_t i = 0;
    int32_t j = 0;

    /* Current working record */
    fat_file_record_t record;

    if (fs->dev == NULL || fs->dev->status != HAL_STATUS_OPEN)
    {
        rc = FAT_ERROR_NOT_OPEN;
        return rc;
    }

    buff = (uint8_t *)malloc(fs->root_dir_size * fs->header.sector_size);

    fs->dev->read_multi_sector(fs->dev, off, fs->root_dir_size, buff);

    for (i = 0; i < fs->header.root_entries_count; ++i)
    {
        memcpy(&record, buff + (i * FAT_FILE_RECORD_SIZE), FAT_FILE_RECORD_SIZE);

        if (record.name[0] == FAT_DIR_STAT_FREE || record.name[0] == FAT_DIR_STAT_END)
        {
            break;
        }

        if (record.attrs & ATTR_VOLUME_ID)
        {
            continue;
        }

        records[j++] = record;
    }

    *total = j;
    free(buff);

    return rc;
}

int32_t fat16_read_file(fat16_fs_t *fs,
                        fat_file_record_t *record,
                        int8_t *buff)
{
    /* Return code */
    int32_t rc = FAT_ERROR_NONE;

    /* Indexing variable */
    int32_t i = 0;

    /* Current working sector */
    DWORD curr = fat_get_cluster_off(fs, record->first_cluster_lo);

    for (i = 0; curr < 0xFF8; ++i)
    {
        rc = fs->dev->read_multi_sector(fs->dev,
                                        curr,
                                        fs->header.cluster_size,
                                        buff + i * fs->header.cluster_size);

        if (rc == 0)
        {
            rc = FAT_ERROR_CANNOT_READ;
            break;
        }
    }

    return rc;
}
