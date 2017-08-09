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
#include <stddef.h>

#include "haltypes.h"
#include "fat/fat.h"
#include "HAL.h"

/*******************************************************************************
 * Global variables
 ******************************************************************************/
/* Access FAT12/16 file system */
fat16_fs_t g_fs;

/*******************************************************************************
 * Code
 ******************************************************************************/

INT kmc_open_fs(char *path, fat16_fs_t *fsp)
{
    /* Return code */
    int32_t ret = FAT_ERROR_NONE;

    ret = fat16_open_fs(path, fsp);
    g_fs = *fsp;

    return ret;
}

INT kmc_close_fs(fat16_fs_t *fsp)
{
    /* Return code */
    int32_t ret = FAT_ERROR_NONE;

    ret = fat16_close_fs(fsp);
    g_fs.fp = NULL;

    return ret;
}

INT kmc_read_sector(ULONG index, UCHAR *buff)
{
    int32_t ret = 0;

    fseek(g_fs.fp, index * g_fs.header.sectorSize, SEEK_SET);
    ret = fread(buff, g_fs.header.sectorSize, 1, g_fs.fp);
    ret = ret * g_fs.header.sectorSize;

    return ret;
}

INT kmc_read_multi_sector(ULONG index, UINT num, UCHAR *buff)
{
    int32_t ret = 0;

    fseek(g_fs.fp, index * g_fs.header.sectorSize, SEEK_SET);
    ret = fread(buff, g_fs.header.sectorSize, num, g_fs.fp);
    ret = ret * g_fs.header.sectorSize;

    return ret;
}
