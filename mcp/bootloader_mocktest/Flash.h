#if !defined (__FLASH_H__)
#define __FLASH_H__

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define CMD_PROGRAM_LONGWORD     (0x06)
#define CMD_ERASE_FLASH_SECTOR   (0x09)

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief
 * get address
 * @param Addr: input address
 * @return
 * return address
 */
uint32_t Read_FlashAddress(uint32_t Addr);

/*!
 * @brief
 * flash data input into flash
 * @param Addr: address to flash data to flash
 * @param *Data: input data need to flash data into flash
 * @return
 * return 1: if success
 */
uint8_t Program_LongWord_8B(uint32_t Addr,uint8_t *Data);

/*!
 * @brief
 * flash data input into flash
 * @param Addr: address to flash data to flash
 * @param Data: input data 32 bits need to flash data into flash
 * @return
 * return 1: if success
 */
uint8_t Program_LongWord(uint32_t Addr,uint32_t Data);

/*!
 * @brief
 * erase a sector in flash
 * @param Addr: address to erase
 * @return
 * return 1: if success
 */
uint8_t Erase_Sector(uint32_t Addr);

/*!
 * @brief
 * erase multi sectors in flash
 * @param Addr: address to erase
 * @return
 * return 1: if success
 */
uint8_t Erase_Multi_Sector(uint32_t Addr,uint8_t Size);

#endif
