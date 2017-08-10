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

/* Use this to communicate with device */
kmc_device_t *g_pDevice = NULL;

/*******************************************************************************
 * APIs
 ******************************************************************************/

int32_t fat16_open_fs(char *path, fat16_fs_t *fsp)
{
    /* Temporary variable to store return code from functions */
    int32_t ret = FAT_ERROR_NONE;

    /* Buffer to read first 512 bytes */
    uint8_t buff[BUFF_SIZE];

    ret = kmc_open(path, &g_pDevice);
    if (ret || g_pDevice->status != HAL_STATUS_OPEN)
    {
        ret = FAT_ERROR_CANNOT_OPEN;
        return ret;
    }

    /* Read first 512 bytes then initialize FAT12/16 header */
    ret = g_pDevice->read_single_sector(g_pDevice, 0, buff);
    if (ret < BUFF_SIZE)
    {
        ret = FAT_ERROR_CANNOT_OPEN;
        return ret;
    }

    memcpy(&fsp->header, buff, sizeof(fat16_header_t));

    g_pDevice->sector_size = fsp->header.sector_size;

    fsp->fat_off = fsp->header.reserved_sectors_count;
    fsp->fat_size = fsp->header.fat_size;
    fsp->root_dir_off = fsp->fat_off + fsp->header.fats_count * fsp->fat_size;
    /* root_entries_count * 32 always devide sector_size */
    fsp->root_dir_size = fsp->header.root_entries_count * 32 / fsp->header.sector_size;
    fsp->data_off = fsp->root_dir_off + fsp->root_dir_size;

    return FAT_ERROR_NONE;
}

int32_t fat16_close_fs(fat16_fs_t *fsp)
{
    /* Return code */
    int32_t ret = FAT_ERROR_NONE;

    g_pDevice->close(g_pDevice);
    g_pDevice = NULL;

    if (ret)
    {
        ret = FAT_ERROR_CANNOT_CLOSE;
    }
    else
    {
        ret = FAT_ERROR_NONE;
    }

    return ret;
}
