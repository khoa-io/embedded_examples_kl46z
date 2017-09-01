#include <stddef.h>

#include "MKL46Z4.h"
#include "queue.h"
#include "uart.h"

/*!
 * @file uart.c Implementation of UART. Only support UART0 module for now.
 * @author KhoaHV1
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define UART_ERR_MSG "Error"
#define UART_ERR_MSG_SZ (6U)

/*******************************************************************************
 * Macros
 ******************************************************************************/

#define UART_BAUD_RATE_GEN(clock, sbr, osr) ((clock) / ((sbr) * ((osr) + 1)))

#define UART_GET_SBR(clock, baud, osr) ((clock) / ((baud) * ((osr) + 1)))

#define ABS(v) ((v) >= 0 ? v : -v)

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/* Default system core clock */
extern uint32_t SystemCoreClock;

/* UART0 receive handler */
void (*uart0OnReceive)(void) = NULL;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Called when receiving data.
 */
void UART0_IRQHandler(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

void UART_enable(uint8_t uartx)
{
    if (uartx != UART_0)
    {
        return;
    }

    /* Enable clock for PORTA and UART0 */
    SIM_SCGC5 |= SIM_SCGC5_PORTA(1);
    SIM_SCGC4 |= SIM_SCGC4_UART0(1);

    /* Select clock source */
    SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);
}

void UART_config(uint8_t uartx, uart_conf_t *conf)
{
    uint8_t osr = 0;
    uint16_t sbr = 0;
    uint32_t baud = 0;

    uint32_t delta = 0;

    uint32_t minDelta = -1;
    uint32_t minOsr = 0;
    uint16_t minSbr = 0;

    if (uartx != UART_0)
    {
        return;
    }

    if (conf->type & UART_TYPE_TRANSMITTER_MASK)
    {
        /* Enable UART0_TX*/
        PORTA->PCR[2] &= ~PORT_PCR_MUX_MASK;
        PORTA->PCR[2] |= PORT_PCR_MUX(2);
    }

    if (conf->type && UART_TYPE_RECEIVER_MASK)
    {
        /* Enable UART0_RX */
        PORTA->PCR[1] &= ~PORT_PCR_MUX_MASK;
        PORTA->PCR[1] |= PORT_PCR_MUX(2);

        /* Enable receiver interrupt */
        NVIC_ClearPendingIRQ(UART0_IRQn);
        NVIC_EnableIRQ(UART0_IRQn);
        NVIC_SetPriority(UART0_IRQn, 0);

        uart0OnReceive = conf->onReceive;
    }

    /*  Number of data bits */
    switch (conf->sz)
    {
    case 8:
        UART0->C1 |= UART0_C1_M(0);
        UART0->C4 |= UART0_C4_M10(0);
        break;

    case 9:
        UART0->C1 |= UART0_C1_M(1);
        UART0->C4 |= UART0_C4_M10(0);
        break;

    case 10:
        UART0->C4 |= UART0_C4_M10(1);
        break;
    }

    /* Parity and parity type */
    UART0->C1 |= conf->parityEnable ? UART0_C1_PE(1) : UART0_C1_PE(0);
    UART0->C1 |= conf->parityType ? UART0_C1_PT(1) : UART0_C1_PT(0);

    /* MSB or LSB first */
    UART0->S2 |= conf->msbf ? UART0_S2_MSBF(1) : UART0_S2_MSBF(0);

    /* Data polarity */
    UART0->S2 |= (conf->polarity & UART_DATA_POLARITY_RX_MASK)
        ? UART0_S2_RXINV(1)
        : UART0_S2_RXINV(0);
    UART0->C3 |= (conf->polarity & UART_DATA_POLARITY_TX_MASK)
        ? UART0_C3_TXINV(1)
        : UART0_C3_TXINV(0);

    /* Configure the baud rate */
    for (osr = 3; osr < 32; ++osr)
    {
        sbr = UART_GET_SBR(SystemCoreClock, conf->baudRate, osr);
        baud = UART_BAUD_RATE_GEN(SystemCoreClock, sbr, osr);
        delta = ABS(baud - conf->baudRate);
        if (delta < minDelta)
        {
            minDelta = delta;
            minOsr = osr;
            minSbr = sbr;
        }
    }

    UART0->BDL &= ~UART0_BDL_SBR_MASK;
    UART0->BDL |= UART0_BDL_SBR(((uint8_t *)&minSbr)[0]);
    UART0->BDH &= ~UART0_BDH_SBR_MASK;
    UART0->BDH |= UART0_BDH_SBR(((uint8_t *)&minSbr)[1]);

    UART0->C4 &= UART0_C4_OSR(minOsr); /* Not exactly */

    /* Enable the receiver and/or transmitter */
    UART0->C2 |= (conf->type & UART_TYPE_TRANSMITTER_MASK) ? UART0_C2_TE(1)
                                                           : UART0_C2_TE(0);

    UART0->C2 |= (conf->type & UART_TYPE_RECEIVER_MASK) ? UART0_C2_RE(1)
                                                        : UART0_C2_RE(0);
    /* Enable receiver interrupt */
    UART0->C2 |= (conf->type & UART_TYPE_RECEIVER_MASK) ? UART0_C2_RIE(1)
                                                        : UART0_C2_RIE(0);
}

