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
#include "app/app.h"

/*******************************************************************************
 * Global variables
 ******************************************************************************/

fat16_fs_t g_fs;

/*******************************************************************************
 * Code
 ******************************************************************************/

int32_t app_cmd_help()
{
    printf("Available commands:\n");
    printf("help\t-\tPrint this message\n");
    printf("fsinfo\t-\tDisplay file system information\n");
    printf("exit\t-\tExit program\n");

    return APP_ERROR_NONE;
}

int32_t app_cmd_exit()
{
    return APP_ERROR_EXIT;
}

int32_t app_prompt()
{
    int32_t rc = APP_ERROR_NONE;
    int8_t cmd[MAX_CMD_LEN] = {0};

    printf("cmd: ");
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
        rc = app_cmd_fsinfo(&g_fs);
    }

    return rc;
}

int main(int argc, char *argv[])
{
    int8_t buff[1024] = {0};

    int32_t ret = 0;

    if (argc < 2)
    {
        printf("Usage: mocktest /path/to/image/file\n");
        return 0;
    }

    ret = fat16_open_fs(argv[1], &g_fs);
    if (ret != FAT_ERROR_NONE)
    {
        printf("Error code: %d\n", ret);
        return 0;
    }

    app_cmd_help();
    for (ret = APP_ERROR_NONE; ret == APP_ERROR_NONE;)
    {
        ret = app_prompt();
    }

    fat16_close_fs(&g_fs);

    return 0;
}
