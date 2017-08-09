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

#include "hal/haltypes.h"
#include "fat.h"

/*******************************************************************************
 * APIs
 ******************************************************************************/

int32_t fat16_open_fs(char *path, fat16_fs_t *fsp)
{
    /* Not yet fully implemented */

    fsp->fp = fopen(path, "rb");

    if (fsp->fp == NULL)
    {
        return FAT_ERROR_UNKNOWN;
    }

    fread(&fsp->header, sizeof(fat16_header_t), 1, fsp->fp);

    fsp->fatOff = fsp->header.sector_size * fsp->header.reserved_sectors;
    fsp->fatSize = fsp->header.fat_size * fsp->header.sector_size;
    fsp->rootDirOff = fsp->fatOff + fsp->header.fats * fsp->fatSize;
    fsp->rootDirSize = fsp->header.root_entries * 32;

    return FAT_ERROR_NONE;
}

int32_t fat16_close_fs(fat16_fs_t *fsp)
{
    if (!fsp->fp)
    {
        return FAT_ERROR_FS_NOT_OPEN;
    }

    if (!fclose(fsp->fp))
    {
        return FAT_ERROR_UNKNOWN;
    }

    return FAT_ERROR_NONE;
}
