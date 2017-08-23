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

#include "gpio.h"
#include "board.h"
#include "port.h"

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/* Count the miliseconds. Default: no counting. When set to 0: start counting */
int32_t g_ms_count = -1;

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

void PORTC_PORTD_IRQHandler(void)
{
    if (PORT_get_pin_isf(PORTC, PIN_SW1))
    {
        /* SW1 hit */
        /* Clear interrupt service flag */
        PORT_clear_pin_isf(PORTC, PIN_SW1);

        /* Toggle green LED status */
        GPIO_Toggle(GPIOD, PIN_GREEN_LED);
    }

    if (PORT_get_pin_isf(PORTC, PIN_SW3))
    {
        /* SW3 hit */
        /* Clear interrupt service flag */
        PORT_clear_pin_isf(PORTC, PIN_SW3);

        /* Set start counting */
        g_ms_count = 0;
        /* Turn on red LED */
        GPIO_Write(GPIOE, PIN_RED_LED, 0);
    }
}

void SysTick_Handler(void)
{
    if (g_ms_count < 0)
    {
        /* No counting */
        return;
    }

    /* Increase counter every 1ms. */
    ++g_ms_count;

    if (g_ms_count >= 2000)
    {
        /* Turn off red LED when count to 2000ms */
        GPIO_Write(GPIOE, PIN_RED_LED, 1);
        /* Stop counting */
        g_ms_count = -1;
    }
}

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
