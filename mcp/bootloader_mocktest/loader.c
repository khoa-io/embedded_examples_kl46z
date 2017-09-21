#include <stdint.h>
#include <stddef.h>

#include "MKL46Z4.h"
#include "board.h"
#include "srec_reader.h"
#include "Flash.h"
#include "loader.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

bool LOADER_preload()
{
    uint8_t rc = 0;

    __disable_irq();
    rc = Erase_Multi_Sector(APP_START_ADDR, APP_SIZE);
    __enable_irq();

    return rc;
}

void LOADER_resetState()
{
    __disable_irq();
    Erase_Multi_Sector(APP_START_ADDR, 1);
    __enable_irq();
}

void LOADER_runApp()
{
    entry_point_t appResetHandler = NULL;

    /* Re-locate vector table for the app */
    SCB->VTOR = APP_START_ADDR;

    /* Change main SP with value of the app's vector table offset 0 */
    __set_MSP(APP_SP);

    /* Entry point of the application. The appResetHandler will copy vector
     * table to RAM so we don't have to do it */
    appResetHandler = (entry_point_t)(Read_FlashAddress(APP_ENTRY_ADDR));

    /* Start the app */
    appResetHandler();
}

uint8_t LOADER_write(parsed_dat_t *dat)
{
    /* Indexing variable */
    uint8_t i = 0;

    if (dat == NULL)
    {
        return i;
    }

    __disable_irq();
    for (; i < dat->dataLength; i += 4)
    {
        if (!Program_LongWord_8B(dat->address + i, dat->data + i))
        {
            break;
        }
    }
    __enable_irq();

    /* Return number of wrote bytes */
    return i;
}
