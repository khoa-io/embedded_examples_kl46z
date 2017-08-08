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

#include "hal/haltypes.h"
#include "fat/fat.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

void f(const char *path)
{

}

int main(int argc, char *argv[])
{

    FILE *fp;

    uint32_t sector[512] = {0};

    uint32_t i = 0;
    uint32_t c = 0;

    if (argc < 2)
    {
        printf("Usage: mocktest /path/to/image/file\n");
        return 0;
    }

    fp = fopen(argv[1], "rb");
    if (fp == NULL)
    {
        return 0;
    }

    for (i = 0; i < 512; ++i)
    {
        fscanf(fp, "%u", &c);
        sector[i] = c;
    }

    for (i = 0; i < 512; ++i)
    {
        printf(" 0x%X", sector[i]);
    }

    fclose(fp);

    return 0;
}
