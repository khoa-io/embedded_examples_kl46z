#include "MKL46Z4.h"
#include "port.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

void PORT_clear_pin_isf(PORT_Type * base, uint32_t pin)
{
    base->PCR[pin] |= PORT_PCR_ISF_MASK;
}

uint32_t PORT_get_pin_isf(PORT_Type * base, uint32_t pin)
{
    return base->PCR[pin] & PORT_PCR_ISF_MASK;
}
