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

#ifndef _HAL_H_
#define _HAL_H_

/*******************************************************************************
 * Integer type definitions
 ******************************************************************************/

#define UCHAR uint8_t

#define UINT uint32_t

#define INT int32_t

#define ULONG uint64_t

/*******************************************************************************
 * Status codes
 ******************************************************************************/

/* Un-initialized devices have this status code */
#define HAL_STATUS_UNDEFINED 0x0
/* Opened devices have this status code */
#define HAL_STATUS_OPEN 0x1
/* Closed devices have this status code */
#define HAL_STATUS_CLOSE 0x2

/*******************************************************************************
 * Error codes
 ******************************************************************************/

/* No error code */
#define HAL_ERROR_NONE 0x0
/* Cannot open device */
#define HAL_ERROR_CANNOT_OPEN 0x1
/* Cannot close device */
#define HAL_ERROR_CANNOT_CLOSE 0x2
/* Unknown error code */
#define HAL_ERROR_UNKNOWN 0xFFFFFFFF

/*******************************************************************************
 * Types and structues
 ******************************************************************************/

#pragma pack(push)
#pragma pack(1)
/*!
 * @brief FAT module doesn't communicate with device directly. It does it
 * through HAL module by using this structure.
 * The idea was taken from Android HAL
 * (https://source.android.com/reference/hal/structhw__device__t).
 */
typedef struct kmc_device_t
{
    /* Status of this device. See Status codes section. */
    uint16_t status;

    /* Size of a sector (byte). Default is 512 */
    uint16_t sector_size;

    /** Callback functions **/

    /* Close this device */
    uint32_t (*close)(struct kmc_device_t *dev);

    /* Read and copy data on a sector to buffer */
    uint32_t (*read_single_sector)(struct kmc_device_t *dev,
                                   uint64_t index,
                                   uint8_t *buff);

    /* Read and copy data on multi sectors to buffer */
    uint32_t (*read_multi_sector)(struct kmc_device_t *dev,
                                  uint64_t index,
                                  uint32_t num,
                                  uint8_t *buff);
} kmc_device_t;

#pragma pack(pop)

typedef uint32_t (*kmc_close_callback)(kmc_device_t *);
typedef uint32_t (*kmc_read_single_sector_callback)(kmc_device_t *,
                                                    uint64_t,
                                                    uint8_t *);
typedef uint32_t (*kmc_read_multi_sector_callback)(kmc_device_t *,
                                                   uint64_t,
                                                   uint32_t,
                                                   uint8_t *);

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Open a disk image file with the given path.
 *
 * @param path Path to the disk image file.
 *
 * @return Return error code. Read Error codes section for more information.
 */
INT kmc_open(char *path, kmc_device_t **dev);

/*!
 * @brief Close a disk image file with the given path.
 *
 * @param path Path to the device.
 *
 * @return Return error code. Read Error codes section for more information.
 */
INT kmc_close(kmc_device_t *dev);

/*!
 * @brief Read and copy data on sector to buffer.
 *
 * @param index Sector's index.
 * @param buff Destination buffer where data is copied to.
 *
 * @return Return the number of read bytes.
 */
INT kmc_read_sector(ULONG index, UCHAR *buff);

/*!
 * @brief Read multiple sectors and copy data to buffer.
 *
 * @param index First sector's index.
 * @param num Number of sector.
 * @param buff Destination buffer where data is copied to.
 *
 * @return Return the number of read bytes.
 */
INT kmc_read_multi_sector(ULONG index, UINT num, UCHAR *buff);

#endif /* _HAL_H_ */
