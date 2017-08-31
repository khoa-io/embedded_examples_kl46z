/*
 * This is template for main module created by KSDK Project Generator. Enjoy!
 */

/*
 * [File Name]     main.c
 * [Platform]      MKL46Z256VLH4
 * [Project]       pit_assignment
 * [Version]       1.00
 * [Author]        KhoaHV1
 * [Date]          28-08-2017
 * [Language]      'C'
 * [History]       1.00 - Original Release
 *
 */

#include <stdbool.h>
#include <stdint.h>

#include "MKL46Z4.h"
#include "board.h"
#include "gpio.h"
#include "pit.h"
#include "port.h"
#include "uart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BAUD_RATE (115200U)
#define BUFF_SIZE (256U)

/*******************************************************************************
 * Global variables
 ******************************************************************************/
/* Count the second of clock (from 0-59) */
uint8_t g_secCnt = -1;
/* Count the minute of clock (from 0-99) */
uint8_t g_minCnt = 0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Called every second.
 */
void secondTickHandler(void);

/*!
 * @brief Copy an integer number to a buffer as a string.
 *
 * @param val  [in]     Integer number.
 * @param buff [in,out] Buffer store string of `val`. `buff` must be wide enough
 * to store the string.
 *
 * @return Return number of numeric character in `val`.
 */
uint8_t int2str(uint32_t val, uint8_t buff[]);

/*******************************************************************************
 * Code
 ******************************************************************************/
void secondTickHandler(void)
{
    /* Store bytes to send */
    uint8_t buff[BUFF_SIZE] = {0};

    /* Number of bytes will be sent */
    uint8_t n = 0;

    /* Increase the second each second */
    ++g_secCnt;

    if (g_secCnt > 59)
    {
        /* Increase the minute and reset the second */
        ++g_minCnt;
        g_secCnt = 0;
    }

    if (g_minCnt > 99)
    {
        /* Reset the minute if it reached max value */
        g_minCnt = 0;
    }

    n = int2str(g_secCnt, buff);
    buff[n++] = ':';
    n += int2str(g_minCnt, buff + n);

    /* Remove previous output */
    UART_sendBytes(UART_0, "\b\b\b\b\b", n);
    /* Print format: "ss:mm" */
    UART_sendBytes(UART_0, buff, n);
}

uint8_t int2str(uint32_t val, uint8_t buff[])
{
    /* Indexing variables */
    uint8_t i = 0;
    uint8_t j = 0;

    /* Number of numeric characters in val */
    uint8_t n = 0;

    /* Copy `val` to `buff` by reversed-order */
    for (i = 0; val != 0; ++i)
    {
        buff[i] = val % 10 + '0';
        val = val / 10;
        ++n;
    }

    /* Reverse buff */
    for (j = 0; j < i / 2; ++j)
    {
        buff[j] ^= buff[i - j - 1];
        buff[i - j - 1] ^= buff[j];
        buff[j] ^= buff[i - j - 1];
    }

    if (n < 1)
    {
        /* If `val == 0` then copy '00' to `buff` */
        buff[0] = '0';
        buff[1] = '0';
        n = 2;
    }

    if (n < 2)
    {
        /* If `val < 10` then copy "0x" to `buff` (x == `val`) */
        buff[1] = buff[0];
        buff[0] = '0';
        n = 2;
    }

    return n;
}

int main(void)
{
    /* Store configuration of PIT channel 0 */
    pit_chnl_conf_t pitConf;

    /* Store configuration of UART0 */
    uart_conf_t uartConf;

    PIT_enable();

    /* Configure timer 0 */
    pitConf.time = 1000;
    pitConf.handler = secondTickHandler;
    pitConf.chn = false;

    PIT_configChannel(0, &pitConf);
    PIT_startChannel(0);

    /* Configure UART0 */
    uartConf.type = UART_TYPE_TRANSMITTER_MASK;
    uartConf.sz = 8;
    uartConf.parityEnable = false;
    uartConf.msbf = false;
    uartConf.polarity = 0;
    uartConf.baudRate = BAUD_RATE;

    UART_enable(UART_0);
    UART_config(UART_0, &uartConf);

    while (1)
    {
        /* Main loop (not used) */
    };
}
