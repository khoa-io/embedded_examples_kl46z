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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "fat/fat.h"
#include "util/util.h"
#include "app/app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFF_SIZE 512

/*******************************************************************************
 * Code
 ******************************************************************************/

int32_t app_cmd_fsinfo(fat16_fs_t *fs)
{
    int8_t buff[BUFF_SIZE] = {0};

    printf("FILE SYSTEM INFORMATION: \n");

    memcpy(buff, &fs->header.fs_type, sizeof(fs->header.fs_type));
    printf("Format: %s\n", buff);
    memset(buff, 0, BUFF_SIZE);

    memcpy(buff, &fs->header.oemName, sizeof(fs->header.oemName));
    printf("OEM Name: %s\n", buff);
    memset(buff, 0, BUFF_SIZE);

    printf("Sector size (bytes): %u\n", fs->header.sector_size);
    printf("Cluster size (sectors): %u\n", fs->header.cluster_size);
    printf("Number of reserved sectors: %u\n",
           fs->header.reserved_sectors_count);

    printf("Number of FAT tables: %u\n", fs->header.fats_count);
    printf("FAT tables start at sector 0x%X\n", fs->fat_off);
    printf("FAT table size (byte): %u\n", fs->fat_size);

    printf("Max number of root directory's entries: %u\n",
           fs->header.root_entries_count);

    printf("Total sectors: %u\n", fs->header.total_sectors);

    if (fs->header.media_type == 0xF8)
    {
        printf("Media: Hard disk\n");
    }
    else if (fs->header.media_type == 0xF0)
    {
        printf("Media: Soft disk 1.44M\n");
    }
    else
    {
        printf("Media code: 0x%X\n", fs->header.media_type);
    }

    if (fs->header.vol_label[0])
    {
        memcpy(buff, &fs->header.vol_label, sizeof(fs->header.vol_label));
        printf("Volume label: %s\n", buff);
        memset(buff, 0, BUFF_SIZE);
    }
    else
    {
        printf("Volume label: NO NAME\n");
    }

    printf("Root directory offset: 0x%X\n", fs->root_dir_off);
    printf("Root directory size (byte): %u\n", fs->root_dir_size);
    printf("Data region offset: 0x%X\n", fs->data_off);

    return APP_ERROR_NONE;
}

int32_t app_cmd_ls(fat16_fs_t *fs, DWORD off)
{
    /* Stores all listed files and sub-folders */
    fat_file_record_t records[BUFF_SIZE];

    /* Return code */
    int32_t rc = FAT_ERROR_NONE;

    /* Indexing variable */
    int32_t i = 0;

    /* The number of listed files and directories */
    int32_t total = 0;

    rc = fat16_readfolder(fs, off, records, BUFF_SIZE, &total);
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

    return rc;
}

int32_t app_cmd_cd(fat16_fs_t *fs,
                   fat_file_record_t *cwd, DWORD *cwd_off)
{
    /* Return code */
    int32_t rc = APP_ERROR_NONE;

    /* Stores all listed files and sub-folders */
    fat_file_record_t records[BUFF_SIZE];

    /* Indexing variable */
    int32_t i = 0;

    /* The number of listed files and directories */
    int32_t total = 0;

    /* Name of new directory. Can be: parent or sub directory name in ONE level */
    int8_t name[BUFF_SIZE] = {0};
    /* Temporary buffer */
    int8_t buff[BUFF_SIZE] = {0};

    printf("New directory: ");
    scanf("%s", name);

    for (i = 0; i < BUFF_SIZE; ++i)
    {
        name[i] = toupper(name[i]);
    }

    rc = fat16_readfolder(fs, *cwd_off, records, BUFF_SIZE, &total);
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
        if (strcmp(buff, name) == 0)
        {
            if (records[i].attrs & ATTR_DIRECTORY == 0)
            {
                printf("Not a directory!\n");
                break;
            }

            *cwd = records[i];
            *cwd_off = ((cwd->first_cluster_lo - 2) * fs->header.cluster_size) +
                       fs->data_off;

            /* Sub-directory is root directory */
            if (cwd->first_cluster_lo == 0)
            {
                *cwd_off = fs->root_dir_off;
            }

            printf("Changed working directory into %s\n", name);
            break;
        }
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
    int8_t buff[BUFF_SIZE] = {0};

    int32_t rc = APP_ERROR_NONE;

    /* Store command user entered */
    int8_t cmd[MAX_CMD_LEN] = {0};

    /* Use this to work with FAT12/16 file system */
    fat16_fs_t fs;

    /* Current working directory. Default is root directory */
    fat_file_record_t cwd;
    /* First sector of current working directory */
    DWORD cwd_off;

    if (argc < 2)
    {
        printf("Usage: mocktest /path/to/image/file\n");
        return 0;
    }

    rc = fat16_open_fs(argv[1], &fs);
    if (rc != FAT_ERROR_NONE)
    {
        printf("Cannot open file system! Error code: %d\n", rc);
        return 0;
    }

    cwd_off = fs.root_dir_off;

    app_cmd_help();
    for (rc = APP_ERROR_NONE; rc == APP_ERROR_NONE;)
    {
        printf("Command: ");
        scanf("%s", cmd);

        if (strcmp(cmd, "help") == 0)
        {
            rc = app_cmd_help();
        }
        else if (strcmp(cmd, "exit") == 0)
        {
            rc = APP_ERROR_NONE;
            break;
        }
        else if (strcmp(cmd, "fsinfo") == 0)
        {
            rc = app_cmd_fsinfo(&fs);
        }
        else if (strcmp(cmd, "ls") == 0)
        {
            rc = app_cmd_ls(&fs, cwd_off);
        }
        else if (strcmp(cmd, "cd") == 0)
        {
            rc = app_cmd_cd(&fs, &cwd, &cwd_off);
        }
        else
        {
            printf("Invalid command!\n");
        }
    }

    if (rc != APP_ERROR_NONE)
    {
        printf("Error code: 0x%X", rc);
    }

    fat16_close_fs(&fs);

    return 0;
}
