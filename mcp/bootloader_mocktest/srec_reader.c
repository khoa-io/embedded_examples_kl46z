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

parse_status_t parseData(uint8_t pInput[], parsed_dat_t *pOutput)
{
    /* Return value */
    parse_status_t status = e_parseStatus_undefined;

    /* Indexing variables */
    int16_t i = 0;
    int16_t j = 0;

    /* Size in bytes of address field of srec record converted from hex char */
    uint32_t addrSize = 0;

    /* Count of srec record converted from hex char */
    /* count = address + data + checksum. All in this expression is size */
    uint8_t count = (hex2val(pInput[2]) << 4) + hex2val(pInput[3]);

    /* Type of srec record converted from hex char */
    uint8_t srecType = hex2val(pInput[1]);

    /* Checksum value which will converted from hex char */
    uint8_t checksum = 0;

    /* Calculated checksum from real values */
    uint32_t checkvalue = count;

    /* Check invalid srec record */
    if (pInput[0] != 'S')
    {
        status = e_parseStatus_error;
        return status;
    }

    switch (srecType)
    {
    case 0:
        status = e_parseStatus_start;
        pOutput->address = 0;
        pOutput->dataLength = count - 3;

        /* Jump to data section */
        i = 8;
        break;

    case 1:
    case 2:
    case 3:
        status = e_parseStatus_inprogress;

        addrSize = srecType + 1;

        /* Decode and copy address from file to pOutput->address follow */
        /* little endian order */
        for (i = 4, j = addrSize - 1; j >= 0; i += 2, --j)
        {
            ((uint8_t *)&pOutput->address)[j] = (hex2val(pInput[i]) << 4) + hex2val(pInput[i + 1]);
        }

        pOutput->dataLength = count - addrSize - 1;

        /* Already jump to data section */
        break;

    case 4:
        status = e_parseStatus_undefined;
        pOutput->dataLength = 0;
        break;

    case 5:
    case 6:
        status = e_parseStatus_inprogress;
        pOutput->dataLength = 0;
        break;

    case 7:
    case 8:
    case 9:
        addrSize = count - 1;
        status = e_parseStatus_done;

        /* Decode and copy address from file to pOutput->address follow little
        endian order */
        for (i = 4, j = addrSize - 1; j >= 0; i += 2, --j)
        {
            ((uint8_t *)&pOutput->address)[j] = (hex2val(pInput[i]) << 4) + hex2val(pInput[i + 1]);
        }

        pOutput->dataLength = 0;

        break;

    default:
        status = e_parseStatus_error;
    }

    if (status == e_parseStatus_error)
    {
        return status;
    }

    /* Decode and copy data from file to pOutput->data */
    for (j = 0; j < pOutput->dataLength; ++j, i += 2)
    {
        pOutput->data[j] = (hex2val(pInput[i]) << 4) + hex2val(pInput[i + 1]);
    }

    checksum = (hex2val(pInput[i]) << 4) + hex2val(pInput[i + 1]);

    /* Check valid checksum */
    /* Calculate checksum from parsed_dat_t::address and */
    /* parsed_dat_t::data */
    for (i = 0; i < pOutput->dataLength + 4; ++i)
    {
        checkvalue += ((uint8_t *)pOutput)[i];
    }
    checkvalue = ((uint8_t)~checkvalue) & 0xFF;
    if (checkvalue != checksum)
    {
        status = e_parseStatus_error;
    }

    return status;
}
