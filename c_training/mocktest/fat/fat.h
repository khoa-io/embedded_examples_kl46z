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

#ifndef _FAT_H_
#define _FAT_H_

/*******************************************************************************
 * Integer type definitions
 ******************************************************************************/

#define BYTE uint8_t

#define WORD uint16_t

#define DWORD uint32_t

/*******************************************************************************
 * File system type codes
 ******************************************************************************/

/* Not a FAT file system */
#define FS_NOT_FAT 0x00
/* FAT12 file system */
#define FS_FAT12 0x0C
/* FAT16 file system */
#define FS_FAT16 0x10
/* FAT32 file system */
#define FS_FAT32 0x40

/*******************************************************************************
 * Error codes
 ******************************************************************************/

/* No error */
#define FAT_ERROR_NONE 0
/* Cannot open the device. */
#define FAT_ERROR_CANNOT_OPEN 0x1
/* Cannot close the device */
#define FAT_ERROR_CANNOT_CLOSE 0x2
/* Unknown error */
#define FAT_ERROR_UNKNOWN 0xFFFFFFFF

/*******************************************************************************
 * File's attributes
 ******************************************************************************/

#define ATTR_READ_ONLY 0x01
#define ATTR_HIDDEN 0x02
#define ATTR_SYSTEM 0x04
#define ATTR_VOLUME_ID 0x08
#define ATTR_DIRECTORY 0x10
#define ATTR_ARCHIVE 0x20
#define ATTR_LONG_NAME ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FAT_FILE_RECORD_SIZE 32

/* Fields start at offset 0 */
#define FAT_PREAMBLE \
    BYTE jmpBoot[3]; \
    BYTE oemName[8]

/* Fields start at offset 11 */
#define FAT_BPB                  \
    WORD sector_size;            \
    BYTE cluster_size;           \
    WORD reserved_sectors_count; \
    BYTE fats_count;             \
    WORD root_entries_count;     \
    WORD total_sectors;          \
    BYTE media_type;             \
    WORD fat_size;               \
    WORD track_size;             \
    WORD heads;                  \
    DWORD hidden_sectors;        \
    DWORD total_sectors_ex

#pragma pack(push)
#pragma pack(1)

/*!
 * @brief FAT12/16 First Sector's layout.
 */
struct fat16_header
{
    FAT_PREAMBLE;
    FAT_BPB;

    BYTE drv_num;
    BYTE reserved1;
    BYTE boot_signature;
    DWORD vol_serial;
    BYTE vol_label[11];
    BYTE fs_type[8];
};

struct fat_file_record
{
    BYTE name[11];
    BYTE attrs;
    BYTE nt_reserved;
    BYTE created_time_tenth;
    WORD created_time;
    WORD created_date;
    WORD last_access_date;
    WORD first_cluster_hi;
    WORD modified_time;
    WORD modified_date;
    WORD first_cluster_lo;
    DWORD file_size;
};

#pragma pack(pop)

/*!
 * @brief Represent FAT12/16 file system.
 */
struct fat16_fs
{
    FILE *fp;
    struct fat16_header header;
    /* First sector of FAT Table */
    DWORD fat_off;
    /* Size of each FAT table (sectors) */
    DWORD fat_size;
    /* First sector of root directory */
    DWORD root_dir_off;
    /* Root directory's size (sectors) */
    DWORD root_dir_size;
    /* First sector of data region */
    DWORD data_off;
};

typedef struct fat16_header fat16_header_t;
typedef struct fat16_fs fat16_fs_t;
typedef struct fat_file_record fat_file_record_t;

/*******************************************************************************
 * APIs
 ******************************************************************************/

/*!
 * @brief Check if a file system is FAT or not.
 *
 * @param path Path to the input file system.
 *
 * @return Return file system type code:
 * - 0x00: Not a FAT file system.
 * - 0x0C: FAT12 file system.
 * - 0x10: FAT16 file system.
 * - 0x40: FAT32 file system.
 */
int32_t fat_check_fs(char *path);

/*!
 * @brief Open a FAT12/16 file system. You should use function fat_check_fs to
 * check if the file system is FAT12/16 file system before calling this.
 *
 * @param path [in] Path of the device.
 * @param fsp [out] Point to FAT12/16 file system data structure.
 *
 * @return Return error code. Read Error codes section for more information.
 */
int32_t fat16_open_fs(char *path, fat16_fs_t *fsp);

/*!
 * @brief Close an open FAT12/16 file system.
 *
 * @param fsp Point to an open FAT12/16 file system.
 *
 * @return Return error code. Read Error codes section for more information.
 */
int32_t fat16_close_fs(fat16_fs_t *fsp);

#endif /* _FAT_H_ */
