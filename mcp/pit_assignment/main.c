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

#include "MKL46Z4.h"
#include "gpio.h"
#include "board.h"
#include "port.h"
#include "pit.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Calculate value for LDVAL register from period (ms).
 *
 * @param T Period (ms).
 *
 * @return Value for LDVAL register.
 */
#define LDVAL_GET_COUNT(T) (((T) / 1000) * SystemCoreClock / 2 - 1)

/*******************************************************************************
 * Global variables
 ******************************************************************************/

extern uint32_t SystemCoreClock;

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
    
    PIT_enable();

    /* Configure timer 0 */
    PIT->CHANNEL[0].LDVAL = LDVAL_GET_COUNT(1000);
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE_MASK;
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;

    while (1)
    {
        /* Main loop (not used) */
    };
}
