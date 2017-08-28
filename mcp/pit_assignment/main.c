/*
 * This is template for main module created by KSDK Project Generator. Enjoy!
 */

/*
 * [File Name]     main.c
 * [Platform]      MKL46Z256VLH4
 * [Project]       pit_example
 * [Version]       1.00
 * [Author]        KhoaHV1
 * [Date]          24-08-2017
 * [Language]      'C'
 * [History]       1.00 - Original Release
 *
 */

#include "gpio.h"
#include "board.h"
#include "port.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define COUNT_VAL 4799999U
/*!
 * @brief Calculate value for LDVAL register.
 *
 * @param T Period (ms).
 * @param F Clock frequency (MHz).
 *
 * @return Value for LDVAL register.
 */
#define GET_COUNT(T, F) ((1000 * (T) * (F)) - 1)

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void PIT_IRQHandler(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
void PIT_IRQHandler(void)
{
    if (PIT->CHANNEL[0].TFLG)
    {
        PIT->CHANNEL[0].TFLG = 1;
        GPIO_Toggle(GPIOD, PIN_GREEN_LED);
    }
}

int main(void)
{
    /* Setup GPIO for (in order) green led, red led, SW1, SW2 */
    GPIO_Init(PORT_D, PIN_GREEN_LED, OUTPUT);
    GPIO_Init(PORT_E, PIN_RED_LED, OUTPUT);

    /* Leds are turned off by default. */
    GPIO_Write(GPIOE, PIN_RED_LED, 1);
    GPIO_Write(GPIOD, PIN_GREEN_LED, 1);

    /* Enable clock */
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

    /* Enable interrupt */
    NVIC_ClearPendingIRQ(PIT_IRQn);
    NVIC_EnableIRQ(PIT_IRQn);
    NVIC_SetPriority(PIT_IRQn, 0);

    /* Turn on PIT */
    PIT->MCR = 0x00;

    /* Configure timer 0 */
    PIT->CHANNEL[0].LDVAL = GET_COUNT(100, 48);
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE_MASK;
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;

    while (1)
    {
        /* Main loop (not used) */
    };
}
