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

#include "hal/haltypes.h"
#include "fat/fat.h"
#include "util/util.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BUFF_SIZE 13

/*******************************************************************************
 * Code
 ******************************************************************************/

void printFat16FsInfo(fat16_fs_t *fsp)
{
    /* Not yet fully implemented */
    int8_t buff[BUFF_SIZE] = {0};

    memcpy(buff, &fsp->header.fileSystemType, sizeof(fsp->header.fileSystemType));
    printf("File system type: %s\n", buff);
    memset(buff, 0, BUFF_SIZE);

    memcpy(buff, &fsp->header.oemName, sizeof(fsp->header.oemName));
    printf("OEM Name: %s\n", buff);
    memset(buff, 0, BUFF_SIZE);

    printf("Sector size (byte): %u\n", fsp->header.sectorSize);
    printf("Cluster size (sector): %u\n", fsp->header.clusterSize);

    printf("FAT tables offset = 0x%X\n", fsp->fatOff);

    printf("Number of FAT tables: %u\n", fsp->header.nFats);

    printf("FAT table size (byte): %u\n", fsp->fatSize);

    printf("Max number of root directory's entries: %u\n", fsp->header.nRootEntries);
    printf("Total sectors: %u\n", fsp->header.nTotalSectors);

    if (fsp->header.mediaFlag == 0xF8)
    {
        printf("Media: Hard disk\n");
    }
    else if (fsp->header.mediaFlag == 0xF0)
    {
        printf("Media: Soft disk 1.44M\n");
    }
    else
    {
        printf("Media code: 0x%X\n", fsp->header.mediaFlag);
    }

    if (fsp->header.volumeLabel[0])
    {
        memcpy(buff, &fsp->header.volumeLabel, sizeof(fsp->header.volumeLabel));
        printf("Volume label: %s\n", buff);
        memset(buff, 0, BUFF_SIZE);
    }
    else
    {
        printf("Volume label: NO NAME\n");
    }

    printf("Root directory offset: 0x%X\n", fsp->rootDirOff);
    printf("Root directory size (byte): %u\n", fsp->rootDirSize);
}
