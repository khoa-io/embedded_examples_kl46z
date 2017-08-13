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
#define FS_UNKNOWN 0x00
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
/* File system is not opened */
#define FAT_ERROR_NOT_OPEN 0x3
/* Cannot read file */
#define FAT_ERROR_CANNOT_READ 0x4
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
#define ATTR_LONG_NAME                                                         \
    (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)

#define ATTR_LONG_NAME_MASK                                                    \
    (ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID |             \
     ATTR_DIRECTORY | ATTR_ARCHIVE)

/*******************************************************************************
 * FAT directory entry statistics code in fat_frec_t::name[0]
 ******************************************************************************/

/* The directory entry is free (there is no  file or directory name in this
entry) */
#define FAT_DIR_STAT_FREE 0xE5
/* The directory entry is free (same as FAT_DIR_STAT_FREE) and there are no
allocated directory entries after this one. */
#define FAT_DIR_STAT_END 0x00
/* The actual file name character for this byte is 0xE5. Not used in this
module yet */
#define FAT_DIR_STAT_KANJI 0x05

/*******************************************************************************
 * Macros
 ******************************************************************************/

/*!
 * @brief Compute sector number of the first sector of a cluster.
 *
 * @param  fs       Pointer points to fat_fs structure.
 * @param  clus_num Cluster number.
 * @return          The sector number of the first sector of the cluster.
 */
#define FAT_CLUS_SEC_NUM(fs, clus_num)                                         \
    ((((clus_num)-2) * (fs)->bs.clus_sz) + (fs)->data_sec_num)

/*******************************************************************************
 * FAT's structures.
 ******************************************************************************/

#pragma pack(push)
#pragma pack(1)

/* Extended FAT12/16 boot sector. Start from offset 36. */
struct fat16_bs_ex
{
    /* Number of physical disk. For example: 0: floppy disk, 0x80: hard disk. */
    BYTE drv_num;
    /* Not used. */
    BYTE reserved1;
    /* Extended boot signature (0x29). */
    BYTE boot_sig;
    /* Volume serial number. */
    DWORD serial;
    /* Volume label. */
    BYTE label[11];
    /* File system type. */
    BYTE fs_type[8];
};

/* Extended FAT32 boot sector. Start from offset 36 */
struct fat32_bs_ex
{
    /* FAT32 32-bit count of sectors occupied by ONE FAT */
    DWORD fat32_sz;
    /*
     * Bits  0-3: Zero-based number of active FAT. Only valid if mirroring is
     * disable.
     * Bits  4-6: Reserved.
     * Bit     7: 0 means the FAT is mirrored at runtime into all FATs.
     *          : 1 means only one FAT is active; it is the one referenced in
     *            bits 0-3.
     * Bits 8-15: Reserved.
     */
    WORD ext_flags;
    /* High byte is major revision number. Low byte is minor revision number.
     * This is version number of the FAT32 volume.
     */
    WORD ver;
    /* Root's first cluster. */
    DWORD root_clus_num;
    /* The sector number contains fs_info. */
    WORD fs_info_sec_num;
    /* The sector number contains backup of boot sector. */
    WORD bk_boot_sec_num;
    /* Not used. */
    BYTE reserved[12];
    /* Driver number. Example: 0: floppy disk, 0x80: hard disk. */
    BYTE drv_num;
    /* Not used. */
    BYTE reserved1;
    /* Extended boot signature (0x29). */
    BYTE boot_sig;
    /* Volume serial number, or volume ID. */
    DWORD serial;
    /* Volume label. */
    BYTE label[11];
    /* File system type. */
    BYTE fs_type[8];
};

/*!
 * @brief Structure of Boot Sector.
 */
struct fat_bs
{
    /* Jump instruction to boot code. */
    BYTE jmp_boot[3];
    /* Name of the system formatted the volume. */
    BYTE oem_name[8];
    /* Count of bytes per sector. */
    WORD sec_sz;
    /* Number of sectors per cluster. */
    BYTE clus_sz;
    /* Number of reserved sectors in the Reserved region of the volume starting
     * at the first sector of the volume. */
    WORD rsvd_sec_cnt;
    /* The count of FAT tables. */
    BYTE fat_cnt;
    /* FAT12/16: The count of 32-byte directory entries in the root directory.
       FAT32: This field must be set to 0. */
    WORD root_ent_cnt;
    /* FAT12/16: Total sectors of this volume.
       FAT32: This field must be set to 0. */
    WORD tot_sec_16;
    /* Indicate type of the media (non-removable or removable) */
    BYTE media_type;
    /* FAT12/16: The 16-bit count of sectors occupied by a FAT12/16 table.
       In FAT32: This field must be 0. */
    WORD fat16_sz;
    /* Sectors per track. */
    WORD trk_sz;
    /* Number of heads. */
    WORD num_heads;
    DWORD hidden_sectors;
    /* This field is the new 32-bit total count of sectors on the volume
       FAT12/16: This field contains the sector count if tot_sec_16 == 0.
       FAT32: This field must be non-zero. */
    DWORD tot_sec_32;

