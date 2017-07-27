#include <stdint.h>

#include "srec_reader.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
 /* Returned code when pass invalid hexadecimal character */
#define INVALID_HEX_CHAR -1

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
 /*!
  * @brief Get value of a hexadecimal character.
  *
  * @param ch ASCII code of the hexadecimal character.
  *
  * @return Returns the value which the hexadecimal character represents or
  * INVALID_HEX_CHAR.
  */
static uint8_t hex2val(uint8_t ch);

/*******************************************************************************
 * Variables
 ******************************************************************************/

 /*******************************************************************************
  * Code
  ******************************************************************************/

static uint8_t hex2val(uint8_t ch)
{
    uint8_t retVal = INVALID_HEX_CHAR;

    if ('0' <= ch && ch <= '9')
    {
        retVal = ch - '0';
    }

    if ('A' <= ch && ch <= 'F')
    {
        retVal = ch - 'A' + 10;
    }

    return retVal;
}

parse_status_t parseData(uint8_t pInput[], parse_data_struct_t *pOutput)
{
    parse_status_t status = e_parseStatus_undefined;
    /* Indexing variables */
    int32_t i = 0;
    int32_t j = 0;

    uint8_t count = (hex2val(pInput[2]) << 4) + hex2val(pInput[3]);

    uint32_t addrSize = 0;

    uint8_t srecType = hex2val(pInput[1]);

    uint32_t tmp = 0;

    switch (srecType)
    {
    case 0:
        pOutput->address = 0;
        pOutput->dataLength = count - 3;

        /* i: indexing on pInput */
        /* j: indexing on pOutput->data */
        for (i = 8, j = 0; j < pOutput->dataLength; i += 2, ++j)
        {
            pOutput->data[j] = (hex2val(pInput[i]) << 4) + hex2val(pInput[i + 1]);
        }

        /* for debugging only */
        /* pOutput->data is null-terminated string */
        /* pOutput->data[j] = 0; */

        status = e_parseStatus_inprogress;
        break;

    case 1:
    case 2:
    case 3:
        addrSize = srecType + 1;
        pOutput->address = 0;

        for (i = 0, j = 4; i < addrSize; ++i, j += 2)
        {

        }

        pOutput->dataLength = count - 3;

        break;

    case 4:
    case 5:
    case 6:
        break;;
    }

    return status;
}
