/**************************************************************************//**
 * @file     FlashPrg.c
 * @brief    Flash Programming Functions adapted for New Device Flash
 * @version  V1.0.0
 * @date     10. January 2018
 ******************************************************************************/
/*
 * Copyright (c) 2010-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "FlashOS.h"        // FlashOS Structures

/* 
   Mandatory Flash Programming Functions (Called by FlashOS):
                int Init        (unsigned long adr,   // Initialize Flash
                                 unsigned long clk,
                                 unsigned long fnc);
                int UnInit      (unsigned long fnc);  // De-initialize Flash
                int EraseSector (unsigned long adr);  // Erase Sector Function
                int ProgramPage (unsigned long adr,   // Program Page Function
                                 unsigned long sz,
                                 unsigned char *buf);

   Optional  Flash Programming Functions (Called by FlashOS):
                int BlankCheck  (unsigned long adr,   // Blank Check
                                 unsigned long sz,
                                 unsigned char pat);
                int EraseChip   (void);               // Erase complete Device
      unsigned long Verify      (unsigned long adr,   // Verify Function
                                 unsigned long sz,
                                 unsigned char *buf);

       - BlanckCheck  is necessary if Flash space is not mapped into CPU memory space
       - Verify       is necessary if Flash space is not mapped into CPU memory space
       - if EraseChip is not provided than EraseSector for all sectors is called
*/

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* Public Definition
*******************************************************************************/
#define	rSCU_CSCR			(*(volatile unsigned long *)0x40000040)
#define	rSCU_SCCR			(*(volatile unsigned long *)0x40000044)

#define	rFMC_MR					(*(volatile unsigned long *)0x40000104)
#define	rFMC_CR					(*(volatile unsigned long *)0x40000108)
#define	rFMC_AR					(*(volatile unsigned long *)0x4000010C)
#define	rFMC_DR					(*(volatile unsigned long *)0x40000110)
#define	rFMC_BUSY			(*(volatile unsigned long *)0x40000118)
#define	rFMC_CFG				(*(volatile unsigned long *)0x40000130)
#define	rFMC_WPROT	(*(volatile unsigned long *)0x40000134)
	
#define	rWDT_CR					(*(volatile unsigned long *)0x40001A00)
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

#define	FMMR_BUSY								(rFMC_BUSY & 0x01)
#define	FMCR_SELFPGM_SET	(rFMC_CR |= BV_SELFPGM)
#define	FMCR_SELFPGM_CLR	(rFMC_CR &= ~BV_SELFPGM)
#define	FMCR_IFEN_SET					(rFMC_CR |= BV_IFEN)
#define	FMCR_IFEN_CLR					(rFMC_CR &= ~BV_IFEN)
#define	FMCR_BBLOCK_SET		(rFMC_CR |= BV_BBLOCK)
#define	FMCR_BBLOCK_CLR		(rFMC_CR &= ~BV_BBLOCK)
#define	FMCR_MAS_SET					(rFMC_CR |= BV_MAS)
#define	FMCR_MAS_CLR					(rFMC_CR &= ~BV_MAS)
#define	FMCR_SECT4K_SET			(rFMC_CR |= BV_SECT4K)
#define	FMCR_SECT4K_CLR			(rFMC_CR &= ~BV_SECT4K)
#define	FMCR_SECT1K_SET			(rFMC_CR |= BV_SECT1K)
#define	FMCR_SECT1K_CLR			(rFMC_CR &= ~BV_SECT1K)
#define	FMCR_PMOD_SET				(rFMC_CR |= BV_PMOD)
#define	FMCR_PMOD_CLR				(rFMC_CR &= ~BV_PMOD)
#define	FMCR_WADCK_SET			(rFMC_CR |= BV_WADCK)
#define	FMCR_WADCK_CLR			(rFMC_CR &= ~BV_WADCK)
#define	FMCR_PGM_SET					(rFMC_CR |= BV_PGM)
#define	FMCR_PGM_CLR					(rFMC_CR &= ~BV_PGM)
#define	FMCR_ERS_SET						(rFMC_CR |= BV_ERS)
#define	FMCR_ERS_CLR						(rFMC_CR &= ~BV_ERS)
#define	FMCR_HVEN_SET					(rFMC_CR |= BV_HVEN)
#define	FMCR_HVEN_CLR					(rFMC_CR &= ~BV_HVEN)
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
	rFMC_MR = 0x66;
	rFMC_MR = 0x99;
	rFMC_WPROT = 0x00000000;
	rFMC_MR = 0x00;
	rFMC_MR = 0x00;
}

/**
* @brief			Configure Flash Access Time
* @param	None
* @return		None
*/
void ConfigFlashAccessTime(void)
{
	// AMBA mode entry
	rFMC_MR = 0x81;
	rFMC_MR = 0x28;
	rFMC_CFG = 0x78580300;	// 3-wait
	rFMC_MR = 0x00;
	rFMC_MR = 0x00;
}