    /* Fields start at offset 0x24 */
    union {
        struct fat16_bs_ex fat16_bs_ex;
        struct fat32_bs_ex fat32_bs_ex;
    };

    /* Not used in this program. */
    BYTE not_used[420];

    /* System signature. */
    BYTE sys_sig[2];
};

/* FAT File Record. This represent a file or folder in FAT file system.
 * Microsoft's FAT documentations call this FAT Directory Structure, but I
 * think call this is file record is more understandable and comfortable.
 */
struct fat_frec
{
    /* Short name */
    BYTE name[8];
    /* Extension. For example: exe, bat, txt, doc, ... */
    BYTE ext[3];
    /* File's properties. */
    BYTE attrs;
    /* Not used. */
    BYTE nt_reserved;
    /* Timestamp when created file. */
    BYTE created_timestamp;
    /* Time file was created. */
    WORD created_time;
    /* Date file was created. */
    WORD created_date;
    /* Last read or write file date. */
    WORD last_access_date;
    /* High word of this entry's first cluster number (always 0 for a FAT12 or
     * FAT16 volume).
     */
    WORD first_clus_hi;
    /* Time of last write. */
    WORD modified_time;
    /* Date of last write. */
    WORD modified_date;
    /* Low word of this entry's first cluster number. */
    WORD first_clus_lo;
    /* File size in bytes. */
    DWORD file_sz;
};

/*!
 * @brief FAT Long Directory Structure.
 */
struct fat_lfrec
{
    BYTE ord;
    BYTE name1[10];
    BYTE attrs;
    BYTE type;
    BYTE chk_sum;
    BYTE name[12];
};

#pragma pack(pop)

/*!
 * @brief Represent FAT file system. Use this to communicate with the disk.
 */
struct fat_fs
{
    /* FAT's boot sector. */
    struct fat_bs bs;

    /* One of:
     * - FS_UNKNOWN (0x00)
     * - FS_FAT12   (0x0C)
     * - FS_FAT16   (0x10)
     * - FS_FAT32   (0x40)
     */
    BYTE fs_type;
    /* Name of the file system. */
    BYTE *fs_name;

    /* Total numbers of sectors of the partition. */
    DWORD total_sec_cnt;
    /* Total numbers of clusters (starting at cluster 2) of the partition. */
    DWORD total_clus_cnt;

    /* First sector of FAT Table. */
    DWORD fat_sec_num;
    /* Size of each FAT table (sectors). */
    DWORD fat_sz;

    /* The sector number of the first sector of root directory. */
    DWORD root_dir_sec_num;
    /* Root Directory's size (sectors). It's only meaning in FAT12/16. In FAT32,
     * Root Directory's, like any other directories, is not fixed. Thus, in
     * FAT32, root_dir_sz = 0.
     */
    DWORD root_dir_sz;

    /* The sector number of the first sector of data region. */
    DWORD data_sec_num;
    /* Data region's size (sectors). */
    DWORD data_sz;
};

typedef struct fat_bs fat_bs_t;
typedef struct fat_fs fat_fs_t;
typedef struct fat_frec fat_frec_t;

/*******************************************************************************
 * APIs
 ******************************************************************************/

/*!
 * @brief Open a FAT12/16 file system. You should use function fat_check_fs to
 * check if the file system is FAT12/16 file system before calling this.
 *
 * @param path [in] Path of the device. For example: "/dev/sdb1" (Linux),
 * "\\\\.\\A:" (Windows), ...
 * @param fs [out] Point to FAT12/16 file system data structure.
 *
 * @return Return error code. Read Error codes section for more information.
 */
int32_t fat_open_fs(char *path, fat_fs_t *fs);

/*!
 * @brief Close an open FAT12/16 file system.
 *
 * @param fs Point to an open FAT12/16 file system.
 *
 * @return Return error code. Read Error codes section for more information.
 */
int32_t fat_close_fs(fat_fs_t *fs);

/*!
 * @brief List all files and directories inside a folder.
 *
 * @param fs [in] File system structure.
 * @param sec_num [in] Sector number of first sector of the folder.
 * @param records [in,out] Stores listed files and directories.
 * @param max [in] Max of the number of files and directories the caller could
 * retrieved.
 * @param total [out] The number of listed files and directories.
 *
 * @return Return error code. Refer Error codes section.
 */
int32_t fat_read_folder(fat_fs_t *fs, DWORD sec_num, fat_frec_t *records,
                        int32_t max, int32_t *total);

/*!
 * @brief Read and copy all file data to buffer.
 *
 * @param fs [in] File system structure.
 * @param record [in] Record of read file.
 * @param buff [out] Store file data. This function treats that buff is wide
 * enough to store all file data (due to simplyness of implementation).
 *
 * @return Return error code. Refer Error codes section.
 */
int32_t fat_read_file(fat_fs_t *fs, fat_frec_t *record, uint8_t *buff);

#endif /* _FAT_H_ */
