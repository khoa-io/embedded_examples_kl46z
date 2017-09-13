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

#define PRINT_QUEUE_ERR UART_sendArray(UART_0, (uint8_t *)"QueueError\r\n", 12)

/*******************************************************************************
 * Macros
 ******************************************************************************/

#define ABS(v) ((v) >= 0 ? v : -v)

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/* Default system core clock */
extern uint32_t SystemCoreClock;

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

    /* Disable UART0 before setting up configuration */
    /* Disable UART0_TX*/
    PORTA->PCR[2] &= ~PORT_PCR_MUX_MASK;
    /* Disable UART0_RX */
    PORTA->PCR[1] &= ~PORT_PCR_MUX_MASK;
    /* Disable interrupt */
    NVIC_DisableIRQ(UART0_IRQn);
    NVIC_ClearPendingIRQ(UART0_IRQn);
}

void UART_config(uint8_t uartx, uart_conf_t *conf)
{
    uint16_t sbr = 0;

    if (uartx != UART_0)
    {
        /* Only support UART0 for now */
        return;
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
    sbr = SystemCoreClock / (conf->baudRate * 16);
    /* write the sbr value to the BDH and BDL registers*/
    UART0->BDL &= ~UART0_BDL_SBR_MASK;
    UART0->BDL |= UART0_BDL_SBR(((uint8_t *)&sbr)[0]);
    UART0->BDH &= ~UART0_BDH_SBR_MASK;
    UART0->BDH |= UART0_BDH_SBR(((uint8_t *)&sbr)[1]);

    /* Enable the receiver and/or transmitter */
    UART0->C2 |= (conf->type & UART_TYPE_TRANSMITTER_MASK) ? UART0_C2_TE(1)
                                                           : UART0_C2_TE(0);

    UART0->C2 |= (conf->type & UART_TYPE_RECEIVER_MASK) ? UART0_C2_RE(1)
                                                        : UART0_C2_RE(0);
    /* Enable receiver interrupt */
    /* UART0->C2 |= (conf->type & UART_TYPE_RECEIVER_MASK) ? UART0_C2_RIE(1)
                                                        : UART0_C2_RIE(0); */

    /* Enable UART0 after setting up configuration */
    if (conf->type & UART_TYPE_TRANSMITTER_MASK)
    {
        /* Enable UART0_TX*/
        PORTA->PCR[2] |= PORT_PCR_MUX(2);
    }

    if (conf->type & UART_TYPE_RECEIVER_MASK)
    {
        /* Enable UART0_RX */
        PORTA->PCR[1] |= PORT_PCR_MUX(2);

        /* Enable receiver interrupt */
        NVIC_ClearPendingIRQ(UART0_IRQn);
        NVIC_EnableIRQ(UART0_IRQn);
        NVIC_ClearPendingIRQ(UART0_IRQn);
        NVIC_SetPriority(UART0_IRQn, 0);
    }
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

void UART_sendArray(uint8_t uartx, uint8_t *arr, uint8_t sz)
{
    uint8_t i = 0;
    for (; i < sz; ++i)
    {
        UART_sendByte(uartx, arr[i]);
    }
}

void UART0_IRQHandler(void)
{
    /* The item for pushing to top of queue */
    static queue_item_t *top = NULL;

    /* Error code */
    uint32_t rc = QUEUE_ERR_NONE;

    if ((UART0->S1 & UART0_S1_RDRF_MASK) == 0)
    {
        /* Not ready to receive => abort */
        return;
    };

    if (top == NULL)
    {
        /* Get next item if we don't have one */
        rc = QUEUE_top(&top);
    }

    if (rc != QUEUE_ERR_NONE)
    {
        /* Can't get new item => abort */
        PRINT_QUEUE_ERR;
        return;
    }

    /* Add received byte to the item */
    QUEUE_itemAddByte(*top, UART0->D);

    if (QUEUE_itemIsFull(*top) || (UART0->S1 & UART0_S1_IDLE_MASK))
    {
        /* This item is full or receive line becomes idle
         * => push to queue and release it */
        QUEUE_push();
        top = NULL;
    }
}
