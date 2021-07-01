/**
 *******************************************************************************
 * @file        a31t21x_fmc.h
 * @brief       FLASH driver
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

/**
 * @addtogroup  driver
 * @{
 * @defgroup    driver_flash FLASH
 * @{
 * @brief       FLASH driver
 */

#ifndef _A31T21X_FMC_H_
#define _A31T21X_FMC_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"


/** Flash write busy  time-out definitions in case of using function
 * with Blocking Flag mode
 */
#define	FLASH_BUSY_BLOCKING_TIMEOUT			(0x35000UL)	


/**
 * flash area type
 */
enum flash_area
{
    FLASH_AREA_CODE = 0,                    /**< flash code area, used for instruction and data */
};

/**
 * flash section size
 */
enum flash_sector_size
{
    FLASH_SECTOR_SIZE_512B  = 0,            /**< sector size as 512 byte */
    FLASH_SECTOR_SIZE_1KB   = 1,            /**< sector size as 1024 byte */
    FLASH_SECTOR_SIZE_4KB   = 2,            /**< sector size as 4096 byte */
};

/**
 * flash protection for 8k byte code area
 */
enum flash_prot8k_code_sector
{
    FLASH_PROT8K_CODE_SECTOR0		= BIT(0),   /**< 0x0000_0000 ~ 0x0000_1FFF protect */
    FLASH_PROT8K_CODE_SECTOR1		= BIT(1),   /**< 0x0000_2000 ~ 0x0000_3FFF protect */
    FLASH_PROT8K_CODE_SECTOR2		= BIT(2),   /**< 0x0000_4000 ~ 0x0000_5FFF protect */
    FLASH_PROT8K_CODE_SECTOR3		= BIT(3),   /**< 0x0000_6000 ~ 0x0000_7FFF protect */
    FLASH_PROT8K_CODE_SECTOR4		= BIT(4),   /**< 0x0000_8000 ~ 0x0000_9FFF protect */
    FLASH_PROT8K_CODE_SECTOR5		= BIT(5),   /**< 0x0000_A000 ~ 0x0000_BFFF protect */
    FLASH_PROT8K_CODE_SECTOR6		= BIT(6),   /**< 0x0000_C000 ~ 0x0000_DFFF protect */
    FLASH_PROT8K_CODE_SECTOR7		= BIT(7),   /**< 0x0000_E000 ~ 0x0000_FFFF protect */
    FLASH_PROT8K_CODE_SECTOR8		= BIT(8),   /**< 0x0001_0000 ~ 0x0001_1FFF protect */
    FLASH_PROT8K_CODE_SECTOR9		= BIT(9),		/**< 0x0001_2000 ~ 0x0001_3FFF protect */
    FLASH_PROT8K_CODE_SECTOR10  = BIT(10),  /**< 0x0001_4000 ~ 0x0001_5FFF protect */
    FLASH_PROT8K_CODE_SECTOR11  = BIT(11),  /**< 0x0001_6000 ~ 0x0001_7FFF protect */
    FLASH_PROT8K_CODE_SECTOR12  = BIT(12),  /**< 0x0001_8000 ~ 0x0001_9FFF protect */
    FLASH_PROT8K_CODE_SECTOR13  = BIT(13),  /**< 0x0001_A000 ~ 0x0001_BFFF protect */
    FLASH_PROT8K_CODE_SECTOR14  = BIT(14),  /**< 0x0001_C000 ~ 0x0001_DFFF protect */
    FLASH_PROT8K_CODE_SECTOR15  = BIT(15),  /**< 0x0001_E000 ~ 0x0001_FFFF protect */
    FLASH_PROT8K_CODE_SECTOR16  = BIT(16),  /**< 0x0002_0000 ~ 0x0002_1FFF protect */
    FLASH_PROT8K_CODE_SECTOR17  = BIT(17),  /**< 0x0002_2000 ~ 0x0002_3FFF protect */
    FLASH_PROT8K_CODE_SECTOR18  = BIT(18),  /**< 0x0002_4000 ~ 0x0002_5FFF protect */
    FLASH_PROT8K_CODE_SECTOR19  = BIT(19),  /**< 0x0002_6000 ~ 0x0002_7FFF protect */
    FLASH_PROT8K_CODE_SECTOR20  = BIT(20),  /**< 0x0002_8000 ~ 0x0002_9FFF protect */
    FLASH_PROT8K_CODE_SECTOR21  = BIT(21),  /**< 0x0002_A000 ~ 0x0002_BFFF protect */
    FLASH_PROT8K_CODE_SECTOR22  = BIT(22),  /**< 0x0002_C000 ~ 0x0002_DFFF protect */
    FLASH_PROT8K_CODE_SECTOR23  = BIT(23),  /**< 0x0002_E000 ~ 0x0002_FFFF protect */
    FLASH_PROT8K_CODE_SECTOR24  = BIT(24),  /**< 0x0003_0000 ~ 0x0003_1FFF protect */
    FLASH_PROT8K_CODE_SECTOR25  = BIT(25),  /**< 0x0003_2000 ~ 0x0003_3FFF protect */
    FLASH_PROT8K_CODE_SECTOR26  = BIT(26),  /**< 0x0003_4000 ~ 0x0003_5FFF protect */
    FLASH_PROT8K_CODE_SECTOR27  = BIT(27),  /**< 0x0003_6000 ~ 0x0003_7FFF protect */
    FLASH_PROT8K_CODE_SECTOR28  = BIT(28),  /**< 0x0003_8000 ~ 0x0003_9FFF protect */
    FLASH_PROT8K_CODE_SECTOR29  = BIT(29),  /**< 0x0003_A000 ~ 0x0003_BFFF protect */
    FLASH_PROT8K_CODE_SECTOR30  = BIT(30),  /**< 0x0003_C000 ~ 0x0003_DFFF protect */
//    FLASH_PROT8K_CODE_SECTOR31  = BIT(31),  /**< 0x0003_E000 ~ 0x0003_FFFF protect */
};

