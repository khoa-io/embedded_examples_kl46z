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
#include "main.h"

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
#define CIRCULAR_INDEX_OF(a, li) ((li) % (sizeof((a)) / sizeof(typeof((a)[0]))))

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/* Count the miliseconds */
uint32_t ms_count = 0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Handle SysTick Interrupt. In this program, SysTick_Handler will be
 * configured to be called every milisecond. This function does blink RED LED
 * at 1Hz, GREEN LED at 2Hz.
 */
void SysTick_Handler(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

void SysTick_Handler(void)
{
    /* Blink frequencies of red led (unit: Hz). */
    uint32_t red_led_freq[] = { 1, 2, 3, 4 };
    /* Blink frequencies of green led (unit: Hz). */
    uint32_t green_led_freq[] = { 2, 1, 4, 6, 4, 3 };

    /* Indexing on red_led_freq. */
    static uint8_t i = 0;
    /* Indexing on green_led_freq. */
    static uint8_t j = 0;

    /* Increase counter every 1ms. */
    ms_count++;

    if (GPIO_Read(GPIOC, 3) == 0)
    {
        /* Choose next frequency if SW1 is pushed. */
        ++i;
        i = CIRCULAR_INDEX_OF(red_led_freq, i);
        ++j;
        j = CIRCULAR_INDEX_OF(green_led_freq, j);
    }

    if (GPIO_Read(GPIOC, 12) == 0)
    {
        /* Choose previous frequency if SW2 is pushed. */
        --i;
        i = CIRCULAR_INDEX_OF(red_led_freq, i);
        --j;
        j = CIRCULAR_INDEX_OF(green_led_freq, j);
    }

    if (ms_count % (1000 / (2 * red_led_freq[i])) == 0)
    {
        /* Blink the red led at rate red_led_freq[i] */
        GPIO_Toggle(GPIOE, 29);
    }

    if (ms_count % (1000 / (2 * green_led_freq[j])) == 0)
    {
        /* Blink the green led at rate green_led_freq[j] */
        GPIO_Toggle(GPIOD, 5);
    }

    if (ms_count >= 1000)
    {
        /* Reset counter when it reached max value (1000) */
        ms_count = 0;
    }
}

int main(void)
{
    /* Return code of the functions */
    uint32_t rc;

    /* Setup GPIO for GREEN LED and RED LED */
    GPIO_Init(PORT_D, 5, OUTPUT);
    GPIO_Init(PORT_E, 29, OUTPUT);
    /* Setup GPIO for SW1 and SW2 */
    GPIO_Init(PORT_C, 3, INPUT);
    GPIO_Init(PORT_C, 12, INPUT);

    /* Configure SysTick to generate an interrupt every 1ms */
    rc = SysTick_Config(SystemCoreClock / 1000);

    if (rc != 0)
    {
        /* Failed to configure SysTick => blink RED and GREEN LED */
        GPIO_Toggle(GPIOE, 29);
        GPIO_Toggle(GPIOD, 5);
    }

    while (1)
        ;
}
