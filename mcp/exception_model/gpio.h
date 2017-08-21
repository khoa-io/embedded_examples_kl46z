
#ifndef _GPIO_H_
#define _GPIO_H_

#include "MKL46Z4.h"

/*******************************************************************************
 * I/O direction flags
 ******************************************************************************/

#define OUTPUT      1
#define INPUT       0

/*******************************************************************************
 * Port numbers in order
 ******************************************************************************/

#define PORT_A      0
#define PORT_B      1
#define PORT_C      2
#define PORT_D      3
#define PORT_E      4

/*******************************************************************************
 * APIs
 ******************************************************************************/

/*!
 * @brief Initialize registers for GPIO.
 *
 * @param port One of PORT_A, PORT_B, PORT_C, PORT_D, PORT_E.
 * @param pin  Read FRDM-KL46Z User's Manual for pin.
 * @param dir  I/O direction. One of INPUT or OUTPUT
 */
void GPIO_Init(uint8_t port, uint8_t pin, uint8_t dir);

/*!
 * @brief Write value for GPIO.
 *
 * @param gpio  One of GPIOA, GPIOB, GPIOC, GPIOC, GPIOD, GPIOE.
 * @param pin   Read FRDM-KL46Z User's Manual for pin.
 * @param value The value to write.
 */
void GPIO_Write(GPIO_Type * gpio, uint8_t pin, uint8_t value);

/*!
 * @brief Toggle value for GPIO.
 *
 * @param gpio  One of GPIOA, GPIOB, GPIOC, GPIOC, GPIOD, GPIOE.
 * @param pin   Read FRDM-KL46Z User's Manual for pin.
 */
void GPIO_Toggle(GPIO_Type * gpio, uint8_t pin);

/*!
 * @brief Get value for GPIO.
 *
 * @param gpio  One of GPIOA, GPIOB, GPIOC, GPIOC, GPIOD, GPIOE.
 * @param pin   Read FRDM-KL46Z User's Manual for pin.
 *
 * @return The read value.
 */
uint8_t GPIO_Read(GPIO_Type * gpio, uint8_t pin);

#endif /* _GPIO_H_ */