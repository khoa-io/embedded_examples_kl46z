#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "srec_reader.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(int argc, char *argv[])
{
    /* Indexing variables */
    int32_t i = 0;
    /* Line number */
    int32_t j = 0;

    /* Buffers and temporary variables */
    int8_t buff[MAX_RECORD_SIZE + 1] = {0};
    int8_t c = 0;

    /* Used for reading filehandling.srec */
    FILE *fp;

    parse_data_struct_t parsedData;
    parse_status_t status = e_parseStatus_undefined;

    if (argc < 2)
    {
        printf("Usage: assignment5.exe path_to_filehandling.srec\n");
    }

    fp = fopen(argv[1], "r");

    /* Read each line and copy to buff */
    for (j = 1; (c = fgetc(fp)) != EOF; ++j)
    {
        /* Set all bytes to 0 */
        memset(buff, 0, MAX_RECORD_SIZE);
        memset(&parsedData, 0, sizeof(parse_data_struct_t));

        for (i = 0; c != '\n' && i < MAX_RECORD_SIZE; ++i, c = fgetc(fp))
        {
            buff[i] = c;
        }

        status = parseData(buff, &parsedData);

        if (status == e_parseStatus_error)
        {
            continue;
        }

        printf("Line number: %05d. Converted address: 0x%08x. Converted data: [",
               j, parsedData.address);
        for (i = 0; i < parsedData.dataLength; ++i)
        {
            printf(" 0x%02x", parsedData.data[i]);
        }
        printf("]\n");
    }

    fclose(fp);

    return 0;
}
