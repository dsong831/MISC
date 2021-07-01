/**
 *******************************************************************************
 * @file        a31t21x_fmc.c
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

#include "a31t21x_fmc.h"


#define FLASH_SIZE_128K             0x00020000
#define FLASH_SIZE_256K             0x00040000

#define FLASH_OPT_ADDRESS_Msk       0x0F000000
#define FLASH_OPT_SIZE              0x00000600

#define FLASH_TYPE_CODE             0

#define FLASH_ACCESS_FLASHMODE_KEY1	0x5A
#define FLASH_ACCESS_FLASHMODE_KEY2	0xA5
#define FLASH_ACCESS_TRIMMODE_KEY1	0xA5
#define FLASH_ACCESS_TRIMMODE_KEY2	0x5A
#define FLASH_ACCESS_AMBAMODE_KEY1	0x81
#define FLASH_ACCESS_AMBAMODE_KEY2	0x28
#define FLASH_ACCESS_PROTMODE_KEY1	0x66
#define FLASH_ACCESS_PROTMODE_KEY2	0x99

#define FLASH_CODE_ERASE_CTRL_Msk		((1 << 1) | (1 << 5) | (1 << 6) | (1 << 7))	// ERS | SECT1K | SECT4K | BULK

#define flash_code_busy_check(void)							\
    {																						\
        uint32_t timeout;												\
				timeout = FLASH_BUSY_BLOCKING_TIMEOUT;	\
				while (FMC->BUSY & 0x01)								\
				{																				\
						timeout--;													\
						if(timeout==0) break;								\
				}																				\
				FMC->BUSY = 0x80;												\
    }

#define VIO(addr)   (*(volatile uint32_t *)(addr))

uint32_t g_code_flash_size = FLASH_SIZE_256K;
enum flash_sector_size g_erase_code_sector_size = FLASH_SECTOR_SIZE_512B;


static void FLASH_CodeWrite(uint32_t flash_addr, uint8_t *data, uint32_t len)
{
    uint32_t *data32;
    uint32_t i;

  	/* All interrupts must be stopped when flash erase or write */
  	__disable_irq();

		/* Data Load */
    data32 = (uint32_t *)data;
#if 1
		/* Clear Write Protection */
		FMC->MR = FLASH_ACCESS_PROTMODE_KEY1;
		FMC->MR = FLASH_ACCESS_PROTMODE_KEY2;
		FMC->WPROT = 0;
		FMC->MR = 0; FMC->MR = 0;
#endif
		/* Set Flash Address */
		FMC->AR = flash_addr >> 2;		// Least 2 bits will be ignored (Auto Incremental after WADCK trigger)

		/* Config Flash Access Time */
		FMC->MR = FLASH_ACCESS_AMBAMODE_KEY1;
		FMC->MR = FLASH_ACCESS_AMBAMODE_KEY2;
		FMC->CFG = 0x78580300;	// 3-wait
		FMC->MR = 0x00; FMC->MR = 0x00;

		/* Enter Flash Mode */
		FMC->MR = FLASH_ACCESS_FLASHMODE_KEY1;
		FMC->MR = FLASH_ACCESS_FLASHMODE_KEY2;

		/* PGM Sequence Start */
		FMC->CR |= (1 << 4);					// <PMODE> Set
		FMC->CR |= (1 << 2);					// <PGM> Set

		for (i = 0; i < len; i += 4)
		{
				FMC->DR = *data32++;

				FMC->CR |= (1 << 3);			// <WADCK> Set
				FMC->CR |= (1 << 0);			// <HVEN> Set
				flash_code_busy_check();
				FMC->CR &= ~(1 << 0);			// <HVEN> Clear
		}

		FMC->CR &= ~(1 << 2);					// <PGM> Clear

		/* Exit Flash Mode */
		FMC->CR = 0;
		FMC->MR = 0; FMC->MR = 0;
}

static void FLASH_CodeErase(uint32_t flash_addr, enum flash_sector_size sector_size)
{
    uint32_t reg_val;
    uint32_t sector_type;

		/* All interrupts must be stopped when flash erase or write */
		__disable_irq();
#if 1
		/* Clear Write Protection */
		FMC->MR = FLASH_ACCESS_PROTMODE_KEY1;
		FMC->MR = FLASH_ACCESS_PROTMODE_KEY2;
		FMC->WPROT = 0;
		FMC->MR = 0; FMC->MR = 0;
#endif
		/* Set Flash Address */
		FMC->AR = flash_addr >> 2;		// Least 2 bits will be ignored (Auto Incremental after WADCK trigger)

		/* Config Flash Access Time */
		FMC->MR = FLASH_ACCESS_AMBAMODE_KEY1;
		FMC->MR = FLASH_ACCESS_AMBAMODE_KEY2;
		FMC->CFG = 0x78580300;	// 3-wait
		FMC->MR = 0x00; FMC->MR = 0x00;

		/* Enter Flash Mode */
		FMC->MR = FLASH_ACCESS_FLASHMODE_KEY1;
		FMC->MR = FLASH_ACCESS_FLASHMODE_KEY2;

		/* Check Sector Size */
    switch (sector_size)
    {
        case FLASH_SECTOR_SIZE_512B:
            sector_type = 0;
            break;
        case FLASH_SECTOR_SIZE_1KB:
            sector_type = (1 << 5);
            break;
        case FLASH_SECTOR_SIZE_4KB:
            sector_type = (1 << 6);
            break;
        default:
            DRIVER_ASSERT(0);
            break;
    }

    reg_val = FMC->CR;
    reg_val &= ~FLASH_CODE_ERASE_CTRL_Msk;
    reg_val |= sector_type;
    FMC->CR = reg_val;

		/* Erase Sequence Start */
		FMC->CR |= (1 << 4);					// <PMODE> Set
		FMC->CR |= (1 << 1);					// <ERS> Set
		FMC->CR |= (1 << 3);					// <WADCK> Set
		FMC->CR |= (1 << 0);					// <HVEN> Set

    flash_code_busy_check();

		FMC->CR &= ~(1 << 0);					// <HVEN> Clear
		FMC->CR &= ~(1 << 1);					// <ERS> Clear

		/* Exit Flash Mode */
		FMC->CR = 0;
		FMC->MR = 0; FMC->MR = 0;
}

