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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fat/fat.h"
#include "hal/HAL.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Let default sector size is 512 bytes. So default buffer size is 512 bytes too
 */
#define BUFF_SIZE 512

/*******************************************************************************
 * Formulas
 ******************************************************************************/

/*!
 * @brief Compute index of a FAT entry in the FAT table.
 *
 * @param  n    Cluster number
 * @param  type One of FS_FAT12, FS_FAT16 or FS_FAT32.
 *
 * @return Return index of the entry for the cluster number n.
 */
#define FAT_ENTRY_INDEX(n, type)                                               \
    ((type) == FS_FAT32                                                        \
         ? ((n) << 2)                                                          \
         : ((type) == FS_FAT16 ? ((n) << 1) : ((n) + ((n) >> 2))))

/*!
 * @brief Compute the sector number of the FAT sector that contains the FAT
 * entry.
 *
 * @param  fs Pointer points to a fs_fs structure.
 * @param  i  Index of the FAT entry in FAT table.
 * @return    Return the sector number of the FAT sector that contains the
 * FAT entry.
 */
#define FAT_ENTRY_SEC_NUM(fs, i) ((fs)->bs.rsvd_sec_cnt + (i) / (fs)->bs.sec_sz)

/*!
 * @brief Compute FAT entry's offset from start of the sector contains it.
 *
 * @param  fs Pointer points to a fs_fs structure.
 * @param  i Index of the FAT entry in FAT table.
 *
 * @return   Offset of the entry from start of the secter contains it.
 */
#define FAT_ENTRY_OFF(fs, i) ((i) % (fs)->bs.sec_sz)

/*******************************************************************************
 * Protypes of rivate functions.
 ******************************************************************************/

/*!
 * @brief Get the first sector number of the cluster which the FAT entry points
 * to.
 *
 * @param  fs File system structure.
 * @param  n  Cluster number.
 *
 * @return    Cluster entry value or 0.
 */
static DWORD fat_get_clus_entry_val(fat_fs_t *fs, DWORD n);

/*!
 * @brief Check if a value of a FAT entry means EOF (end of file).
 *
 * @param  fs  Points to file system structure.
 * @param  val FAT entry's value.
 * @return     Return TRUE if FAT entry means EOF, otherwise FALSE.
 */
static bool fat_is_eof(fat_fs_t *fs, DWORD val);

/*******************************************************************************
 * Global variables.
 ******************************************************************************/

/* FAT use this to communicate with disk. */
kmc_device_t *g_disk = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/

static bool fat_is_eof(fat_fs_t *fs, DWORD val)
{
    bool is_eof = false;

    switch (fs->fs_type)
    {
    case FS_FAT12:
        is_eof = val >= 0x0FF8;
        break;

    case FS_FAT16:
        is_eof = val >= 0xFFF8;
        break;

    case FS_FAT32:
        is_eof = val >= 0x0FFFFFF8;
        break;
    }

    return is_eof;
}

static DWORD fat_get_clus_entry_val(fat_fs_t *fs, DWORD n)
{
    /* Entry's index. */
    DWORD i = FAT_ENTRY_INDEX(n, fs->fs_type);
    /* Sector number of the FAT sector. */
    DWORD sec_num = FAT_ENTRY_SEC_NUM(fs, i);
    /* Entry's offset from start of the FAT sector. */
    DWORD off = FAT_ENTRY_OFF(fs, i);
    /* Entry's value. */
    DWORD clus_entry_val = 0;

    /* Store data read from sector contains the FAT entry. */
    BYTE *buff = (BYTE *)malloc(fs->bs.sec_sz);

    /* Read the sector contains the FAT entry. */
    uint32_t rc = g_disk->read_sector(g_disk, sec_num, buff);
    if (rc < fs->bs.sec_sz)
    {
        /* Reading error! */
        free(buff);
        return 0;
    }

    /* Compute entry value for each type of FAT file system. */
    switch (fs->fs_type)
    {
    case FS_FAT12:
        clus_entry_val = *((WORD *)&buff[off]);
        clus_entry_val =
            (n & 0x0001) ? (clus_entry_val >> 4) : (clus_entry_val & 0x0FFF);
        break;

    case FS_FAT16:
        clus_entry_val = *((WORD *)&buff[off]);
        break;

    case FS_FAT32:
        clus_entry_val = (*((DWORD *)&buff[off])) & 0x0FFFFFFF;
        break;
    }

    free(buff);

    return clus_entry_val;
}

