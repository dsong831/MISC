/**********************************************************************//**
* @file				A31T21x_hal_fmc.h
* @brief			Contains all functions support for firmware library on A31T21x
* @version	1.00
* @date			26. JUNE. 2020
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
*ABOV assumes no liability for application assistance or the design of purchasers?? products. No license, 
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


#ifndef A31T21X_HAL_FMC_H
#define A31T21X_HAL_FMC_H


/* Includes -------------------------------------------------------------- */
#include "A31T21x.h"
#include "A31T21x_hal_aa_types.h"


/* Private macros -------------------------------------------------------- */
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


/* Public Functions ----------------------------------------------------------- */
int EraseChip(void);
int ErasePage(unsigned long target_addr);
int EraseSector1KB(unsigned long target_addr);
int EraseSector4KB(unsigned long target_addr);
int ProgramPage(unsigned long target_addr, unsigned long page_size, unsigned char *data_buffer);


#endif		/* A31T21X_HAL_FMC_H */

/* --------------------------------- End Of File ------------------------------ */
