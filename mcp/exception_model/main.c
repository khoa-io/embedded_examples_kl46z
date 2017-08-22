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
uint32_t g_red_led_freq[] = { 1, 2, 3, 4 };
/* Indexing on g_red_led_freq. */
uint8_t g_ired = 0;

/* Blink frequencies of green led (unit: Hz). */
uint32_t g_green_led_freq[] = { 4, 3, 2, 1 };
/* Indexing on g_green_led_freq. */
uint8_t g_igreen = 0;

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
    if (PORTC->PCR[3] & PORT_PCR_ISF_MASK)
    {
        /* SW1 hit */
        /* Clear interrupt service flag */
        PORTC->PCR[3] |= PORT_PCR_ISF_MASK;

        /* Choose next frequency if SW1 is pushed. */
        ++g_ired;
        g_ired = CIRCULAR_INDEX_OF(g_red_led_freq, g_ired);
        ++g_igreen;
        g_igreen = CIRCULAR_INDEX_OF(g_green_led_freq, g_igreen);
    }

    if (PORTC->PCR[12] & PORT_PCR_ISF_MASK)
    {
        /* SW2 hit */
        /* Clear interrupt service flag */
        PORTC->PCR[12] |= PORT_PCR_ISF_MASK;

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
        GPIO_Toggle(GPIOE, 29);
    }

    if (g_ms_count % FREQ_TO_PERIOD(g_green_led_freq[g_igreen]) == 0)
    {
        /* Blink the green led at g_green_led_freq[j] Hz */
        GPIO_Toggle(GPIOD, 5);
    }

    if (g_ms_count >= 1000)
    {
        /* Reset counter when it reached max value (1000) */
        g_ms_count = 0;
    }
}

int main(void)
{
    /* Return code of the functions */
    uint32_t rc = 0;

    /* Setup GPIO for GREEN LED and RED LED */
    GPIO_Init(PORT_D, 5, OUTPUT);
    GPIO_Init(PORT_E, 29, OUTPUT);
    /* Setup GPIO for SW1 and SW2 */
    GPIO_Init(PORT_C, 3, INPUT);
    GPIO_Init(PORT_C, 12, INPUT);

    /* Leds are turned off by default. */
    GPIO_Write(GPIOE, 29, 1);
    GPIO_Write(GPIOD, 5, 1);

    /* Configure SysTick to generate an interrupt every 1ms */
    rc = SysTick_Config(SystemCoreClock / 1000);

    /* Enable interrupt for SW1 and SW2 */
    PORTC->PCR[3] |= PORT_PCR_IRQC(0x9);
    PORTC->PCR[12] |= PORT_PCR_IRQC(0x9);
    NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
    NVIC_EnableIRQ(PORTC_PORTD_IRQn);
    NVIC_SetPriority(PORTC_PORTD_IRQn, 0);

    if (rc != 0)
    {
        /* Failed to configure SysTick => turn on not blink red and green led */
        GPIO_Write(GPIOE, 29, 0);
        GPIO_Write(GPIOD, 5, 0);
    }

    while (1)
    {
    };
}
