#include "MKL46Z4.h"
#include "pit.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

void PIT_enable()
{
    /* Enable clock */
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

    /* Enable interrupt */
    NVIC_ClearPendingIRQ(PIT_IRQn);
    NVIC_EnableIRQ(PIT_IRQn);
    NVIC_SetPriority(PIT_IRQn, 0);

    /* Turn on PIT */
    PIT->MCR = 0x00;
}