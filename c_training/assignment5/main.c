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

/*!
 * @brief Print parsed data to standard output if it is available.
 *
 * @param status Show that parsed data is available or not.
 * @param pParsedData Data to print out.
 * @param line Line number.
 */
static void displayData(parse_status_t status, parse_data_struct_t *pParsedData,
                        int32_t line);

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

    /* Used for reading SREC file */
    FILE *fpSrecFile;

    parse_data_struct_t parsedData;
    parse_status_t status = e_parseStatus_undefined;

    if (argc < 2)
    {
        printf("Usage: assignment5.exe path_to_filehandling.srec\n");
    }

    fpSrecFile = fopen(argv[1], "r");

    /* Read each line and copy to buff */
    for (j = 1; (c = fgetc(fpSrecFile)) != EOF; ++j)
    {
        /* Set all bytes to 0 */
        memset(buff, 0, MAX_RECORD_SIZE);
        memset(&parsedData, 0, sizeof(parse_data_struct_t));

        for (i = 0; c != '\n' && i < MAX_RECORD_SIZE; ++i, c = fgetc(fpSrecFile))
        {
            buff[i] = c;
        }

        status = parseData(buff, &parsedData);
        displayData(status, &parsedData, j);
    }

    fclose(fpSrecFile);

    return 0;
}

static void displayData(parse_status_t status, parse_data_struct_t *pParsedData,
                        int32_t line)
{
    int32_t i = 0;

    if (status == e_parseStatus_error || status == e_parseStatus_undefined)
    {
        return;
    }

    printf("Line number: %05d. Converted address: 0x%08x. Converted data: [",
           line, pParsedData->address);
    printf("0x%02x", pParsedData->data[0]);
    for (i = 1; i < pParsedData->dataLength; ++i)
    {
        printf(" 0x%02x", pParsedData->data[i]);
    }
    printf("]\n");
}
