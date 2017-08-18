#include "MKL46Z4.h"
#include "gpio.h"

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/* Count the mili-seconds */
volatile uint32_t ms_count = 0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Handle SysTick Interupt. In this program, SysTick_Handler will be
 * configured to be called every mili-second. This function does blink RED LED
 * at 1Hz, GREEN LED at 2Hz.
 */
void SysTick_Handler(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

void SysTick_Handler(void)
{
    /* Increase counter every mili-second */
    ms_count++;

    if (ms_count % 250 == 0)
    {
        /* Blink the GREEN LED every 250 ms (means blink at 2 Hz) */
        GPIO_Toggle(GPIOD, 5);
    }

    if (ms_count >= 500)
    {
        /* Blink the RED LED every 500 ms (means blink at 1 Hz) */
        GPIO_Toggle(GPIOE, 29);

        /* Reset counter when it reached max value (500) */
        ms_count = 0;
    }
}

int main (void)  {
    /* Return code of the functions */
    uint32_t rc;

    /* Setup GPIO for GREEN LED and RED LED */
    GPIO_Init(PORT_D, 5, OUTPUT);
    GPIO_Init(PORT_E, 29, OUTPUT);

    /* Configure SysTick to generate an interrupt every millisecond */
    rc = SysTick_Config(SystemCoreClock / 1000);

    if (rc != 0)
    {
        /* Failed to configure SysTick => blink RED and GREEN LED */
        GPIO_Toggle(GPIOE, 29);
        GPIO_Toggle(GPIOD, 5);
    }

    while(1);
}
