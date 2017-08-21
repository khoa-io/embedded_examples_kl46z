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
#define CIRCULAR_INDEX_OF(a, li) ((li) % (sizeof((a)) / sizeof(a[0])))

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/* Count the miliseconds */
uint32_t ms_count = 0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* TODO comment here */
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
    ++ms_count;
}

void SysTick_Handler(void)
{
    /* Blink frequencies of red led (unit: Hz). */
    static uint32_t red_led_freq[] = { 1, 2, 3, 4 };
    /* Blink frequencies of green led (unit: Hz). */
    static uint32_t green_led_freq[] = { 6, 5, 4, 3, 2, 1 };

    /* Indexing on red_led_freq. */
    static uint8_t i = 0;
    /* Indexing on green_led_freq. */
    static uint8_t j = 0;

    /* Increase counter every 1ms. */
    ++ms_count;

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
        /* Blink the red led at red_led_freq[i] Hz */
        GPIO_Toggle(GPIOE, 29);
    }

    if (ms_count % (1000 / (2 * green_led_freq[j])) == 0)
    {
        /* Blink the green led at green_led_freq[j] Hz */
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
    /* TODO un-comment following line when done */
    rc = SysTick_Config(SystemCoreClock / 1000);
    /*__disable_irq();
     NVIC_SetPriority(PORTC_PORTD_IRQn, 0);
     NVIC_EnableIRQ(PORTC_PORTD_IRQn);*/

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
