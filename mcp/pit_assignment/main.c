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

#include <stdint.h>
#include <stdbool.h>

#include "MKL46Z4.h"
#include "gpio.h"
#include "board.h"
#include "port.h"
#include "pit.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define UART_BAUD_RATE_GEN(clock, sbr, osr) ((clock) / ((sbr) * ((osr) + 1)))

#define UART_GET_SBR(clock, baud, osr) ((clock) / ((baud) * ((osr) + 1)))

#define BAUD_RATE (115200U)

#define ABS(v) ((v) >= 0 ? v : -v)

/*******************************************************************************
 * Global variables
 ******************************************************************************/

extern uint32_t SystemCoreClock;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void secondTickHandler(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
void secondTickHandler(void)
{
    GPIO_Toggle(GPIOD, PIN_GREEN_LED);
}

int main(void)
{
    uint8_t osr = 0;
    uint16_t sbr = 0;
    uint32_t baud = 0;

    uint32_t delta = 0;

    uint32_t minDelta = -1;
    uint32_t minOsr = 0;
    uint16_t minSbr = 0;

    uint8_t *p = (uint8_t *) &minSbr;
    uint8_t a[2] = {0};

    /* Setup GPIO for (in order) green led, red led, SW1, SW2 */
    GPIO_Init(PORT_D, PIN_GREEN_LED, OUTPUT);
    GPIO_Init(PORT_E, PIN_RED_LED, OUTPUT);

    /* Leds are turned off by default. */
    GPIO_Write(GPIOE, PIN_RED_LED, 1);
    GPIO_Write(GPIOD, PIN_GREEN_LED, 1);

    PIT_enable();

    /* Configure timer 0 */
    pit_chnl_conf_t conf;
    conf.time = 1000;
    conf.handler = secondTickHandler;
    conf.chn = false;

    PIT_configChannel(0, &conf);
    PIT_startChannel(0);

    /* Enable clock for PORTA and UART0 */
    SIM_SCGC5 |= SIM_SCGC5_PORTA(1);
    SIM_SCGC4 |= SIM_SCGC4_UART0(1);

    /* Select clock source */
    SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);

    /* Enable UART0_RX */
    PORTA->PCR[1] &= ~PORT_PCR_MUX_MASK;
    PORTA->PCR[1] |= PORT_PCR_MUX(2);
    /* Enable UART0_TX*/
    PORTA->PCR[2] &= ~PORT_PCR_MUX_MASK;
    PORTA->PCR[2] |= PORT_PCR_MUX(2);

    /* Configure the UART control registers for the desired data format */

    /*  Number of data bits */
    UART0->C1 |= UART0_C1_M(0);
    UART0->C4 |= UART0_C4_M10(0);

    /* Parity and parity type */
    UART0->C1 |= UART0_C1_PE(0);

    /* MSB or LSB first */
    UART0->S2 |= UART0_S2_MSBF(0);

    /* Data polarity */
    UART0->S2 |= UART0_S2_RXINV(0);
    UART0->C3 |= UART0_C3_TXINV(0);

    /* Configure the baud rate */
    for (osr = 3; osr < 32; ++osr)
    {
        sbr = UART_GET_SBR(SystemCoreClock, BAUD_RATE, osr);
        baud = UART_BAUD_RATE_GEN(SystemCoreClock, sbr, osr);
        delta = ABS(baud - BAUD_RATE);
        if (delta < minDelta)
        {
            minDelta = delta;
            minOsr = osr;
            minSbr = sbr;
        }
    }

    a[0] = ((uint8_t *) minSbr)[0];
    a[1] = ((uint8_t *) minSbr)[1];

    UART0->BDL |= UART0_BDL_SBR(((uint8_t *) minSbr)[0]);
    UART0->BDH |= UART0_BDH_SBR(((uint8_t *) minSbr)[1]);

    UART0->BDL |= UART0_BDL_SBR(a[0]);
    UART0->BDH |= UART0_BDH_SBR(a[1]);

    /* Enable the receiver and/or transmitter */
    UART0->C2 |= UART0_C2_TE(1);

    while (1)
    {
        /* Main loop (not used) */
        UART0->D = 'A';
    };
}
