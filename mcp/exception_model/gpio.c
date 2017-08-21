#include "MKL46Z4.h"
#include "gpio.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

void GPIO_Init(uint8_t port, uint8_t pin, uint8_t dir)
{
    PORT_Type *portType[] = {PORTA, PORTB, PORTC, PORTD, PORTE};
    GPIO_Type *gpioBase[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE};

    /* Enable clock */
    SIM->SCGC5 |= (1 << (port + 9));

    if (PORT_A < port && port > PORT_E)
    {
        /* Invalid port. TODO implement return value for this function */
        return;
    }

    /* Pin muxing */
    portType[port]->PCR[pin] &= ~PORT_PCR_MUX_MASK;
    portType[port]->PCR[pin] |= PORT_PCR_MUX(1);

    /* Configure direction */
    if (dir == OUTPUT)
    {
        gpioBase[port]->PDDR |= (1 << pin);
    }
    else
    {
        gpioBase[port]->PDDR &= ~(1 << pin);
        /* Enable pull-up resistor */
        portType[port]->PCR[pin] |= PORT_PCR_PE_MASK;
        portType[port]->PCR[pin] |= PORT_PCR_PS_MASK;
    }
}

void GPIO_Write(GPIO_Type * gpio, uint8_t pin, uint8_t value)
{
    if (value == 0)
    {
        gpio->PCOR |= (1 << pin);
    }
    else
    {
        gpio->PSOR |= (1 << pin);
    }
}

void GPIO_Toggle(GPIO_Type * gpio, uint8_t pin)
{
    gpio->PTOR |= (1 << pin);
}

uint8_t GPIO_Read(GPIO_Type * gpio, uint8_t pin)
{
    if (gpio->PDIR & (1 << pin))
    {
        return 1;
    }
    return 0;
}
