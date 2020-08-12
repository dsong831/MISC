/**********************************************************************//**
* @file				A31T21x_hal_fmc.c
* @brief			Contains all functions support for firmware library on A31T21x
* @version	1.00
* @date			29. JUNE. 2020
* @author	ABOV M team
*
* Copyright(C) 2019, ABOV Semiconductor
* All rights reserved.
*
************************************************************************
* ABOV Disclaimer
*
*IMPORTANT NOTICE ? PLEASE READ CAREFULLY
*ABOV Semiconductor ("ABOV") reserves the right to make changes, corrections, enhancements, 
*modifications, and improvements to ABOV products and/or to this document at any time without notice. 
*ABOV does not give warranties as to the accuracy or completeness of the information included herein.
*Purchasers should obtain the latest relevant information of ABOV products before placing orders. 
*Purchasers are entirely responsible for the choice, selection, and use of ABOV products and 
*ABOV assumes no liability for application assistance or the design of purchasers¡¯ products. No license, 
*express or implied, to any intellectual property rights is granted by ABOV herein. 
*ABOV disclaims all express and implied warranties and shall not be responsible or
*liable for any injuries or damages related to use of ABOV products in such unauthorized applications. 
*ABOV and the ABOV logo are trademarks of ABOV.
*All other product or service names are the property of their respective owners. 
*Information in this document supersedes and replaces the information previously
*supplied in any former versions of this document.
*2020 ABOV Semiconductor  All rights reserved
*
**********************************************************************/


/* Includes ------------------------------------------------------------ */
#include "A31T21x_hal_fmc.h"


/*********************************************************************//**
 * @brief						Clear Chip Write Protection
 * @param[in]	None
 * @return				None
 **********************************************************************/
void ClearWriteProtection(void)
{
	// PROT mode entry
	FMC->MR = 0x66;
	FMC->MR = 0x99;
	FMC->WPROT = 0x00000000;
	FMC->MR = 0x00;
	FMC->MR = 0x00;
}

/*********************************************************************//**
 * @brief						Configure Flash Access Time
 * @param[in]	None
 * @return				None
 **********************************************************************/
void ConfigFlashAccessTime(void)
{
	// AMBA mode entry
	FMC->MR = 0x81;
	FMC->MR = 0x28;
	FMC->CFG = 0x78580300;	// 3-wait
	FMC->MR = 0x00;
	FMC->MR = 0x00;
}

/*********************************************************************//**
 * @brief						Enter Flash Mode
 * @param[in]	None
 * @return				None
 **********************************************************************/
void EnterFlashMode(void)
{
	// Flash mode entry
	FMC->MR = 0x5A;
	FMC->MR = 0xA5;
}

/*********************************************************************//**
 * @brief						Exit Flash Mode
 * @param[in]	None
 * @return				None
 **********************************************************************/
void ExitFlashMode(void)
{
	FMC->CR = 0x00000000;
	FMC->MR = 0x00;
	FMC->MR = 0x00;
}

/*********************************************************************//**
 * @brief						Check Flash Busy Status
 * @param[in]	None
 * @return				None
 **********************************************************************/
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

/*********************************************************************//**
 * @brief						Set User OTP Area
 * @param[in]	None
 * @return				None
 **********************************************************************/
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

/*********************************************************************//**
 * @brief						Bulk Erase
 * @param[in]	None
 * @return				0: success / 1: fail
 **********************************************************************/
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

/*********************************************************************//**
 * @brief						Page Erase (512Byte)
 * @param[in]	target_addr
 * @return				0: success / 1: fail
 **********************************************************************/
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

/*********************************************************************//**
 * @brief						Sector Erase (1KByte)
 * @param[in]	target_addr
 * @return				0: success / 1: fail
 **********************************************************************/
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

/*********************************************************************//**
 * @brief						Sector Erase (4KByte)
 * @param[in]	target_addr
 * @return				0: success / 1: fail
 **********************************************************************/
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

/*********************************************************************//**
 * @brief						Program Page (512Byte)
 * @param[in]	target_addr
 * @param[in]	page_size
 * @param[in]	data_buffer[512]
 * @return				0: success / 1: fail
 **********************************************************************/
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
