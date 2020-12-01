/**********************************************************************//**
* @file				A31T21x_hal_crc.c
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
#include "A31T21x_hal_crc.h"


/* Private variable -------------------------------------------------------- */
/* Private Types --------------------------------------------------------------- */
/* Public Functions ------------------------------------------------------- */
/******************************************************************************//**
 * @brief        Initialize CRC/Checksum peripheral
 * @param[in]    None
 *
 * @return       HAL_Status_Type
 ******************************************************************************/
 
HAL_Status_Type HAL_CRC_Init(void)
{
    SCU->PER2 &=~(1UL<<31);
	  SCU->PCER2&=~(1UL<<31);
    /* Set up peripheral clock for CRC module */
    SCU->PER2 |= (1UL<<31);
    SCU->PCER2|= (1UL<<31);
	
    return HAL_OK;
}


/******************************************************************************//**
* @brief      DeInitialize CRC peripheral
* @param      None
* @return     HAL_Status
******************************************************************************/
HAL_Status_Type HAL_CRC_DeInit(void)
{
	// Disable CRC peripheral & clock
	SCU->PER2 &= ~(1UL<<31);
	SCU->PCER2 &= (1UL<<31);
	
	return HAL_OK;
}



/******************************************************************************//**
 * @brief        CRC/Checksum Auto Mode Start and Result
 * @param[in]    u32SEL             CRC/Checksum Selection:               CRC_CRC, CRC_CHECKSUM
 * @param[in]    u32POLY            CRC Polynomial Selection:             CRC_CCITT, CRC_16
 * @param[in]    u321stBit          CRC 1st Shifted-in bit:               CRC_MSB, CRC_LSB
 *
 * @return       CRC/Checksum Result
 
 ******************************************************************************/
uint32_t HAL_CRC_ConfigAutoMode(uint32_t u32SEL, uint32_t u32POLY, uint32_t u32FirstBit)
{
//	if(SystemCoreClock>20000000uL) SCUCG->SCDIVR0_b.HDIV = 3;               // HCLK should be less than or equal to 20MHz during CRC/Checksum auto mode

	CRC->CR = (0x1uL<<6)                              // Initialize RLT Register with INIT value
	             |CRC_AUTO_M                                                // Set Auto Mode
	             |u32SEL
	             |u32POLY
	             |CRC_NOINC                                                 // Don't care at auto mode
	             |u32FirstBit;
//	DI();
//	CRCRun();
//	EI();
//	while(ChkCRCFinish()) {}                                                // Check if CRC/Checksum finishes or not
//	SCUCG->PPCLKEN1_b.CRCLKE = 0;                                           // CRC/Checksum Clock Disable
//	if(SystemCoreClock>20000000uL) SCUCG->SCDIVR0_b.HDIV = 4;               // HCLK should be set with original frequency
	return (CRC->RLT & 0xffff);
}

/******************************************************************************//**
 * @brief        CRC/Checksum User Mode Start
 * @param[in]    u32SEL             CRC/Checksum Selection:               CRC_CRC, CRC_CHECKSUM
 * @param[in]    u32POLY            CRC Polynomial Selection:             CRC_CCITT, CRC_16
 * @param[in]    u32AdrInc          Auto Increment of Start Address:      CRC_NOINC, CRC_AUTOINC
 * @param[in]    u321stBit          CRC 1st Shifted-in bit:               CRC_MSB, CRC_LSB
 *
 * @return       HAL_Status_Type 
 ******************************************************************************/
HAL_Status_Type HAL_CRC_ConfigUserMode(uint32_t u32SEL, uint32_t u32POLY, uint32_t u32FirstBit)
{
	CRC->CR = (0x1uL<<6)                              // Initialize RLT Register with INIT value
	             |CRC_USER_M                                                // Set User Mode
	             |u32SEL
	             |u32POLY
	             |u32FirstBit;
	return HAL_OK;
//	CRCRun();
}

/******************************************************************************//**
 * @brief        CRC/Checksum Input on User Mode
 * @param[in]    u32Input           CRC/Checksum Input
 *
 * @return       CRC/Checksum Result with finish Indicator:               0x8a29xxxx if on run, 0x0000xxxx if Finish
 *
 * @explain      This function calculates up to finish. The CRCSADR register should be incremented by 4 every input on CRC_NOINC
 ******************************************************************************/
uint32_t HAL_CRC_UserInput(uint32_t u32Input)
{
	uint32_t u32Result = 0x8a290000uL;
	
	CRC_InData(u32Input);
//	if(!ChkCRCFinish() || (CRC->CRCSADR > CRC->CRCEADR)) {                  // "Auto"/"User" Increment of Start Address
//	  CRCStop();                                                            // Stop forcingly on User Increment of Start Address
//	  SCUCG->PPCLKEN1_b.CRCLKE = 0;                                         // CRC/Checksum Clock Disable
//	  u32Result = (CRC->RLT & 0xffff);
//	}
	return u32Result;
}


/******************************************************************************//**
 * @brief        CRC/Checksum Get Data
 * @return       CRC/Checksum Result with finish Indicator:               0x8a29xxxx if on run, 0x0000xxxx if Finish
 *
 * @explain      This function calculates up to finish. The CRCSADR register should be incremented by 4 every input on CRC_NOINC
 ******************************************************************************/
uint32_t HAL_CRC_GetData(void)
{
	uint32_t	u32Result;
	
	u32Result = CRC->RLT;
	
	return u32Result;
}


/******************************************************************************//**
 * @brief        CRC/Checksum User Init Setting
 * @param[in]    UserInit           CRC/Checksum Init
 *
 * @explain      This function calculates up to finish. The CRCSADR register should be incremented by 4 every input on CRC_NOINC
 ******************************************************************************/
HAL_Status_Type HAL_CRC_SetUserInput(uint32_t InitVal)
{
	CRC->INIT = InitVal;
	return HAL_OK;
}


/******************************************************************************//**
 * @brief        CRC/Checksum Interrupt Setting
 * @param[in]    IntSel           CRC/Checksum Interrup Setting
 *
 * @explain      Set interrupt enable/disable
 ******************************************************************************/
HAL_Status_Type HAL_CRC_ConfigInterrupt(uint32_t IntSel)
{
	uint32_t	tempreg;
	
	tempreg = CRC->CR;
	
	tempreg &= ~(1<<9);
	
	if (IntSel)
	{
		tempreg |= (1<<9);
	}
	
	CRC->CR = tempreg;
	return HAL_OK;
}

/******************************************************************************//**
 * @brief        Get CRC/Checksum Interrupt Status
 * @return       CRC/Checksum Interrupt Status
 *
 * @explain      return CRC interrupt status
 ******************************************************************************/
uint32_t HAL_CRC_GetState_IT(void)
{
	return (CRC->CR & (1<<8));
}

/******************************************************************************//**
 * @brief        Clear CRC/Checksum Interrupt Status
 * @explain      Clear CRC interrupt status
 ******************************************************************************/
HAL_Status_Type HAL_CRC_ClearState_IT(void)
{
	CRC->CR |= (1<<8);
	return HAL_OK;
}


/* --------------------------------- End Of File ------------------------------ */

