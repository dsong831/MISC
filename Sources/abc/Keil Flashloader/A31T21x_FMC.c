/***************************************************************************//**
*   @file					A31T21x_FMC.c
*   @brief				Flash memory controller
*   @author		ABOV Semiconductor Co., Ltd.
*   @version		V1.00
*   @date				29. May. 2020
*
* Copyright(C) 2020, ABOV Semiconductor
* All rights reserved.
*
********************************************************************************
* DISCLAIMER 
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, ABOV SEMICONDUCTOR DISCLAIMS ALL LIABILITIES FROM ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
*******************************************************************************/

/*******************************************************************************
* Included File
*******************************************************************************/
#include "A31G21x.h"
#include "A31T21x_FMC.h"
/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/
#define	BV_SELFPGM		(1<<23)
#define	BV_IFEN						(1<<12)
#define	BV_BBLOCK			(1<<8)
#define	BV_MAS						(1<<7)
#define	BV_SECT4K			(1<<6)
#define	BV_SECT1K			(1<<5)
#define	BV_PMOD				(1<<4)
#define	BV_WADCK			(1<<3)
#define	BV_PGM					(1<<2)
#define	BV_ERS						(1<<1)
#define	BV_HVEN					(1<<0)

#define	FMMR_BUSY								(FMC->BUSY & 0x01)
#define	FMCR_SELFPGM_SET	(FMC->CR |= BV_SELFPGM)
#define	FMCR_SELFPGM_CLR	(FMC->CR &= ~BV_SELFPGM)
#define	FMCR_IFEN_SET					(FMC->CR |= BV_IFEN)
#define	FMCR_IFEN_CLR					(FMC->CR &= ~BV_IFEN)
#define	FMCR_BBLOCK_SET		(FMC->CR |= BV_BBLOCK)
#define	FMCR_BBLOCK_CLR		(FMC->CR &= ~BV_BBLOCK)
#define	FMCR_MAS_SET					(FMC->CR |= BV_MAS)
#define	FMCR_MAS_CLR					(FMC->CR &= ~BV_MAS)
#define	FMCR_SECT4K_SET			(FMC->CR |= BV_SECT4K)
#define	FMCR_SECT4K_CLR			(FMC->CR &= ~BV_SECT4K)
#define	FMCR_SECT1K_SET			(FMC->CR |= BV_SECT1K)
#define	FMCR_SECT1K_CLR			(FMC->CR &= ~BV_SECT1K)
#define	FMCR_PMOD_SET				(FMC->CR |= BV_PMOD)
#define	FMCR_PMOD_CLR				(FMC->CR &= ~BV_PMOD)
#define	FMCR_WADCK_SET			(FMC->CR |= BV_WADCK)
#define	FMCR_WADCK_CLR			(FMC->CR &= ~BV_WADCK)
#define	FMCR_PGM_SET					(FMC->CR |= BV_PGM)
#define	FMCR_PGM_CLR					(FMC->CR &= ~BV_PGM)
#define	FMCR_ERS_SET						(FMC->CR |= BV_ERS)
#define	FMCR_ERS_CLR						(FMC->CR &= ~BV_ERS)
#define	FMCR_HVEN_SET					(FMC->CR |= BV_HVEN)
#define	FMCR_HVEN_CLR					(FMC->CR &= ~BV_HVEN)
/*******************************************************************************
* Private Typedef
*******************************************************************************/

/*******************************************************************************
* Private Variable
*******************************************************************************/

/*******************************************************************************
* Private Function Prototype
*******************************************************************************/

/*******************************************************************************
* Public Function
*******************************************************************************/
/**
* @brief			Clear Chip Write Protection
* @param	None
* @return		None
*/
void ClearWriteProtection(void)
{
	// PROT mode entry
	FMC->MR = 0x66;
	FMC->MR = 0x99;
	FMC->WPROT = 0x00000000;
	FMC->MR = 0x00;
	FMC->MR = 0x00;
}

/**
* @brief			Configure Flash Access Time
* @param	None
* @return		None
*/
void ConfigFlashAccessTime(void)
{
	// AMBA mode entry
	FMC->MR = 0x81;
	FMC->MR = 0x28;
	FMC->CFG = 0x78580300;	// 3-wait
	FMC->MR = 0x00;
	FMC->MR = 0x00;
}

/**
* @brief			Enter Flash Mode
* @param	None
* @return		None
*/
void EnterFlashMode(void)
{
	// Flash mode entry
	FMC->MR = 0x5A;
	FMC->MR = 0xA5;
}

/**
* @brief			Exit Flash Mode
* @param	None
* @return		None
*/
void ExitFlashMode(void)
{
	FMC->CR = 0x00000000;
	FMC->MR = 0x00;
	FMC->MR = 0x00;
}

