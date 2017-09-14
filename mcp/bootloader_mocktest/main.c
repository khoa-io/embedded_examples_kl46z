/*
 * This is template for main module created by KSDK Project Generator. Enjoy!
 */

/*
 * [File Name]     main.c
 * [Platform]      MKL46Z256VLH4
 * [Project]       bootloader_mocktest
 * [Version]       1.00
 * [Author]        KhoaHV1
 * [Date]          28-08-2017
 * [Language]      'C'
 * [History]       1.00 - Original Release
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "MKL46Z4.h"
#include "board.h"
#include "gpio.h"
#include "port.h"
#include "uart.h"
#include "queue.h"
#include "srec_reader.h"
#include "loader.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BAUD_RATE (4800U)

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Main loop.
 */
void mainLoop(void);

/*!
 * @brief Parse SREC line and write to flash if there is no error.
 *
 * @param line Buffer stores SREC line.
 */
void checkAndWrite(uint8_t *line);

/*!
 * @brief Initialize environment.
 */
void init(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

void checkAndWrite(uint8_t *line)
{
    /* Store e_parseStatus_start and e_parseStatus_done */
    static parse_status_t savedStatus = e_parseStatus_undefined;
    /* `err == true` if there is an error */
    static bool err = false;

    parsed_dat_t parsedData;
    parse_status_t status = e_parseStatus_undefined;

    status = parseData(line, &parsedData);

    switch (status)
    {
    case e_parseStatus_error:
    case e_parseStatus_undefined:
        err = true;
        UART_sendArray(UART_0, (uint8_t *)"Parsed error!\r\n", 15);
        break;

    case e_parseStatus_start:
        /* LOADER_preload(); */
        break;

    case e_parseStatus_inprogress:
        if (LOADER_write(&parsedData) != parsedData.dataLength)
        {
            err = true;
            UART_sendArray(UART_0, (uint8_t *)"Cannot write normally to flash!\r\n", 33);
        }
        UART_sendArray(UART_0, (uint8_t *)"Received.\r\n", 11);
        break;

    case e_parseStatus_done:
        if (err)
        {
            UART_sendArray(UART_0, (uint8_t *)"Cannot run the app!\r\n", 21);
            break;
        }

        UART_sendArray(UART_0, (uint8_t *)"Bootloader will stop to run the app!\r\n", 38);
        LOADER_runApp();
        break;
    }

    savedStatus = status;
}

void mainLoop(void)
{
    /* Buffers and temporary variables */
    static uint8_t buff[MAX_RECORD_SIZE] = { 0 };
    /* Indexing on `buff` */
    static uint16_t i = 0;

    /* Pop from bottom of queue */
    queue_item_t *bot = NULL;
    /* Indexing on `bot->dat` */
    uint8_t j = 0;

    /* Error code */
    uint32_t rc = QUEUE_ERR_NONE;

    if (QUEUE_isEmpty())
    {
        /* Data is not ready => abort */
        return;
    }

    /* Data is ready! */

    /* Get new item from the bottom of the queue */
    rc = QUEUE_bot(&bot);
    if (rc != QUEUE_ERR_NONE)
    {
        /* Cannot get new item => abort */
        UART_sendArray(UART_0, (uint8_t *)"[main.c]QueueError\r\n", 20);
        return;
    }

    /* Copy data from `bot->dat` to `buff` because a SREC line can lie on one
     * or more items and/or an item can contain 2 parts of a SREC line. */
    for (j = 0; j < bot->sz; ++j)
    {
        buff[i++] = bot->dat[j];

        if (bot->dat[j] == '\n')
        {
            /* TODO */
            /* UART_sendArray(UART_0, buff, i); */
            checkAndWrite(buff);
            i = 0;
            continue;
        }
    }

    QUEUE_pop();
}

void init(void)
{
    /* Store configuration of UART0 for configuring UART0 */
    uart_conf_t uartConf;

    /* Configure UART0 */
    uartConf.type = UART_TYPE_TRANSMITTER_MASK | UART_TYPE_RECEIVER_MASK;
    uartConf.sz = 8;
    uartConf.parityEnable = false;
    uartConf.msbf = false;
    uartConf.polarity = 0;
    uartConf.baudRate = BAUD_RATE;

    UART_init(UART_0);
    UART_config(UART_0, &uartConf);
}

int main(void)
{
    init();

    while (1)
    {
        mainLoop();
    };

    return 0;
}
