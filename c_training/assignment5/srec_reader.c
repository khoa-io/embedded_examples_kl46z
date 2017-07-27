#include <stdint.h>
#include <stddef.h>

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
    /* Return value */
    parse_status_t status = e_parseStatus_undefined;

    /* Indexing variables */
    int32_t i = 0;
    int32_t j = 0;

    /* Temporary variable */
    uint32_t tmp = 0;

    /* Temporary pointer */
    uint8_t *pTmp = NULL;

    /* Count of srec record converted from hex char */
    /* count = address + data + checksum. All in this expression is size */
    uint8_t count = (hex2val(pInput[2]) << 4) + hex2val(pInput[3]);

    /* Size in bytes of address field of srec record converted from hex char */
    uint32_t addrSize = 0;

    /* Type of srec record converted from hex char */
    uint8_t srecType = hex2val(pInput[1]);

    uint8_t checksum = 0;

    /* Calculated checksum from real value */
    uint32_t checkValue = 0;

    switch (srecType)
    {
    case 0:
        status = e_parseStatus_inprogress;

        pOutput->dataLength = count - 3;

        /* i: indexing on pInput */
        /* j: indexing on pOutput->data */
        for (i = 8, j = 0; j < pOutput->dataLength; i += 2, ++j)
        {
            pOutput->data[j] = (hex2val(pInput[i]) << 4) + hex2val(pInput[i + 1]);
        }

        checksum = (hex2val(pInput[i]) << 4) + hex2val(pInput[i + 1]);

        break;

    case 1:
    case 2:
    case 3:
        status = e_parseStatus_inprogress;

        addrSize = srecType + 1;

        /* Access an uint32_t variable like an uint8_t array using an uint8_t
        pointer. Note that we're on Intel x86_64 (little endian) */
        pTmp = (uint8_t *)&pOutput->address;

        /* Decode and copy address from file to pOutput->address follow little
        endian order */
        for (i = 4, j = addrSize - 1; j >= 0; --j)
        {
            pTmp[j] = (hex2val(pInput[i++]) << 4) + hex2val(pInput[i++]);
        }

        pOutput->dataLength = count - addrSize - 1;

        /* Decode and copy data from file to pOutput->data */
        for (j = 0; j < pOutput->dataLength; ++j)
        {
            pOutput->data[j] = (hex2val(pInput[i++]) << 4) + hex2val(pInput[i++]);
        }

        checksum = (hex2val(pInput[i]) << 4) + hex2val(pInput[i + 1]);

        break;

    /*     case 4:
    case 5:
    case 6:
        status = e_parseStatus_inprogress;
        break; */

    case 7:
        addrSize = 4;
    case 8:
        addrSize = 3;
    case 9:
        addrSize = 2;

        status = e_parseStatus_done;

        /* Access an uint32_t variable like an uint8_t array using an uint8_t
        pointer. Note that we're on Intel x86_64 (little endian) */
        pTmp = (uint8_t *)&pOutput->address;

        /* Decode and copy address from file to pOutput->address follow little
        endian order */
        for (i = 4, j = addrSize - 1; j >= 0; i += 2, --j)
        {
            pTmp[j] = (hex2val(pInput[i]) << 4) + hex2val(pInput[i + 1]);
        }

        pOutput->dataLength = 0;

        checksum = (hex2val(pInput[i]) << 4) + hex2val(pInput[i + 1]);

        break;
    }

    /* Do check valid checksum */

    checkValue = count;

    /* Access pOutput like an array of uint8_t. Bit order is not important */
    pTmp = (uint8_t *)pOutput;
    /* Ignore last byte because it is dataLength */
    for (i = 0; i < pOutput->dataLength + 4; ++i)
    {
        checkValue += pTmp[i];
    }

    checkValue = ~checkValue & 0xFF;

    if (checkValue != checksum)
    {
        status = e_parseStatus_error;
    }

    return status;
}
