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

#pragma pack(push)
#pragma pack(1)

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Fields start at offset 0 */
#define FAT_PREAMBLE \
    BYTE jmpBoot[3]; \
    BYTE oemName[8]

/* Fields start at offset 11 */
#define FAT_BPB            \
    WORD sectorSize;       \
    BYTE clusterSize;      \
    WORD nReservedSectors; \
    BYTE nFats;            \
    WORD nRootEntries;     \
    WORD nTotalSectors;    \
    BYTE mediaFlag;        \
    WORD nFatsize;         \
    WORD nTrackSize;       \
    WORD nHeads;           \
    DWORD nHiddenSectors;  \
    DWORD nTotalSectorsEx

/*!
 * @brief FAT12/16 First Sector's layout.
 */
struct fat16_header
{
    FAT_PREAMBLE;
    FAT_BPB;

    BYTE driverNumber;
    BYTE reserved1;
    BYTE bootSignature;
    DWORD volumeSerialNumber;
    BYTE volumeLabel[11];
    BYTE fileSystemType[8];
};

#pragma pack(pop)

/*!
 * @brief Represent FAT12/16 file system.
 */
struct fat16_fs
{
    FILE *fp;
    struct fat16_header header;
    /* First FAT table's offset */
    DWORD fatOff;
    /* Size of each FAT table (byte) */
    DWORD fatSize;
    /* Root directory's offset */
    DWORD rootDirOff;
    /* Root directory's size (byte) */
    DWORD rootDirSize;
    /* Data's offset */
    DWORD dataOffset;
};

typedef struct fat16_header fat16_header_t;
typedef struct fat16_fs fat16_fs_t;

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
/* File system is not open. */
#define FAT_ERROR_FS_NOT_OPEN 0x00000001
/* Unknown error */
#define FAT_ERROR_UNKNOWN 0xFFFFFFFF

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
 * check if the file system is FAT12/16 file system first.
 *
 * @param path [in] Path to the input file system.
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
