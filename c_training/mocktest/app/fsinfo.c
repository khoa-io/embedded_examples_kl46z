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
#include "app/app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFF_SIZE 512

/*******************************************************************************
 * Code
 ******************************************************************************/

int32_t app_cmd_fsinfo(fat16_fs_t *fs)
{
    int8_t buff[BUFF_SIZE] = {0};

    printf("FILE SYSTEM INFORMATION: \n");

    memcpy(buff, &fs->header.fs_type, sizeof(fs->header.fs_type));
    printf("Format: %s\n", buff);
    memset(buff, 0, BUFF_SIZE);

    memcpy(buff, &fs->header.oemName, sizeof(fs->header.oemName));
    printf("OEM Name: %s\n", buff);
    memset(buff, 0, BUFF_SIZE);

    printf("Sector size (bytes): %u\n", fs->header.sector_size);
    printf("Cluster size (sectors): %u\n", fs->header.cluster_size);
    printf("Number of reserved sectors: %u\n",
           fs->header.reserved_sectors_count);

    printf("Number of FAT tables: %u\n", fs->header.fats_count);
    printf("FAT tables start at sector 0x%X\n", fs->fat_off);
    printf("FAT table size (byte): %u\n", fs->fat_size);

    printf("Max number of root directory's entries: %u\n",
           fs->header.root_entries_count);

    printf("Total sectors: %u\n", fs->header.total_sectors);

    if (fs->header.media_type == 0xF8)
    {
        printf("Media: Hard disk\n");
    }
    else if (fs->header.media_type == 0xF0)
    {
        printf("Media: Soft disk 1.44M\n");
    }
    else
    {
        printf("Media code: 0x%X\n", fs->header.media_type);
    }

    if (fs->header.vol_label[0])
    {
        memcpy(buff, &fs->header.vol_label, sizeof(fs->header.vol_label));
        printf("Volume label: %s\n", buff);
        memset(buff, 0, BUFF_SIZE);
    }
    else
    {
        printf("Volume label: NO NAME\n");
    }

    printf("Root directory offset: 0x%X\n", fs->root_dir_off);
    printf("Root directory size (byte): %u\n", fs->root_dir_size);
    printf("Data region offset: 0x%X\n", fs->data_off);

    return APP_ERROR_NONE;
}
