/*
 * This is template for main module created by KSDK Project Generator. Enjoy!
 */

/*
 * [File Name]     main.c
 * [Platform]      MKL46Z256VLH4
 * [Project]       uart_assignment
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

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BAUD_RATE (115200U)

#define PRINT_SUCCESS UART_sendArray(UART_0, ">>", 2)
#define PRINT_ERR UART_sendArray(UART_0, "Error", 5)

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
 * @brief Initialize environment.
 */
void init(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

void mainLoop(void)
{
    /* Buffers and temporary variables */
    static uint8_t buff[MAX_RECORD_SIZE + 1] = { 0 };
    /* Indexing on `buff` */
    static uint16_t i = 0;

    /* Pop from bottom of queue */
    queue_item_t *bot = NULL;
    /* Indexing on `bot->dat` */
    uint8_t j = 0;

    /* Error code */
    uint32_t rc = QUEUE_ERR_NONE;

    /*     parse_data_struct_t parsedData;
    parse_status_t status = e_parseStatus_undefined; */

    if (QUEUE_isEmpty())
    {
        /* Data is not ready => abort */
        return;
    }

    /* Get new item from the bottom of the queue */
    rc = QUEUE_bot(&bot);
    if (rc != QUEUE_ERR_NONE)
    {
        /* Cannot get new item => abort */
        PRINT_ERR;
        return;
    }

    /* Copy data from `bot->dat` to `buff` because a SREC line can lie on one
     * or more item and an item can contain parts of SREC lines. */
    for (j = 0; bot->dat[j] != '\r'; ++i, ++j)
    {
        if (j >= bot->sz)
        {
            /* No byte left in this queue's item and we haven't reach "\r\n" yet
             * => set to next item in queue */
            QUEUE_pop();
            bot = NULL;

            --i;
            return;
        }

        buff[i] = bot->dat[j];
    }

    buff[i++] = '\r';
    buff[i++] = '\n';
    UART_sendArray(UART_0, buff, i);

    /*     if (bot->sz == QUEUE_MAX_ITEM_SIZE)
    {
        UART_sendArray(UART_0, bot->dat, bot->sz);
    } */
    /*     status = parseData(bot->dat, &parsedData);

    if (status == e_parseStatus_error)
    {
        PRINT_ERR;
    }
    else
    {
        PRINT_SUCCESS;
    } */
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

    UART_enable(UART_0);
    UART_config(UART_0, &uartConf);
}

int main(void)
{
    init();

    while (1)
    {
        mainLoop();
    };
}