/**
* @brief			Check Flash Busy Status
* @param	None
* @return		None
*/
void CheckFlashBusyStatus(void)
{
	while(1)
	{
		// Check WRBUSY flag of FMC_BUSY register (1: Busy / 0: Complete)
		if(FMMR_BUSY == 0)
		{
			break;
		}
	}
	FMC->BUSY = 0x80;	// WRBUSY flag clear
}

/**
* @brief			Set User OTP Area
* @param	None
* @return		None
*/
void SetUserOTPArea(unsigned long otp_idx)
{
	EnterFlashMode();
	// Disable User OTP Area
	if(otp_idx == 0)
	{
		FMC->CR = 0;
	}
	// Enable User OTP Area
	else
	{
		FMCR_IFEN_SET;
		FMCR_SECT4K_CLR;
		FMCR_SECT1K_CLR;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

/**
* @brief			Bulk Erase
* @param	None
* @return		0: success / 1: fail
*/
int EraseChip(void)
{
	ClearWriteProtection();
	ConfigFlashAccessTime();
	EnterFlashMode();

	FMCR_PMOD_SET;
	FMCR_MAS_SET;
	FMCR_ERS_SET;
	FMCR_WADCK_SET; 
	FMCR_HVEN_SET; 

	CheckFlashBusyStatus();

	FMCR_HVEN_CLR;
	FMCR_ERS_CLR;
	FMCR_MAS_CLR;

	ExitFlashMode();
	ConfigFlashAccessTime();

	return (0);	// success
}

/**
* @brief			Page Erase (512Byte)
* @param	target_addr
* @return		0: success / 1: fail
*/
int ErasePage(unsigned long target_addr)
{
	ClearWriteProtection();

	FMC->AR = target_addr >> 2;		// least 2 bits will be ignored

	ConfigFlashAccessTime();
	EnterFlashMode();

	FMCR_PMOD_SET;
	FMCR_ERS_SET;
	FMCR_WADCK_SET;
	FMCR_HVEN_SET;
	
	CheckFlashBusyStatus();

	FMCR_HVEN_CLR;
	FMCR_ERS_CLR;	
	
	ExitFlashMode();
	ConfigFlashAccessTime();

	return (0);		// success
}

/**
* @brief			Sector Erase (1KByte)
* @param	target_addr
* @return		0: success / 1: fail
*/
int EraseSector1KB(unsigned long target_addr)
{
	ClearWriteProtection();
	
	FMC->AR = target_addr >> 2;		// least 2 bits will be ignored
	
	ConfigFlashAccessTime();
	EnterFlashMode();

	FMCR_PMOD_SET;
	FMCR_ERS_SET;
	FMCR_SECT1K_SET;
	FMCR_WADCK_SET;
	FMCR_HVEN_SET;

	CheckFlashBusyStatus();
	
	FMCR_HVEN_CLR;
	FMCR_SECT1K_CLR;
	FMCR_ERS_CLR;	

	ExitFlashMode();
	ConfigFlashAccessTime();

	return (0);		// success
}

/**
* @brief			Sector Erase (4KByte)
* @param	target_addr
* @return		0: success / 1: fail
*/
int EraseSector4KB(unsigned long target_addr)
{
	ClearWriteProtection();
	
	FMC->AR = target_addr >> 2;		// least 2 bits will be ignored
	
	ConfigFlashAccessTime();
	EnterFlashMode();
	
	FMCR_PMOD_SET;
	FMCR_ERS_SET;
	FMCR_SECT4K_SET;
	FMCR_WADCK_SET;
	FMCR_HVEN_SET;
	
	CheckFlashBusyStatus();
	
	FMCR_HVEN_CLR;
	FMCR_SECT4K_CLR;
	FMCR_ERS_CLR;	

	ExitFlashMode();
	ConfigFlashAccessTime();

	return (0);		// success
}

/**
* @brief			Program Page (512Byte)
* @param	target_addr
* @param	page_size
* @param	data_buffer[512]
* @return		0: success / 1: fail
*/
int ProgramPage(unsigned long target_addr, unsigned long page_size, unsigned char *data_buffer)
{
	int	i;
	unsigned long *ptr;

	/* Check address and size alignment */
	if((target_addr & 0x03) || (page_size & 0x03))
	{
		return (1);		// fail
	}

	/* Data Load */
	ptr = (unsigned long *)data_buffer;

	/* PGM Sequence Start */
	ClearWriteProtection();
	
	FMC->AR = target_addr >> 2;		// least 2 bits will be ignored
	
	ConfigFlashAccessTime();
	EnterFlashMode();

	FMCR_PMOD_SET;
	FMCR_PGM_SET;

	for(i=0; i<page_size; i+=4)
	{
		FMC->DR = *ptr++;

		FMCR_WADCK_SET;
		FMCR_HVEN_SET;
		CheckFlashBusyStatus();
		FMCR_HVEN_CLR;
	}
	
	FMCR_PGM_CLR;
	
	ExitFlashMode();
	ConfigFlashAccessTime();

	return (0);		// success
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

/************************ (C) COPYRIGHT ABOV SEMICONDUCTOR *****END OF FILE****/
