#include <stddef.h>

#include "MKL46Z4.h"
#include "pit.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
/*!
 * @brief Calculate value for LDVAL register from period (ms).
 *
 * @param T Period (ms).
 *
 * @return Value for LDVAL register.
 */
#define LDVAL_GET_COUNT(T) (((T) / 1000) * SystemCoreClock / 2 - 1)

/*******************************************************************************
 * Global variables
 ******************************************************************************/

/* Number of PIT channels */
extern uint8_t g_pitChnlNum;

extern void (*g_pitIrqHandler[])(void);

/* Default system clock value */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 * Code
 ******************************************************************************/

void PIT_IRQHandler(void)
{
    /* Channel index */
    uint8_t i = 0;

    for (i = 0; i < g_pitChnlNum; ++i)
    {
        if (PIT->CHANNEL[i].TFLG != PIT_TFLG_OCCURRED)
        {
            /* Channel i: timeout has not yet occurred */
            continue;
        }

        /* Channel i: timeout has occurred */
        /* Reset PIT interrupt flag */
        PIT->CHANNEL[i].TFLG = 1;

        /* Call handler if there is one */
        if (g_pitIrqHandler[i] != NULL)
        {
            g_pitIrqHandler[i]();
        }
    }
}

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

void PIT_startAll()
{
    /* Channel's index */
    uint8_t i = 0;
    for (i = 0; i < g_pitChnlNum; ++i)
    {
        PIT->CHANNEL[i].TCTRL |= PIT_TCTRL_TEN_MASK;
    }
}

void PIT_startChannel(uint8_t idx)
{
    PIT->CHANNEL[idx].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void PIT_stopAll()
{
    /* Channel's index */
    uint8_t i = 0;
    for (i = 0; i < g_pitChnlNum; ++i)
    {
        PIT->CHANNEL[i].TCTRL &= ~PIT_TCTRL_TEN_MASK;
    }
}

void PIT_stopChannel(uint8_t idx)
{
    PIT->CHANNEL[idx].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}

void PIT_configChannel(uint8_t idx, pit_chnl_conf_t *conf)
{
    g_pitIrqHandler[idx] = conf->handler;

    PIT->CHANNEL[idx].LDVAL = LDVAL_GET_COUNT(conf->time);

    /* Set value for TCTRL register */
    PIT->CHANNEL[idx].TCTRL = 0;

    if (conf->handler)
    {
        /* Enable interrupt handler */
        PIT->CHANNEL[idx].TCTRL |= PIT_TCTRL_TIE_MASK;
    }

    if (conf->chn)
    {
        /* Enable chain mode */
        PIT->CHANNEL[idx].TCTRL |= PIT_TCTRL_CHN_MASK;
    }
}