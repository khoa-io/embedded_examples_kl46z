#include "MKL46Z4.h"
#include "uart.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/

#define UART_BAUD_RATE_GEN(clock, sbr, osr) ((clock) / ((sbr) * ((osr) + 1)))

#define UART_GET_SBR(clock, baud, osr) ((clock) / ((baud) * ((osr) + 1)))

#define ABS(v) ((v) >= 0 ? v : -v)

/*******************************************************************************
 * Global variables
 ******************************************************************************/

extern uint32_t SystemCoreClock;

/*******************************************************************************
 * Code
 ******************************************************************************/

void UART_enableUart(uint8_t uartx)
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

void UART_configUart(uint8_t uartx, uart_conf_t *conf)
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
    UART0->C2 |= (conf->type & UART_TYPE_TRANSMITTER_MASK)
                     ? UART0_C2_TE(1)
                     : UART0_C2_TE(0);

    UART0->C2 |= (conf->type & UART_TYPE_RECEIVER_MASK)
                     ? UART0_C2_RE(1)
                     : UART0_C2_RE(0);
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