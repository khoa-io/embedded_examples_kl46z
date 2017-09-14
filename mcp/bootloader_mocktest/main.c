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
#include "Flash.h"

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

/*!
 * @brief Check if there is an application and run it.
 */
void checkApp(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

void checkApp(void)
{
    uint32_t appResetHandler = 0;
    uint8_t dat[8] = { 0 };

    if (GPIO_Read(GPIOC, PIN_SW1) == 0)
    {
        /* SW1 hit => run bootloader instead of app */
        /* Return to bootloader */
        return;
    }

    if (GPIO_Read(GPIOC, PIN_SW3) == 0)
    {
        /* SW3 hit => remove app and return bootloader */
        LOADER_preload();
        UART_sendArray(UART_0, (uint8_t *)"App is removed!\r\n", 17);
    }

    appResetHandler = Read_FlashAddress(APP_ENTRY_ADDR);

    if (appResetHandler == 0 || appResetHandler == 0xFFFFFFFF)
    {
        UART_sendArray(UART_0, (uint8_t *)"App is not available!\r\n", 23);
        UART_sendArray(UART_0, (uint8_t *)"Please send app via UART.\r\n", 27);
    }
    else
    {
        UART_sendArray(UART_0, (uint8_t *)"Run app!\r\n", 10);
        LOADER_runApp();
    }
}

void checkAndWrite(uint8_t *line)
{
    /* Store e_parseStatus_start and e_parseStatus_done */
    static parse_status_t savedStatus = e_parseStatus_undefined;
    /* `err == true` if there is an error */
    static bool err = false;

    /* Output parsed data */
    parsed_dat_t parsedData;
    /* Status of current parsing */
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
        UART_sendArray(UART_0, (uint8_t *)"Loading ", 9);
        UART_sendArray(UART_0, parsedData.data, parsedData.dataLength);
        UART_sendArray(UART_0, (uint8_t *)". Be patience!\r\n", 16);
        break;

    case e_parseStatus_inprogress:
        if (!(savedStatus == e_parseStatus_start || savedStatus == e_parseStatus_inprogress))
        {
            err = true;
            UART_sendArray(UART_0, (uint8_t *)"Status error 1!\r\n", 17);
            break;
        }

        if (err)
        {
            UART_sendArray(UART_0, (uint8_t *)"Cannot load the app!\r\n", 22);
            break;
        }

        __disable_irq();
        if (LOADER_write(&parsedData) != parsedData.dataLength)
        {
            err = true;
            UART_sendArray(UART_0, (uint8_t *)"Cannot load the app!\r\n", 22);
        }
        __enable_irq();

        break;

    case e_parseStatus_done:
        if (savedStatus != e_parseStatus_inprogress)
        {
            err = true;
            UART_sendArray(UART_0, (uint8_t *)"Status error 2!\r\n", 17);
            break;
        }

        if (err)
        {
            UART_sendArray(UART_0, (uint8_t *)"Cannot load the app!\r\n", 22);
            break;
        }

        UART_sendArray(UART_0, (uint8_t *)"Complete!\r\n", 11);
        UART_sendArray(UART_0, (uint8_t *)"Push RESET to run the app.\r\n", 28);

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
            checkAndWrite(buff);
            i = 0;
            continue;
        }
    }

    QUEUE_pop();
}

void init(void)
{
    /* Initialize UART0 */
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

    /* Initialize SW1 and SW3 */
    GPIO_Init(PORT_C, PIN_SW1, INPUT);
    GPIO_Init(PORT_C, PIN_SW3, INPUT);
}

void main(void)
{
    init();

    UART_sendArray(UART_0, (uint8_t *)"Noobloader v1.0 - A bootloader for FRDM-KL46Z\r\n", 47);
    UART_sendArray(UART_0, (uint8_t *)"Author: KhoaHV1\r\n", 17);
    UART_sendArray(UART_0, (uint8_t *)"Reset while pulling SW1 to run bootloader.\r\n", 44);
    UART_sendArray(UART_0, (uint8_t *)"Reset while pulling SW3 to remove app.\r\n", 40);
    checkApp();

    while (1)
    {
        mainLoop();
    };
}