static void FLASH_SelfWrite(uint32_t flash_addr, uint8_t *data, uint32_t len)
{
    uint32_t *data32;
    uint32_t i;

  	/* All interrupts must be stopped when flash erase or write */
  	__disable_irq();

		/* Data Load */
    data32 = (uint32_t *)data;
#if 1
		/* Clear Write Protection */
		FMC->MR = FLASH_ACCESS_PROTMODE_KEY1;
		FMC->MR = FLASH_ACCESS_PROTMODE_KEY2;
		FMC->WPROT = 0;
		FMC->MR = 0; FMC->MR = 0;
#endif
		/* Config Flash Access Time */
		FMC->MR = FLASH_ACCESS_AMBAMODE_KEY1;
		FMC->MR = FLASH_ACCESS_AMBAMODE_KEY2;
		FMC->CFG = 0x78580300;	// 3-wait
		FMC->MR = 0x00; FMC->MR = 0x00;

		/* Enter Flash Mode */
		FMC->MR = FLASH_ACCESS_FLASHMODE_KEY1;
		FMC->MR = FLASH_ACCESS_FLASHMODE_KEY2;

		/* PGM Sequence Start */
		FMC->CR |= (1 << 23);					// <SELFPGM> Set
		FMC->CR |= (1 << 2);					// <PGM> Set
		__nop(); __nop(); __nop(); __nop(); __nop();

		for (i = 0; i < len; i += 4)
		{
				VIO(flash_addr) = *data32++;
				flash_addr += 4;
		}
		__nop(); __nop(); __nop(); __nop(); __nop();

		FMC->CR &= ~(1 << 2);					// <PGM> Clear
		FMC->CR &= ~(1 << 23);				// <SELFPGM> Clear

		/* Exit Flash Mode */
		FMC->CR = 0;
		FMC->MR = 0; FMC->MR = 0;
}

static void FLASH_SelfErase(uint32_t flash_addr, enum flash_sector_size sector_size)
{
    uint32_t reg_val;
    uint32_t sector_type;

		/* All interrupts must be stopped when flash erase or write */
		__disable_irq();
#if 1
		/* Clear Write Protection */
		FMC->MR = FLASH_ACCESS_PROTMODE_KEY1;
		FMC->MR = FLASH_ACCESS_PROTMODE_KEY2;
		FMC->WPROT = 0;
		FMC->MR = 0; FMC->MR = 0;
#endif
		/* Config Flash Access Time */
		FMC->MR = FLASH_ACCESS_AMBAMODE_KEY1;
		FMC->MR = FLASH_ACCESS_AMBAMODE_KEY2;
		FMC->CFG = 0x78580300;	// 3-wait
		FMC->MR = 0x00; FMC->MR = 0x00;

		/* Enter Flash Mode */
		FMC->MR = FLASH_ACCESS_FLASHMODE_KEY1;
		FMC->MR = FLASH_ACCESS_FLASHMODE_KEY2;

		/* Check Sector Size */
    switch (sector_size)
    {
        case FLASH_SECTOR_SIZE_512B:
            sector_type = 0;
            break;
        case FLASH_SECTOR_SIZE_1KB:
            sector_type = (1 << 5);
            break;
        case FLASH_SECTOR_SIZE_4KB:
            sector_type = (1 << 6);
            break;
        default:
            DRIVER_ASSERT(0);
            break;
    }

    reg_val = FMC->CR;
    reg_val &= ~FLASH_CODE_ERASE_CTRL_Msk;
    reg_val |= sector_type;
    FMC->CR = reg_val;

		/* Erase Sequence Start */
		FMC->CR |= (1 << 23);					// <SELFPGM> Set
		FMC->CR |= (1 << 1);					// <ERS> Set
		__nop(); __nop(); __nop(); __nop(); __nop();

		VIO(flash_addr) = 0xFFFFFFFF;
		__nop(); __nop(); __nop(); __nop(); __nop();

		FMC->CR &= ~(1 << 1);					// <ERS> Clear
		FMC->CR &= ~(1 << 23);				// <SELFPGM> Clear

		/* Exit Flash Mode */
		FMC->CR = 0;
		FMC->MR = 0; FMC->MR = 0;
}