/**
 * flash protection level
 */
enum flash_read_prot_lvl
{
    FLASH_READ_PROT_LVL_UNPROTECT   = 0,    /**< flash protection none */
    FLASH_READ_PROT_LVL_1           = 1,    /**< flash protection leve 1 */
};

/**
 *******************************************************************************
 * @brief       Flash initalize
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_Init(void);

/**
 *******************************************************************************
 * @brief       Read flash memory.
 * @param[in]   flash_addr address of the memory to read
 * @param[out]  data pointer to the memory the data is to be copied to.
 * @param[in]   len length of the memory to read
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_Read(uint32_t flash_addr, uint8_t *data, uint32_t len);

/**
 *******************************************************************************
 * @brief       Write flash memory.
 *              The memory must be erased first to write new data.
 *              - memory address can be byte aligned.
 * @param[in]   flash_addr address of the memory to read
 * @param[in]   data pointer to the memory the data is to be copied from.
 * @param[in]   len length of the memory to write
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_Write(uint32_t flash_addr, uint8_t *data, uint32_t len);

/**
 *******************************************************************************
 * @brief       Erase flash memory. The whole sector at the
 *              specified memory will be erased.
 * @param[in]   flash_addr address of the memory to erase
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_Erase(uint32_t flash_addr);

/**
 *******************************************************************************
 * @brief       Self Write flash memory.
 *              The memory must be erased first to write new data.
 *              - memory address can be byte aligned.
 * @param[in]   flash_addr address of the memory to read
 * @param[in]   data pointer to the memory the data is to be copied from.
 * @param[in]   len length of the memory to write
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_SelfWrite(uint32_t flash_addr, uint8_t *data, uint32_t len);

/**
 *******************************************************************************
 * @brief       Self Erase flash memory. The whole sector at the
 *              specified memory will be erased.
 * @param[in]   flash_addr address of the memory to erase
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_SelfErase(uint32_t flash_addr);

/**
 *******************************************************************************
 * @brief       Read from OTP flash memory.
 * @param[in]   otp_addr address of the memory to read
 * @param[in]   data pointer to the memory the data is to be copied to.
 * @param[in]   len length of the memory to read
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_OTP_Read(uint32_t otp_addr, uint8_t *data, uint32_t len);

/**
 *******************************************************************************
 * @brief       This function set flash erase sector size.
 * @param[in]   area flash area (::flash_area)
 * @param[in]   sector_size flash sector size (::flash_sector_size)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_SetEraseSecotorSize(enum flash_area area, enum flash_sector_size sector_size);

/**
 *******************************************************************************
 * @brief       This function enable write/erase protect of code area by 8kb unit.
 * @param[in]   sector protection sector (::flash_prot8k_code_sector)
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_SetWriteProtect8KCodeArea(enum flash_prot8k_code_sector sector, uint8_t enable);

/**
 *******************************************************************************
 * @brief       This function select read protection level in debug mode.
 * @param[in]   prot_lvl protection level (::flash_read_prot_lvl)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_FLASH_SetReadProtectCodeArea(enum flash_read_prot_lvl prot_lvl);


#endif /* _A31T21X_FMC_H_ */

/** @} */
/** @} */

