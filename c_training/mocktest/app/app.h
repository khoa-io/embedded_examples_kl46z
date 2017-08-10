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

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Max length of each commands is 20. Byte 21 for '\0' */
#define MAX_CMD_LEN 21

/*******************************************************************************
 * Error codes
 ******************************************************************************/
/* No error */
#define APP_ERROR_NONE 0x0
/* No error. normal exit */
#define APP_ERROR_EXIT 0x1
/* Unknown error */
#define APP_ERROR_UNKNOWN 0xFFFFFFFF

/*******************************************************************************
 * APIs
 ******************************************************************************/

/*!
 * @brief Command "help": print help message.
 *
 * @return Return APP_ERROR_NONE;
 */
int32_t app_cmd_help();

/*!
 * @brief Command "fsinfo": read and display file system's information.
 *
 * @param fs Point to initialized FAT file system structure.
 *
 * @param Return error code. Refer Error codes section.
 */
int32_t app_cmd_fsinfo(fat16_fs_t *fs);

/*!
 * @brief Command "ls": list all files and directories inside a folder and
 * print result to stdout.
 *
 * @param fs [in] File system structure.
 * @param off [in] First sector of the folder.
 *
 * @return Return error code. Refer Error codes section.
 */
int32_t app_cmd_ls(fat16_fs_t *fs, DWORD off);

/*!
 * @brief Command "cd": change working directory to a sub or parent directory
 * (with one level due to simplyness of implementation).
 *
 * @param fs [in] File system structure.
 * @param cwd [in,out] Current working directory's record.
 * @param cwd_off [in,out] Current working directory's first sector.
 *
 * @return Return error code. Refer Error codes section.
 */
int32_t app_cmd_cd(fat16_fs_t *fs, fat_file_record_t *cwd, DWORD *cwd_off);

/*!
 * @brief Command "cat": read data of a file in current working directory and
 * print it to stdout.
 *
 * @param fs [in] File system structure.
 *
 * @return Return error code. Refer Error codes section.
 */
int32_t app_cmd_cat(fat16_fs_t *fs, fat_file_record_t *cwd, DWORD *cwd_off);

#endif /* _APP_H_ */
