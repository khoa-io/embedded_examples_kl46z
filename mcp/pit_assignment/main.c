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

/*******************************************************************************
 * Global variables
 ******************************************************************************/

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

    while (1)
    {
        /* Main loop (not used) */
    };
}
