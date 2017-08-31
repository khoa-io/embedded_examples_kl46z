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

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Callback will be called when receive data.
 */
void onReceive(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

void onReceive(void)
{
    /* Indexing variables */
    int32_t i = 0;
    /* Line number */
    int32_t j = 0;

    /* Pop from bottom of queue */
    static queue_item_t *bot = NULL;

    /* Buffers and temporary variables */
    int8_t buff[MAX_RECORD_SIZE + 1] = { 0 };
    uint8_t c = 0;

    /* Error code */
    uint32_t rc = QUEUE_ERR_NONE;

    parse_data_struct_t parsedData;
    parse_status_t status = e_parseStatus_undefined;

    rc = QUEUE_pop(&bot);
    if (rc != QUEUE_ERR_NONE)
    {
        return;
    }

    UART_sendBytes(UART_0, bot->dat, bot->sz);
}

int main(void)
{
    /* Store configuration of UART0 */
    uart_conf_t uartConf;

    /* Configure UART0 */
    uartConf.type = UART_TYPE_TRANSMITTER_MASK | UART_TYPE_RECEIVER_MASK;
    uartConf.sz = 8;
    uartConf.parityEnable = false;
    uartConf.msbf = false;
    uartConf.polarity = 0;
    uartConf.baudRate = BAUD_RATE;
    uartConf.onReceive = onReceive;

    UART_enable(UART_0);
    UART_config(UART_0, &uartConf);

    while (1)
    {
        /* Main loop (not used) */
    };
}