void UART_sendByte(uint8_t uartx, uint8_t b)
{
    while ((UART0->S1 & UART0_S1_TDRE_MASK) == 0)
    {
        /* Waiting here */
    };

    if (uartx == UART_0)
    {
        UART0->D = b;
    }
}

void UART_sendBytes(uint8_t uartx, uint8_t *arr, uint8_t sz)
{
    uint8_t i = 0;
    for (; i < sz; ++i)
    {
        UART_sendByte(uartx, arr[i]);
    }
}

uint32_t UART_readByte(uint8_t uartx, uint8_t *b)
{
    /* Error code */
    uint32_t rc = UART_ERR_NONE;

    if ((UART0->S1 & UART0_S1_OR_MASK))
    {
        /* TODO */
        rc = UART_ERR_UNKNOWN;
    }

    while ((UART0->S1 & UART0_S1_RDRF_MASK) == 0)
    {
        /* Waiting here */
//        if ((UART0->S1 & UART0_S1_IDLE_MASK))
//        {
//            /* UART receive line becomes idle => abort */
//            rc = UART_ERR_IDLE;
//            UART0->S1 |= UART0_S1_IDLE(1);
//            return rc;
//        }
    }

    *b = UART0->D;

    return rc;
}

uint32_t UART_readBytes(uint8_t uartx, uint8_t *buff, uint8_t n, uint8_t *r)
{
    /* Error code */
    uint32_t rc = UART_ERR_NONE;

    /* Indexing variable */
    uint8_t i = 0;

    /* Read `n` byte, stop when encountered an error */
    for (i = 0; i < n && rc == UART_ERR_NONE; ++i)
    {
        rc = UART_readByte(uartx, &buff[i]);
    }

    *r = i;
    return rc;
}

uint32_t UART_readLine(uint8_t uartx, uint8_t *buff, uint8_t n, uint8_t *r)
{
    /* Error code */
    uint32_t rc = UART_ERR_NONE;

    /* Indexing variable */
    uint8_t i = 0;
    /* Read `n` byte */
    for (i = 0; i < n; ++i)
    {
        rc = UART_readByte(uartx, &buff[i]);

        if (rc != UART_ERR_NONE)
        {
            /* Stop when encountered an error */
            break;
        }

        if (buff[i] == '\n')
        {
            /* Stop when reached new line */
            ++i;
            break;
        }
    }

    *r = i;
    return rc;
}

void UART0_IRQHandler(void)
{
    /* Push to top of queue */
    static queue_item_t *top = NULL;

    /* Error code */
    uint32_t rc = 0;

    rc = QUEUE_push(&top);
    if (rc != QUEUE_ERR_NONE)
    {
        return;
    }

    rc = UART_readLine(UART_0, top->dat, QUEUE_MAX_ITEM_SIZE, &top->sz);

    if (rc != UART_ERR_NONE)
    {
        return;
    }

    if (uart0OnReceive)
    {
        /* Call handler to get data */
        uart0OnReceive();
    }
}
