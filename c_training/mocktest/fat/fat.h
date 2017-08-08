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
 * Definitions
 ******************************************************************************/

/*!
 * @brief First part of Boot Sector which both FAT12/16 and FAT32 share. Start
 * at offset 0. Fields have name starts with "BS_" are part of the boot sector
 * and not really part of BPB (BIOS Parameter Block). Fields have name starts
 * with "BPB_" are part of BPB. The differences starts at offset 36.
 */
#define BOOT_SECTOR_GENERAL BYTE BS_jmpBoot[3]; \
                            BYTE BS_oemName[8]; \
                            WORD BPB_bytsPerSec; \
                            BYTE BPB_secPerClus; \
                            WORD BPB_rsvdSecCnt; \
                            BYTE BPB_numFATs; \
                            WORD BPB_rootEntCnt; \
                            WORD BPB_totSec16; \
                            BYTE BPB_media; \
                            WORD BPB_fatSz16; \
                            WORD BPB_SecPerTrk; \
                            WORD BPB_numHeads; \
                            DWORD BPB_hiddSec; \
                            DWORD BPB_totSec32; \

/*!
 * @brief FAT12/16's Boot Sector
 */
struct boot_sector_fat16
{
    BOOT_SECTOR_GENERAL

    BYTE BS_drvNum;
    BYTE BS_reserved1;
    BYTE BS_bootSig;
    DWORD BS_volID;
    BYTE BS_volLab[11];
    BYTE BS_filSysType[8];
};

struct boot_sector_fat32
{
    BOOT_SECTOR_GENERAL
};

typedef struct boot_sector_fat16 boot_sector_fat16_t;

#endif /* _FAT_H_ */
