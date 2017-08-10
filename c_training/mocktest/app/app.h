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
 * @brief Command "exit": exit program.
 *
 * @return Return APP_ERROR_EXIT
 */
int32_t app_cmd_exit();

/*!
 * @brief Wait user input a command and then call the command if it exists.
 *
 * @return Return command's return code.
 */
int32_t app_prompt();

/*!
 * @brief Command "fsinfo": read and display file system's information.
 *
 * @param fs Point to initialized FAT file system structure.
 *
 * @param Return error code. Refer Error codes section.
 */
int32_t app_cmd_fsinfo(fat16_fs_t *fs);

#endif /* _APP_H_ */
