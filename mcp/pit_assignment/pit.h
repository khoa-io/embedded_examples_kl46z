#ifndef _PIT_H_
#define _PIT_H_

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Value of TFL register show that timeout has occurred */
#define PIT_TFLG_OCCURRED (1)

/* Value of TFL register show that timeout has not yet occurred */
#define PIT_TFLG_NOT_OCCURRED (0)

/*******************************************************************************
 * Structures and types
 ******************************************************************************/

/*!
 * @brief Configuration of a PIT timer channel.
 */
struct pit_chnl_conf
{
    /* Timeout period (in ms) for the timer interrupt */
    uint32_t time;

    /* Call when reached timeout */
    void (*handler)(void);

    /* Chain mode enable */
    bool chn;
};

typedef struct pit_chnl_conf pit_chnl_conf_t;

/*******************************************************************************
 * APIs
 ******************************************************************************/

/*!
 * @brief Handle PIT_IRQ interrupt.
 */
void PIT_IRQHandler(void);

/*!
 * @brief Enable PIT.
 */
void PIT_enable();

/*!
 * @brief Configure a pit channel.
 *
 * @param idx  Channel's index.
 * @param conf Channel's configuration.
 */
void PIT_configChannel(uint8_t idx, pit_chnl_conf_t *conf);

/*!
 * @brief Start all timers.
 */
void PIT_startAll();

/*!
 * @brief Start a timer channel.
 *
 * @param idx Channel's index.
 */
void PIT_startChannel(uint8_t idx);

/*!
 * @brief Stop all timers.
 */
void PIT_stopAll();

/*!
 * @brief Stop a timer channel.
 *
 * @param idx Channel's index.
 */
void PIT_stopChannel(uint8_t idx);

#endif /* _PIT_H_ */