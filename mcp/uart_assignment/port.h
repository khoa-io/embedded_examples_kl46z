#ifndef _PORT_H_
#define _PORT_H_

/*******************************************************************************
 * APIs
 ******************************************************************************/
/*!
 * @brief Clears the individual pin's ISF.
 *
 * @param base  Port base pointer.
 * @param pin   Port pin number.
 */
void PORT_clear_pin_isf(PORT_Type *base, uint32_t pin);

/*!
 * @brief Reads the individual pin's ISF.
 *
 * @param base  Port base pointer.
 * @param pin   Port pin number.
 *
 * @return Return:
 *         - 0: interrupt is not detected.
 *         - 1: interrupt is detected.
 */
uint32_t PORT_get_pin_isf(PORT_Type *base, uint32_t pin);

#endif /* _PORT_H_ */
