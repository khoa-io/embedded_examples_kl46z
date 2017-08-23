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
 * Macros
 ******************************************************************************/

/*!
 * @brief Convert linear index to circular index.
 *
 * @param a Array.
 * @param li Linear index.
 *
 * @return Return circular index.
 */
#define CIRCULAR_INDEX_OF(a, li) ((li) % (sizeof((a)) / sizeof(a[0])))

/*!
 * @brief Calculate period from frequency.
 *
 * @param f Frequency.
 *
 * @return Return period.
 */
#define FREQ_TO_PERIOD(f) (1000 / (f))

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/* Count the miliseconds */
uint32_t g_ms_count = 0;

/* Blink frequencies of red led (unit: Hz). */
uint32_t g_red_led_freq[] = {1, 2, 3, 4};
/* Indexing on g_red_led_freq. */
uint8_t g_ired = 0;

/* Blink frequencies of green led (unit: Hz). */
uint32_t g_green_led_freq[] = {4, 3, 2, 1};
/* Indexing on g_green_led_freq. */
uint8_t g_igreen = 0;

extern uint32_t SystemCoreClock;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Handle SW1, SW2 pressed.
 */
void PORTC_PORTD_IRQHandler(void);

/*!
 * @brief Handle SysTick Interrupt. In this program, SysTick_Handler will be
 * configured to be called every 1ms. This function does blink red led at 1Hz,
 * green led at 2Hz.
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

        /* Choose next frequency if SW1 is pushed. */
        ++g_ired;
        g_ired = CIRCULAR_INDEX_OF(g_red_led_freq, g_ired);
        ++g_igreen;
        g_igreen = CIRCULAR_INDEX_OF(g_green_led_freq, g_igreen);
    }

    if (PORT_get_pin_isf(PORTC, PIN_SW2))
    {
        /* SW2 hit */
        /* Clear interrupt service flag */
        PORT_clear_pin_isf(PORTC, PIN_SW2);

        /* Choose previous frequency if SW2 is pushed. */
        --g_ired;
        g_ired = CIRCULAR_INDEX_OF(g_red_led_freq, g_ired);
        --g_igreen;
        g_igreen = CIRCULAR_INDEX_OF(g_green_led_freq, g_igreen);
    }
}

void SysTick_Handler(void)
{
    /* Increase counter every 1ms. */
    ++g_ms_count;

    if (g_ms_count % FREQ_TO_PERIOD(g_red_led_freq[g_ired]) == 0)
    {
        /* Blink the red led at g_red_led_freq[i] Hz */
        GPIO_Toggle(GPIOE, PIN_RED_LED);
    }

    if (g_ms_count % FREQ_TO_PERIOD(g_green_led_freq[g_igreen]) == 0)
    {
        /* Blink the green led at g_green_led_freq[j] Hz */
        GPIO_Toggle(GPIOD, PIN_GREEN_LED);
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

    /* Leds are turned off by default. */
    GPIO_Write(GPIOE, PIN_RED_LED, 1);
    GPIO_Write(GPIOD, PIN_GREEN_LED, 1);

    /* Configure SysTick to generate an interrupt every 1ms */
    rc = SysTick_Config(SystemCoreClock / 1000);

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
