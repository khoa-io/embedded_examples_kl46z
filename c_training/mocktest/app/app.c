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

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "fat/fat.h"
#include "util.h"
#include "app/app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFF_SIZE 512

struct
{
    uint8_t media;
    const char *descr;
} mediatytes[] = {
    {0xf0, "5.25\" or 3.5\" HD floppy"},
    {0xf8, "Hard disk"},
    {0xf9, "3,5\" 720k floppy 2s/80tr/9sec or "
           "5.25\" 1.2M floppy 2s/80tr/15sec"},
    {0xfa, "5.25\" 320k floppy 1s/80tr/8sec"},
    {0xfb, "3.5\" 640k floppy 2s/80tr/8sec"},
    {0xfc, "5.25\" 180k floppy 1s/40tr/9sec"},
    {0xfd, "5.25\" 360k floppy 2s/40tr/9sec"},
    {0xfe, "5.25\" 160k floppy 1s/40tr/8sec"},
    {0xff, "5.25\" 320k floppy 2s/40tr/8sec"},
};

/*******************************************************************************
 * Code
 ******************************************************************************/

int32_t app_cmd_fsinfo(fat_fs_t *fs)
{
    /* Temporary buffer. */
    int8_t buff[BUFF_SIZE] = {0};

    /* Indexing variable. */
    uint8_t i = 0;

    printf("FILE SYSTEM INFORMATION: \n");

    memcpy(buff, fs->fs_name, sizeof(fs->bs.fat16_bs_ex.fs_type));
    printf("Format: %s\n", buff);
    memset(buff, 0, BUFF_SIZE);

    memcpy(buff, &fs->bs.oem_name, sizeof(fs->bs.oem_name));
    printf("OEM Name: %s\n", buff);
    memset(buff, 0, BUFF_SIZE);

    printf("Sector size (bytes): %u\n", fs->bs.sec_sz);
    printf("Cluster size (sectors): %u\n", fs->bs.clus_sz);
    printf("Number of reserved sectors: %u\n", fs->bs.rsvd_sec_cnt);

    printf("Number of FAT tables: %u\n", fs->bs.fat_cnt);
    printf("FAT tables start at sector 0x%X\n", fs->fat_sec_num);
    printf("FAT table size (sector): %u\n", fs->fat_sz);

    printf("Max number of root directory's entries: %u\n", fs->bs.root_ent_cnt);

    printf("Total of sectors: %u\n", fs->total_sec_cnt);
    printf("Total of clusters: %u\n", fs->total_clus_cnt);

    for (i = 0; i < sizeof(mediatytes) / sizeof(*mediatytes); ++i)
    {
        if (fs->bs.media_type == mediatytes[i].media)
        {
            printf("Media type: %s\n", mediatytes[i].descr);
        }
    }

    if (fs->bs.fat16_bs_ex.label[0])
    {
        memcpy(buff, &fs->bs.fat16_bs_ex.label,
               sizeof(fs->bs.fat16_bs_ex.label));
        printf("Volume label: %s\n", buff);
        memset(buff, 0, BUFF_SIZE);
    }
    else
    {
        printf("Volume label: NO NAME\n");
    }

    printf("Root directory offset: 0x%X\n", fs->root_dir_sec_num);
    printf("Root directory size (sectors): %u\n", fs->root_dir_sz);
    printf("Data region offset: 0x%X\n", fs->data_sec_num);
    printf("Data region size (sectors): 0x%X\n", fs->data_sz);

    return APP_ERROR_NONE;
}

int32_t app_cmd_ls(fat_fs_t *fs, DWORD off)
{
    /* Stores all listed files and sub-folders */
    fat_frec_t records[BUFF_SIZE];

    /* Return code */
    int32_t rc = FAT_ERROR_NONE;

    /* Indexing variable */
    int32_t i = 0;

    /* The number of listed files and directories */
    int32_t total = 0;

    rc = fat_read_folder(fs, off, records, BUFF_SIZE, &total);
    if (rc != FAT_ERROR_NONE)
    {
        printf("Cannot list folder!\n");
        rc = APP_ERROR_UNKNOWN;
        return rc;
    }

    printf("Created\t\t\tLast modified\t\tSize\t\tName\n");

    for (i = 0; i < total; ++i)
    {
        util_print_file_record(&records[i]);
    }

    printf("Total %d\n", total);

    return rc;
}

