#include <stdint.h>
#include <stddef.h>

#include "board.h"
#include "srec_reader.h"
#include "Flash.h"
#include "loader.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

bool LOADER_preload()
{
    return Erase_Multi_Sector(APP_START_ADDR, APP_SIZE);
}

void LOADER_runApp()
{
    SCB->VTOR = APP_START_ADDR;

    /* Entry point of the application */
    void (*app)(void) = (void (*)(void))(*((uint32_t *)0x0000A004U));

    app();
}

uint8_t LOADER_write(parsed_dat_t *dat)
{
    /* Indexing variable */
    uint8_t i = 0;

    if (dat == NULL)
    {
        return i;
    }

    for (; i < dat->dataLength; i += 4)
    {
        if (!Program_LongWord_8B(dat->address + i, dat->data + i))
        {
            break;
        }
    }

    /* Return number of wrote bytes */
    return i;
}
