#ifndef _LOADER_H_
#define _LOADER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Starting address of the application */
#define APP_START_ADDR (0x0000A000U)

#define APP_SP (*((uint32_t *)APP_START_ADDR))

/* Starting address of the application's reset handler */
#define APP_ENTRY_ADDR (0x0000A004U)

/* (Maximum) Size of the application in sectors (256 - 0xA000/1024) */
#define APP_SIZE (216U)

/* Entry point function pointer type */
typedef void (*entry_point_t)(void);

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Prepare for loading new application
 *
 * @return Return true if success.
 */
bool LOADER_preload();

/*!
 * @brief Mark as no app
 */
void LOADER_resetState();

/*!
 * @brief Write a parsed SREC line into flash memory.
 *
 * @param dat Point to parsed data.
 *
 * @return Return the number of wrote bytes.
 */
uint8_t LOADER_write(parsed_dat_t *dat);

/*!
 * @brief Run the loaded application.
 *
 * @return Return true if success.
 */
void LOADER_runApp();

#endif /* _LOADER_H_ */
