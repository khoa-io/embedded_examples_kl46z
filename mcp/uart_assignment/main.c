/*
 * This is template for main module created by KSDK Project Generator. Enjoy!
 */

/*
 * [File Name]     main.c
 * [Platform]      MKL46Z256VLH4
 * [Project]       exception_model
 * [Version]       1.00
 * [Author]        KhoaHV1
 * [Date]          21-08-2017
 * [Language]      'C'
 * [History]       1.00 - Original Release
 *
 */

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"
#include "board.h"
#include "port.h"
#include "uart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Handle SW1, SW3 pressed.
 */
void PORTC_PORTD_IRQHandler(void);
/*!
 * @brief Handle SysTick Interrupt.
 */
void SysTick_Handler(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    /* Gợi ý: tạo ra uart_config_struct_t để chứa các config */
    /* ... */
    /* Enable clock for UART0 */
    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
    /* Select clock source */
    SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
    /* Enable UART0_RX */
    PORTA->PCR[1] &= ~PORT_PCR_MUX_MASK;
    PORTA->PCR[1] |= PORT_PCR_MUX(1);
    /* Enable UART0_TX*/
    PORTA->PCR[2] &= ~PORT_PCR_MUX_MASK;
    PORTA->PCR[2] |= PORT_PCR_MUX(1);

    /* Configure the UART control registers for the desired data format */
    /*  Number of data bits */
    /* UART0->C1 */
    /* Parity and parity type */

    /* MSB or LSB first */

    /* Data polarity */

    /* Configure the baud rate */

    /* Enable the receiver and/or transmitter */

    while (1)
    {
        /* Main loop (not used) */
    };
}