/**
* @brief			Enter Flash Mode
* @param	None
* @return		None
*/
void EnterFlashMode(void)
{
	// Flash mode entry
	rFMC_MR = 0x5A;
	rFMC_MR = 0xA5;
}

/**
* @brief			Exit Flash Mode
* @param	None
* @return		None
*/
void ExitFlashMode(void)
{
	rFMC_CR = 0x00000000;
	rFMC_MR = 0x00;
	rFMC_MR = 0x00;
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
	rFMC_BUSY = 0x80;	// WRBUSY flag clear
}

/**
* @brief			Set User OTP Area
* @param	None
* @return		None
*/
void SetUserOTPArea(unsigned long otp_idx)
{
	EnterFlashMode();
	if(otp_idx == 0)
	{
		rFMC_CR = 0;
	}
	else
	{
		FMCR_IFEN_SET;
		FMCR_SECT4K_CLR;
		FMCR_SECT1K_CLR;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {

  /* Add your Code */
	__disable_irq();
	// WDT Off
	rWDT_CR = (0x5A69U << 16)
            |(0x25<<10)
            |(0x1A<<4)
            |0;
	
	// Enable All Clock Source
	rSCU_CSCR = (0xA507U << 16)
            |(0x8<<12)
            |(0x8<<8)
            |(0x8<<4)
            |(0x8<<0);

	// Set SystemClock to HSI
	rSCU_SCCR = (0x570AU << 16)
            |(0x02);
	
  return (0);                                  // Finished without Errors
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {

  /* Add your Code */
	// Set SystemClock to LSI
	rSCU_SCCR = (0x570AU << 16)
            |(0x00);

	// Disable All Clock Source Except LSI
	rSCU_CSCR = (0xA507U << 16)
            |(0x0<<12)
            |(0x8<<8)
            |(0x0<<4)
            |(0x0<<0); 

  return (0);                                  // Finished without Errors
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip (void) {

  /* Add your Code */
#if 1		// Only use in flashloader
	SetUserOTPArea(1);		// Enable User OTP Area
	EraseSector(0);						// Erase User OTP Area
	SetUserOTPArea(0);		// Disable User OTP Area
#endif

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
	
  return (0);                                  // Finished without Errors
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {

  /* Add your Code */
	ClearWriteProtection();

	rFMC_AR = adr >> 2;		// least 2 bits will be ignored

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
	
  return (0);                                  // Finished without Errors
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {

  /* Add your Code */
	int	i;
	unsigned long *ptr;

	/* Check address and size alignment */
	if((adr & 0x03) || (sz & 0x03))
	{
		return (1);		// fail
	}

	/* Data Load */
	ptr = (unsigned long *)buf;

	/* PGM Sequence Start */
	ClearWriteProtection();
	
	rFMC_AR = adr >> 2;		// least 2 bits will be ignored
	
	ConfigFlashAccessTime();
	EnterFlashMode();

	FMCR_PMOD_SET;
	FMCR_PGM_SET;

	for(i=0; i<sz; i+=4)
	{
		rFMC_DR = *ptr++;

		FMCR_WADCK_SET;
		FMCR_HVEN_SET;
		CheckFlashBusyStatus();
		FMCR_HVEN_CLR;
	}
	
	FMCR_PGM_CLR;
	
	ExitFlashMode();
	ConfigFlashAccessTime();
	
  return (0);                                  // Finished without Errors
}


/**
********************************************************************************************************
* @ Name : Verify
*
*
* @ Parameters
*		addr				start address of flash memory to be written (in terms of byte)
*		size				write size (in terms of byte)
*		buf				start address of buffer 
*
*
* @ return value
*		(addr+size)		success
*		other value		error 
*
********************************************************************************************************
*/
unsigned long Verify (unsigned long addr, unsigned long size, unsigned char *buf)
{
	unsigned long	i;
	unsigned long	*src;
	unsigned long	*dst;
	unsigned long	result; 

	//---------------------------------------------------------------------------------
	// check address and size alignment
	//---------------------------------------------------------------------------------
	if((addr & 0x03) || (size & 0x03)) {
		return (1); 
	}

	//---------------------------------------------------------------------------------
	// Verify
	//---------------------------------------------------------------------------------
	result = addr + size; 

	ClearWriteProtection();
	ConfigFlashAccessTime();
	EnterFlashMode();

	src = (unsigned long *)buf;
	dst = (unsigned long *)addr;
	for( i=0; i<size; i+=4) {
		if( *src++ != *dst++ ) {
			result = addr + i;
			break;
		}
	}

	src = (unsigned long *)buf;
	dst = (unsigned long *)addr;
	for( i=0; i<size; i+=4) {
		if( *src++ != *dst++ ) {
			result = addr + i;
			break;
		}
	}

	ExitFlashMode();
	ConfigFlashAccessTime();

	return (result);
}