int32_t fat_open_fs(char *path, fat_fs_t *fs)
{
    /* Temporary variable to store return code from functions */
    int32_t rc = FAT_ERROR_NONE;

    /* Buffer to read first 512 bytes */
    uint8_t buff[BUFF_SIZE];

    /* Open device. */
    rc = kmc_open(path, &g_disk);
    if (rc || g_disk->status != HAL_STATUS_OPENED)
    {
        /* Cannot open device! */
        rc = FAT_ERROR_CANNOT_OPEN;
        return rc;
    }

    /* Read first 512 bytes then initialize FAT Boot Sector. */
    rc = g_disk->read_sector(g_disk, 0, buff);
    if (rc < BUFF_SIZE)
    {
        /* Cannot read first 512 bytes! */
        rc = FAT_ERROR_CANNOT_OPEN;
        return rc;
    }

    /* Initialize Boot Sector. */
    memcpy(&fs->bs, buff, sizeof(fat_bs_t));

    /* Initialize bytes per sector for futher reading. */
    g_disk->sec_sz = fs->bs.sec_sz;

    /** Initialize FAT file system structure. **/

    if (fs->bs.fat16_sz != 0)
    {
        /* Initialize some fields of FAT12/16 file system */
        fs->fat_sz = fs->bs.fat16_sz;
        fs->total_sec_cnt = fs->bs.tot_sec_16;
        fs->root_dir_sz =
            ((fs->bs.root_ent_cnt * sizeof(fat_frec_t)) + (fs->bs.sec_sz - 1)) /
            fs->bs.sec_sz;
    }
    else
    {
        /* Initialize some fields of FAT32 file system */
        fs->fat_sz = fs->bs.fat32_bs_ex.fat32_sz;
        fs->total_sec_cnt = fs->bs.tot_sec_32;
        fs->root_dir_sz = 0;
    }

    /* Initialize some fields for all types of FAT. */
    fs->fat_sec_num = fs->bs.rsvd_sec_cnt;
    fs->data_sec_num =
        fs->bs.rsvd_sec_cnt + (fs->bs.fat_cnt * fs->fat_sz) + fs->root_dir_sz;
    fs->data_sz = fs->total_sec_cnt - fs->bs.rsvd_sec_cnt -
                  (fs->bs.fat_cnt * fs->fat_sz) - fs->root_dir_sz;

    fs->total_clus_cnt = (DWORD)(fs->data_sz / fs->bs.clus_sz);

    /* Determine file system type base on total numers of clusters. */
    /* Initialize other fields. */
    if (fs->total_clus_cnt < 65525)
    {
        fs->fs_type = FS_FAT16;
        fs->root_dir_sec_num = fs->fat_sec_num + fs->bs.fat_cnt * fs->fat_sz;
        fs->fs_name = fs->bs.fat16_bs_ex.fs_type;
    }

    if (fs->total_clus_cnt < 4085)
    {
        /* Just change type because FAT16 is same as FAT16 almost things. */
        fs->fs_type = FS_FAT12;
    }

    if (fs->total_clus_cnt >= 65525)
    {
        fs->fs_type = FS_FAT32;
        fs->root_dir_sec_num =
            FAT_CLUS_SEC_NUM(fs, fs->bs.fat32_bs_ex.root_clus_num);
        fs->fs_name = fs->bs.fat32_bs_ex.fs_type;
    }

    return FAT_ERROR_NONE;
}

