#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

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
    int32_t j = 0;

    /* Buffers and temporary variables */
    int8_t buff[MAX_RECORD_SIZE + 1] = { 0 };
    int8_t c = 0;

    /* Used for reading filehandling.srec */
    FILE *fp;

    parse_data_struct_t parsedData;

    if (argc < 2)
    {
        printf("Usage: assignment5.exe path_to_filehandling.srec\n");
    }

    fp = fopen(argv[1], "r");

    /* Read each line and copy to buff */
    while ((c = fgetc(fp)) != EOF)
    {
        for (i = 0; c != '\r' && c != '\n' && i < MAX_RECORD_SIZE; ++i, c = fgetc(fp))
        {
            buff[i] = c;
        }

        /* Using buff */
        buff[i] = '\n';
        printf("%d: %s", ++j, buff);
        parseData(buff, &parsedData);

        memset(buff, 0, MAX_RECORD_SIZE);
    }

    fclose(fp);

    return 0;
}