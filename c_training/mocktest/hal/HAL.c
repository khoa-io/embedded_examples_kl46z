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

#include "HAL.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Default sector size (bytes) */
#define SECTOR_SIZE 512

/*******************************************************************************
 * Prototype
 ******************************************************************************/

/*!
 * @brief Read a single sector's data and copy to buff.
 *
 * @param dev Device structure.
 * @param index The sector to read.
 * @param buff The buffer to store data
 *
 * @return Return the number of read bytes.
 */
static uint32_t read_single_sector(kmc_device_t *dev,
                                   uint64_t index,
                                   uint8_t *buff);

/*!
 * @brief Read multi sectors data and copy to buff.
 *
 * @param dev Device structure.
 * @param index The first sector to read.
 * @param num The number of sectors.
 * @param buff The buffer to store data
 *
 * @return Return the number of read bytes.
 */
static uint32_t read_multi_sector(kmc_device_t *dev,
                                  uint64_t index,
                                  uint32_t num,
                                  uint8_t *buff);

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/* Keep track of the file which this module use */
FILE *g_fp;

/* Keep track of the device */
kmc_device_t g_dev = {
    .status = HAL_STATUS_UNDEFINED,
    .sector_size = SECTOR_SIZE,
    .close = (kmc_close_callback)kmc_close,
    .read_single_sector = (kmc_read_single_sector_callback)read_single_sector,
    .read_multi_sector = (kmc_read_multi_sector_callback)read_multi_sector,
};

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint32_t read_single_sector(kmc_device_t *dev,
                                   uint64_t index,
                                   uint8_t *buff)
{
    /* Number of read bytes */
    uint32_t n = 0;

    if (dev->status != HAL_STATUS_OPEN)
    {
        n = 0;
    }
    else
    {
        n = kmc_read_sector(index, buff);
    }

    return n;
}

static uint32_t read_multi_sector(kmc_device_t *dev,
                                  uint64_t index,
                                  uint32_t num,
                                  uint8_t *buff)
{
    return kmc_read_multi_sector(index, num, buff);
}

INT kmc_open(char *path, kmc_device_t **dev)
{
    /* Return code */
    int32_t ret = HAL_ERROR_NONE;

    g_fp = fopen(path, "rb");

    if (g_fp == NULL)
    {
        /* Cannot open file */
        ret = HAL_ERROR_CANNOT_OPEN;
    }
    else
    {
        /* Open file successfully */
        g_dev.status = HAL_STATUS_OPEN;
        *dev = &g_dev;
    }

    return ret;
}

INT kmc_close(kmc_device_t *device)
{
    /* Return code */
    int32_t ret = HAL_ERROR_NONE;

    ret = fclose(g_fp);
    if (ret)
    {
        /* Unable to close file */
        ret = HAL_ERROR_CANNOT_CLOSE;
    }
    else
    {
        ret = HAL_ERROR_NONE;
    }

    g_fp = NULL;

    return ret;
}

INT kmc_read_sector(ULONG index, UCHAR *buff)
{
    /* Return code */
    int32_t rc = 0;

    /* Indexing variable */
    int32_t i = 0;

    /* Store data from fgetc */
    uint32_t c;

    rc = fseek(g_fp, index * g_dev.sector_size, SEEK_SET);
    if (rc)
    {
        /* Cannot set file position */
        return 0;
    }

    /* I dont's use fread because if file size < 512 bytes (cannot happen in
    real life but still count) and data was read but fread still returns 0 */
    for (i = 0; i < g_dev.sector_size; ++i)
    {
        c = fgetc(g_fp);
        if (c == EOF)
        {
            break;
        }
        buff[i] = (UCHAR)c;
    }

    return i;
}

INT kmc_read_multi_sector(ULONG index, UINT num, UCHAR *buff)
{
    /* Total number of read bytes */
    int32_t ret = 0;

    /* Number of read bytes in each loop */
    int32_t n = 0;

    /* Indexing variable */
    int32_t i = 0;

    /* Read from sector index to sector index + num */
    for (i = 0; i < num; ++i)
    {
        n = kmc_read_sector(index + i, buff + i * g_dev.sector_size);
        ret += n;

        if (n < g_dev.sector_size)
        {
            /* Reach end of file */
            break;
        }
    }

    return ret;
}