int32_t app_cmd_cd(fat_fs_t *fs, fat_frec_t *cwd, DWORD *cwd_sec_num)
{
    /* Return code */
    int32_t rc = APP_ERROR_NONE;

    /* Stores all listed files and sub-folders */
    fat_frec_t records[BUFF_SIZE];

    /* Indexing variable */
    int32_t i = 0;

    /* The number of listed files and directories */
    int32_t total = 0;

    /* Name of new directory. Can be: parent or sub directory name in ONE level
     */
    char name[BUFF_SIZE] = {0};
    /* Temporary buffer */
    int8_t buff[BUFF_SIZE] = {0};

    /* Show if change working directory success or failed */
    bool changed = false;

    printf("New directory: ");
    scanf("%s", name);

    for (i = 0; i < BUFF_SIZE; ++i)
    {
        name[i] = toupper(name[i]);
    }

    rc = fat_read_folder(fs, *cwd_sec_num, records, BUFF_SIZE, &total);
    if (rc != FAT_ERROR_NONE)
    {
        printf("Cannot list folder!\n");
        rc = APP_ERROR_UNKNOWN;
        return rc;
    }

    for (i = 0; i < total; ++i)
    {
        memset(buff, 0, BUFF_SIZE);
        util_get_file_name(&records[i], buff);
        if (strcmp((const char *)buff, (const char *)name) == 0)
        {
            if ((records[i].attrs & ATTR_DIRECTORY) == 0)
            {
                printf("Not a folder!\n");
                break;
            }

            *cwd = records[i];
            *cwd_sec_num = FAT_CLUS_SEC_NUM(fs, cwd->fst_clus_lo);

            /* Sub-directory is root directory */
            if (cwd->fst_clus_lo == 0)
            {
                *cwd_sec_num = fs->root_dir_sec_num;
            }

            changed = true;
            printf("Changed working directory into %s\n", name);
            break;
        }
    }

    if (!changed)
    {
        printf("Cannot change working directory into %s\n", name);
    }

    return rc;
}

int32_t app_cmd_cat(fat_fs_t *fs, fat_frec_t *cwd, DWORD *cwd_sec_num)
{

    /* Return code */
    int32_t rc = APP_ERROR_NONE;

    /* Stores all listed files and sub-folders */
    fat_frec_t records[BUFF_SIZE];

    /* Indexing variable */
    int32_t i = 0;

    /* The number of listed files and directories */
    int32_t total = 0;

    bool found = false;

    /* Name of new directory. Can be: parent or sub directory name in ONE level
     */
    int8_t name[BUFF_SIZE] = {0};
    /* Temporary buffer */
    int8_t buff[BUFF_SIZE] = {0};

    printf("File name: ");
    scanf("%s", name);

    for (i = 0; i < BUFF_SIZE; ++i)
    {
        name[i] = toupper(name[i]);
    }

    rc = fat_read_folder(fs, *cwd_sec_num, records, BUFF_SIZE, &total);
    if (rc != FAT_ERROR_NONE)
    {
        printf("Cannot list folder!\n");
        rc = APP_ERROR_UNKNOWN;
        return rc;
    }

    for (i = 0; i < total; ++i)
    {
        memset(buff, 0, BUFF_SIZE);
        util_get_file_name(&records[i], buff);

        if (strcmp((const char *)buff, (const char *)name) == 0)
        {
            /* Found the file with given name! */
            found = true;
            if (records[i].attrs & ATTR_DIRECTORY)
            {
                /* User entered a folder name => abort */
                printf("%s is a directory!\n", name);
                break;
            }

            rc = fat_read_file(fs, &records[i], (uint8_t *)buff);
            printf("%s\n", buff);

            break;
        }
    }

    if (!found)
    {
        printf("File not found!\n");
    }

    return rc;
}

int32_t app_cmd_help()
{
    printf("Available commands:\n");
    printf("help\t-\tPrint this message\n");
    printf("fsinfo\t-\tDisplay file system information\n");
    printf("cwd\t-\tGet current working directory\n");
    printf("cd\t-\tChange working directory\n");
    printf("ls\t-\tList all files and directories in a folder\n");
    printf("exit\t-\tExit program\n");

    return APP_ERROR_NONE;
}

int main(int argc, char *argv[])
{
    int32_t rc = APP_ERROR_NONE;

    /* Store command user entered */
    char cmd[MAX_CMD_LEN] = {0};

    /* Use this to work with FAT12/16 file system */
    fat_fs_t fs;

    /* Current working directory. Default is root directory */
    fat_frec_t cwd;
    /* First sector of current working directory */
    DWORD cwd_sec_num;

    if (argc < 2)
    {
        printf("Usage: mocktest /path/to/image/file\n");
        return 0;
    }

    rc = fat_open_fs(argv[1], &fs);
    if (rc != FAT_ERROR_NONE)
    {
        printf("Cannot open file system! Error code: %d\n", rc);
        return 0;
    }

    cwd_sec_num = fs.root_dir_sec_num;

    app_cmd_help();
    for (rc = APP_ERROR_NONE; rc == APP_ERROR_NONE;)
    {
        printf("Command: ");
        scanf("%s", cmd);

        if (strcmp((const char *)cmd, "help") == 0)
        {
            rc = app_cmd_help();
        }
        else if (strcmp((const char *)cmd, "exit") == 0)
        {
            rc = APP_ERROR_NONE;
            break;
        }
        else if (strcmp((const char *)cmd, "fsinfo") == 0)
        {
            rc = app_cmd_fsinfo(&fs);
        }
        else if (strcmp((const char *)cmd, "ls") == 0)
        {
            rc = app_cmd_ls(&fs, cwd_sec_num);
        }
        else if (strcmp((const char *)cmd, "cd") == 0)
        {
            rc = app_cmd_cd(&fs, &cwd, &cwd_sec_num);
        }
        else if (strcmp((const char *)cmd, "cat") == 0)
        {
            rc = app_cmd_cat(&fs, &cwd, &cwd_sec_num);
        }
        else
        {
            printf("Invalid command!\n");
        }
    }

    if (rc != APP_ERROR_NONE)
    {
        printf("Error code: 0x%X\n", rc);
    }

    fat_close_fs(&fs);

    return 0;
}
