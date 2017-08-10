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

    printf("Created\t\t\tLast modified\t\tSize\tName\n");

    for (i = 0; i < total; ++i)
    {
        util_print_file_record(&records[i]);
    }

    return rc;
}

int32_t app_cmd_help()
{
    printf("Available commands:\n");
    printf("help\t-\tPrint this message\n");
    printf("fsinfo\t-\tDisplay file system information\n");
    printf("cwd\t-\tGet current working directory\n");
    printf("ls\t-\tList all files and directories in a folder\n");
    printf("exit\t-\tExit program\n");

    return APP_ERROR_NONE;
}

int32_t app_cmd_exit()
{
    return APP_ERROR_EXIT;
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
    DWORD cwdOff;

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
            rc = app_cmd_exit();
        }
        else if (strcmp(cmd, "fsinfo") == 0)
        {
            rc = app_cmd_fsinfo(&fs);
        }
        else if(strcmp(cmd, "ls") == 0)
        {
            rc = app_cmd_ls(&fs, fs.root_dir_off);
        }
        else
        {
            printf("Invalid command!\n");
        }
    }

    printf("Error code: 0x%X", rc);

    fat16_close_fs(&fs);

    return 0;
}