int32_t fat_close_fs(fat_fs_t *fs)
{
    /* Return code */
    int32_t rc = FAT_ERROR_NONE;

    if (g_disk == NULL)
    {
        rc = FAT_ERROR_NOT_OPEN;
        return rc;
    }

    g_disk->close(g_disk);
    g_disk = NULL;

    if (rc != HAL_ERROR_NONE)
    {
        rc = FAT_ERROR_CANNOT_CLOSE;
    }
    else
    {
        rc = FAT_ERROR_NONE;
    }

    return rc;
}

int32_t fat_read_folder(fat_fs_t *fs, DWORD sec_num, fat_frec_t *recs,
                        int32_t max, int32_t *total)
{
    /* Return code */
    int32_t rc = FAT_ERROR_NONE;

    /* Indexing variable */
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;

    /* Used in case of Root Directory lies on more than 1 sector. */
    uint32_t cnt = 0;

    /* Current working record. */
    fat_frec_t rec;
    /* rec as a long file name entry. */
    /* fat_lfrec_t lfrec; */
    /* Current working first long file name entry. */
    /* fat_frec_t rec83; */

    /* Stores data read from disk */
    BYTE *buff = NULL;

    /* Store long file name. */
    DWORD long_name[256] = {0};

    if (g_disk == NULL || g_disk->status != HAL_STATUS_OPENED)
    {
        rc = FAT_ERROR_NOT_OPEN;
        return rc;
    }

    buff = (BYTE *)malloc(fs->bs.sec_sz);

    for (i = 0;; ++i)
    {
        rc = g_disk->read_sector(g_disk, sec_num + k, buff);
        if (rc < g_disk->sec_sz)
        {
            rc = FAT_ERROR_CANNOT_READ;
            return rc;
        }
        memcpy(&rec, buff + (i * sizeof(fat_frec_t)), sizeof(fat_frec_t));

        cnt += sizeof(fat_frec_t);
        /* Check if need to fetch next sector. */
        if (cnt >= fs->bs.sec_sz)
        {
            cnt = 0;
            k++;
        }

        if (rec.name[0] == FAT_DIR_STAT_END)
        {
            /* There is no more entries from this entry. */
            break;
        }

        if (rec.name[0] == FAT_DIR_STAT_FREE)
        {
            continue;
        }

        if (FAT_IS_LONG_FILE(fs, &rec))
        {
            /* Found a long file name entry. */
            /* memcpy(&lfrec, &rec, sizeof(rec)); */
            if (rec.name[0] != 1)
            {
                continue;
            }
            else
            {
                printf("First entry!\n");
            }
        }

        if (rec.attrs & ATTR_VOLUME_ID)
        {
            printf("ATTR_VOLUME_ID\n");
            continue;
        }

        recs[j++] = rec;
    }

    *total = j;
    free(buff);

    rc = FAT_ERROR_NONE;
    return rc;
}

int32_t fat_read_file(fat_fs_t *fs, fat_frec_t *rec, uint8_t *buff)
{
    /* Return code */
    int32_t rc = FAT_ERROR_NONE;

    /* Indexing variable */
    size_t i = 0;

    /* Current working cluster's number */
    DWORD curr = rec->fst_clus_lo;

    /* Current working sector */
    DWORD sec_num = FAT_CLUS_SEC_NUM(fs, curr);

    for (i = 0; !fat_is_eof(fs, curr); ++i)
    {
        rc = g_disk->read_sectors(g_disk, sec_num, fs->bs.clus_sz,
                                  buff + i * fs->bs.clus_sz);

        if (rc == 0)
        {
            rc = FAT_ERROR_CANNOT_READ;
            break;
        }

        rc = FAT_ERROR_NONE;

        curr = fat_get_clus_entry_val(fs, curr);
    }

    return rc;
}
