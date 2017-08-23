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

/* Count the miliseconds */
uint32_t g_ms_count = -1;

extern uint32_t SystemCoreClock;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Handle SW1, SW2 pressed.
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

    if (PORT_get_pin_isf(PORTC, PIN_SW2))
    {
        /* SW2 hit */
        /* Clear interrupt service flag */
        PORT_clear_pin_isf(PORTC, PIN_SW2);

        g_ms_count = 0;
        /* Turn on red LED */
        GPIO_Write(GPIOE, PIN_RED_LED, 0);
    }
}

void SysTick_Handler(void)
{
    if (g_ms_count < 0)
    {
        return;
    }

    /* Increase counter every 1s. */
    ++g_ms_count;

    if (g_ms_count > 2)
    {
        /* Turn off red LED */
        GPIO_Write(GPIOE, PIN_RED_LED, 1);
        g_ms_count = -1;
    }
}

int main(void)
{
    /* Return code of the functions */
    uint32_t rc = 0;

    /* Setup GPIO for (in order) green led, red led, SW1, SW2 */
    GPIO_Init(PORT_D, PIN_GREEN_LED, OUTPUT);
    GPIO_Init(PORT_E, PIN_RED_LED, OUTPUT);
    GPIO_Init(PORT_C, PIN_SW1, INPUT);
    GPIO_Init(PORT_C, PIN_SW2, INPUT);

    /* Leds are turned on by default. */
    GPIO_Write(GPIOE, PIN_RED_LED, 0);
    GPIO_Write(GPIOD, PIN_GREEN_LED, 0);

    /* Configure SysTick to generate an interrupt every 1s */
    rc = SysTick_Config(SystemCoreClock);

    if (rc != 0)
    {
        /* Failed to configure SysTick => turn on red led */
        GPIO_Write(GPIOE, PIN_RED_LED, 0);
        return rc;
    }

    /* Enable interrupt for SW1 and SW2 */
    PORTC->PCR[PIN_SW1] |= PORT_PCR_IRQC(0x9);
    PORTC->PCR[PIN_SW2] |= PORT_PCR_IRQC(0x9);
    NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
    NVIC_EnableIRQ(PORTC_PORTD_IRQn);
    NVIC_SetPriority(PORTC_PORTD_IRQn, 0);

    while (1)
    {
        /* Main loop (not used) */
    };
}