static uint32_t FLASH_GetEraseSize(enum flash_sector_size sector_size)
{
    uint32_t erase_len = 512;

    switch (sector_size)
    {
        case FLASH_SECTOR_SIZE_512B:
            erase_len = 512;
            break;
        case FLASH_SECTOR_SIZE_1KB:
            erase_len = 1024;
            break;
        case FLASH_SECTOR_SIZE_4KB:
            erase_len = 4096;
            break;
        default:
            erase_len = 512;
            break;
    }

    return erase_len;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_FLASH_Init(void)
{
    if (SCUCC->CHIPID & 0x01)
    {
        g_code_flash_size = FLASH_SIZE_256K;
    }
    else
    {
        g_code_flash_size = FLASH_SIZE_128K;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_FLASH_Read(uint32_t flash_addr, uint8_t *data, uint32_t len)
{
    if (flash_addr + len > g_code_flash_size)
    {
        return DRIVER_ERROR_PARAMETER;
    }

    memcpy(data, (void *)flash_addr, len);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_FLASH_Write(uint32_t flash_addr, uint8_t *data, uint32_t len)
{
    int32_t ret = DRIVER_ERROR_OK;

    DRIVER_ASSERT((flash_addr & 0x03) == 0 && (len & 0x03) == 0);

    if (flash_addr + len > g_code_flash_size)
    {
        return DRIVER_ERROR_PARAMETER;
    }

		FLASH_CodeWrite(flash_addr, data, len);

    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_FLASH_Erase(uint32_t flash_addr)
{
    int32_t ret = DRIVER_ERROR_OK;
    uint32_t erase_len = 512;

		erase_len = FLASH_GetEraseSize(g_erase_code_sector_size);

		if (flash_addr + erase_len > g_code_flash_size)
		{
				return DRIVER_ERROR_PARAMETER;
		}

		FLASH_CodeErase(flash_addr, g_erase_code_sector_size);

    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_FLASH_SelfWrite(uint32_t flash_addr, uint8_t *data, uint32_t len)
{
    int32_t ret = DRIVER_ERROR_OK;

    DRIVER_ASSERT((flash_addr & 0x03) == 0 && (len & 0x03) == 0);

    if (flash_addr + len > g_code_flash_size)
    {
        return DRIVER_ERROR_PARAMETER;
    }

		FLASH_SelfWrite(flash_addr, data, len);

    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_FLASH_SelfErase(uint32_t flash_addr)
{
    int32_t ret = DRIVER_ERROR_OK;
    uint32_t erase_len = 512;

		erase_len = FLASH_GetEraseSize(g_erase_code_sector_size);

		if (flash_addr + erase_len > g_code_flash_size)
		{
				return DRIVER_ERROR_PARAMETER;
		}

		FLASH_SelfErase(flash_addr, g_erase_code_sector_size);

    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_FLASH_OTP_Read(uint32_t otp_addr, uint8_t *data, uint32_t len)
{
    if ((otp_addr & ~(FLASH_OPT_ADDRESS_Msk)) + len > FLASH_OPT_SIZE)
    {
        return DRIVER_ERROR_PARAMETER;
    }

    memcpy(data, (void *)otp_addr, len);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_FLASH_SetEraseSecotorSize(enum flash_area area, enum flash_sector_size sector_size)
{
    if (area == FLASH_AREA_CODE)
    {
        g_erase_code_sector_size = sector_size;
    }
    else
    {
        g_erase_code_sector_size = FLASH_SECTOR_SIZE_512B;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_FLASH_SetWriteProtect8KCodeArea(enum flash_prot8k_code_sector sector, uint8_t enable)
{
    if (enable)
    {
				FMC->MR = FLASH_ACCESS_PROTMODE_KEY1;
				FMC->MR = FLASH_ACCESS_PROTMODE_KEY2;
        FMC->WPROT |= sector;
				FMC->MR = 0; FMC->MR = 0;
    }
    else
    {
				FMC->MR = FLASH_ACCESS_PROTMODE_KEY1;
				FMC->MR = FLASH_ACCESS_PROTMODE_KEY2;
        FMC->WPROT &= ~(sector);
				FMC->MR = 0; FMC->MR = 0;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_FLASH_SetReadProtectCodeArea(enum flash_read_prot_lvl prot_lvl)
{
    switch (prot_lvl)
    {
        case FLASH_READ_PROT_LVL_UNPROTECT:
            FMC->LOCK = 0xFF;
            break;
        case FLASH_READ_PROT_LVL_1:
						FMC->MR = FLASH_ACCESS_PROTMODE_KEY1;
						FMC->MR = FLASH_ACCESS_PROTMODE_KEY2;
            FMC->LOCK = 0x5A;
						FMC->MR = 0; FMC->MR = 0;
            break;
        default:
            break;
    }

    return DRIVER_ERROR_OK;
}

