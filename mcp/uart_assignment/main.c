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
#include "pit.h"
#include "port.h"
#include "uart.h"
#include "queue.h"

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

/*******************************************************************************
 * Code
 ******************************************************************************/

void f()
{
    queue_item_t *pushItem = NULL;
    queue_item_t *popItem = NULL;

    uint32_t rc = QUEUE_ERR_NONE;

    uint8_t dat0[] = "HOANGVANKHOA\r\n";
    uint8_t dat1[] = "PUSHITEM\r\n";
    uint8_t err[] = "Error\r\n";

    uint8_t i = 0;

    rc = QUEUE_push(&pushItem);
    if (rc != QUEUE_ERR_NONE)
    {
        UART_sendBytes(UART_0, err, sizeof(err));
        return;
    }

    for (i = 0; i < sizeof(dat0); ++i)
    {
        pushItem->dat[i] = dat0[i];
    }
    pushItem->sz = i;

    rc = QUEUE_push(&pushItem);
    if (rc != QUEUE_ERR_NONE)
    {
        UART_sendBytes(UART_0, err, sizeof(err));
        return;
    }
    for (i = 0; i < sizeof(dat1); ++i)
    {
        pushItem->dat[i] = dat1[i];
    }
    pushItem->sz = i;

    rc = QUEUE_pop(&popItem);
    if (rc != QUEUE_ERR_NONE)
    {
        UART_sendBytes(UART_0, err, sizeof(err));
        return;
    }
    UART_sendBytes(UART_0, popItem->dat, popItem->sz);

    rc = QUEUE_pop(&popItem);
    if (rc != QUEUE_ERR_NONE)
    {
        UART_sendBytes(UART_0, err, sizeof(err));
        return;
    }
    UART_sendBytes(UART_0, popItem->dat, popItem->sz);
}

int main(void)
{
    /* Store configuration of UART0 */
    uart_conf_t uartConf;

    uint8_t i = 0;

    /* Configure UART0 */
    uartConf.type = UART_TYPE_TRANSMITTER_MASK;
    uartConf.sz = 8;
    uartConf.parityEnable = false;
    uartConf.msbf = false;
    uartConf.polarity = 0;
    uartConf.baudRate = BAUD_RATE;

    UART_enable(UART_0);
    UART_config(UART_0, &uartConf);

    for (i = 0; i < 4; ++i)
    {
        f();
    }

    while (1)
    {
        /* Main loop (not used) */
    };
}
