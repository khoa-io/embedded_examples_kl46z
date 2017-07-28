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

    /* Temporary pointer point to a byte array */
    uint8_t *pByte = NULL;

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
        status = e_parseStatus_inprogress;
        pOutput->dataLength = count - 3;

        /* Jump to data section */
        i = 8;
        break;

    case 1:
    case 2:
    case 3:
        status = e_parseStatus_inprogress;

        addrSize = srecType + 1;

        /* Access an uint32_t variable like an uint8_t array using an uint8_t
        pointer. Note that we're on Intel x86_64 (little endian) */
        pByte = (uint8_t *)&pOutput->address;

        /* Decode and copy address from file to pOutput->address follow */
        /* little endian order */
        for (i = 4, j = addrSize - 1; j >= 0; i += 2, --j)
        {
            pByte[j] = (hex2val(pInput[i]) << 4) + hex2val(pInput[i + 1]);
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

        /* Access an uint32_t variable like an uint8_t array using an uint8_t */
        /* pointer. Note that we're on Intel x86_64 (little endian) */
        pByte = (uint8_t *)&pOutput->address;

        /* Decode and copy address from file to pOutput->address follow little
        endian order */
        for (i = 4, j = addrSize - 1; j >= 0; i += 2, --j)
        {
            pByte[j] = (hex2val(pInput[i]) << 4) + hex2val(pInput[i + 1]);
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
    /* Access pOutput like an array of uint8_t. Bit order is not important */
    pByte = (uint8_t *)pOutput;
    /* Calculate checksum from parse_data_struct_t::address and */
    /* parse_data_struct_t::data */
    for (i = 0; i < pOutput->dataLength + 4; ++i)
    {
        checkvalue += pByte[i];
    }
    checkvalue = ~checkvalue & 0xFF;
    if (checkvalue != checksum)
    {
        status = e_parseStatus_error;
    }

    return status;
}
