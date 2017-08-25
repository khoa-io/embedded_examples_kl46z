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

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Red led on in 200ms */
#define MAX_TIME_RED_ON 200
/* Red led off in 1000ms */
#define MAX_TIME_RED_OFF 1000
/* Green led on in 3000ms */
#define MAX_TIME_GREEN_ON 3000
/* Green led on in 500ms and off in 500ms */
#define TIME_GREEN_BLINK 500

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/* Count the miliseconds for red led.
 * Note: g_ms_red_count = RedOnTime + RedOffTime */
int32_t g_ms_red_count = 0;

/* Count the miliseconds for green led. Default it doesn't count until it is
 * initialized (g_ms_green_count = 0). */
int32_t g_ms_green_count = -1;

/* Toggle blink green led feature */
bool g_blink_green = false;

extern uint32_t SystemCoreClock;

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
    /* Return code of the functions */
    uint32_t rc = 0;

    /* Setup GPIO for (in order) green led, red led, SW1, SW3 */
    GPIO_Init(PORT_D, PIN_GREEN_LED, OUTPUT);
    GPIO_Init(PORT_E, PIN_RED_LED, OUTPUT);
    GPIO_Init(PORT_C, PIN_SW1, INPUT);
    GPIO_Init(PORT_C, PIN_SW3, INPUT);

    /* Leds are turned off by default. */
    GPIO_Write(GPIOE, PIN_RED_LED, 1);
    GPIO_Write(GPIOD, PIN_GREEN_LED, 1);

    /* Configure SysTick to generate an interrupt every 1ms */
    rc = SysTick_Config(SystemCoreClock / 1000);

    if (rc != 0)
    {
        /* Failed to configure SysTick => turn red led on, green led off */
        GPIO_Write(GPIOE, PIN_RED_LED, 0);
        GPIO_Write(GPIOD, PIN_GREEN_LED, 1);
        return rc;
    }

    /* Enable interrupt for SW1 and SW3 */
    PORTC->PCR[PIN_SW1] |= PORT_PCR_IRQC(0x9);
    PORTC->PCR[PIN_SW3] |= PORT_PCR_IRQC(0x9);
    NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
    NVIC_EnableIRQ(PORTC_PORTD_IRQn);
    NVIC_SetPriority(PORTC_PORTD_IRQn, 0);

    while (1)
    {
        /* Main loop (not used) */
    };
}
