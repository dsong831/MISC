/**********************************************************************
* @file		A31G21x_crc.c
* @brief	Contains all functions support for ADC firmware library on A31G21x
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2015, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "A31G21x_crc.h"

/******************************************************************************
 * @brief        Initialize CRC/Checksum peripheral
 * @param[in]    None
 *
 * @return       None
 *
 * @explain      Initialize CRC/Checksum peripheral
 ******************************************************************************/
 
void CRC_Init(void){
    SCU->PER2 &=~(1UL<<31);
	SCU->PCER2&=~(1UL<<31);
    /* Set up peripheral clock for CRC module */
    SCU->PER2 |= (1UL<<31);
    SCU->PCER2|= (1UL<<31);
}

/******************************************************************************
 * @brief        CRC/Checksum Auto Mode Start and Result
 * @param[in]    u32SEL             CRC/Checksum Selection:               CRC_CRC, CRC_CHECKSUM
 * @param[in]    u32POLY            CRC Polynomial Selection:             CRC_CCITT, CRC_16
 * @param[in]    u321stBit          CRC 1st Shifted-in bit:               CRC_MSB, CRC_LSB
 *
 * @return       CRC/Checksum Result
 *
 * @explain      This function starts CRC/Checksum calculation and gets result
 ******************************************************************************/
uint32_t CRC_AutoGet(uint32_t u32SEL, uint32_t u32POLY, uint32_t u32FirstBit)
{
//	if(SystemCoreClock>20000000uL) SCUCG->SCDIVR0_b.HDIV = 3;               // HCLK should be less than or equal to 20MHz during CRC/Checksum auto mode

	CRC->CR = (0x1uL<<CRC_CR_RLTCLR_Pos)                              // Initialize RLT Register with INIT value
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

/******************************************************************************
 * @brief        CRC/Checksum User Mode Start
 * @param[in]    u32SEL             CRC/Checksum Selection:               CRC_CRC, CRC_CHECKSUM
 * @param[in]    u32POLY            CRC Polynomial Selection:             CRC_CCITT, CRC_16
 * @param[in]    u32AdrInc          Auto Increment of Start Address:      CRC_NOINC, CRC_AUTOINC
 * @param[in]    u321stBit          CRC 1st Shifted-in bit:               CRC_MSB, CRC_LSB
 *
 * @return       None
 *
 * @explain      This function sets the control register for CRC/Checksum User mode
 ******************************************************************************/
void CRC_UserSet(uint32_t u32SEL, uint32_t u32POLY, uint32_t u32FirstBit)
{
	CRC->CR = (0x1uL<<CRC_CR_RLTCLR_Pos)                              // Initialize RLT Register with INIT value
	             |CRC_USER_M                                                // Set User Mode
	             |u32SEL
	             |u32POLY
	             |u32FirstBit;
//	CRCRun();
}

/******************************************************************************
 * @brief        CRC/Checksum Input on User Mode
 * @param[in]    u32Input           CRC/Checksum Input
 *
 * @return       CRC/Checksum Result with finish Indicator:               0x8a29xxxx if on run, 0x0000xxxx if Finish
 *
 * @explain      This function calculates up to finish. The CRCSADR register should be incremented by 4 every input on CRC_NOINC
 ******************************************************************************/
uint32_t CRC_UserInput(uint32_t u32Input)
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


/******************************************************************************
 * @brief        CRC/Checksum Get Data
 * @return       CRC/Checksum Result with finish Indicator:               0x8a29xxxx if on run, 0x0000xxxx if Finish
 *
 * @explain      This function calculates up to finish. The CRCSADR register should be incremented by 4 every input on CRC_NOINC
 ******************************************************************************/
uint32_t CRC_GetData(void)
{
	uint32_t	u32Result;
	
	u32Result = CRC->RLT;
	
	return u32Result;
}


/******************************************************************************
 * @brief        CRC/Checksum User Init Setting
 * @param[in]    UserInit           CRC/Checksum Init
 *
 * @explain      This function calculates up to finish. The CRCSADR register should be incremented by 4 every input on CRC_NOINC
 ******************************************************************************/
void CRC_SetINIT(uint32_t InitVal)
{
	CRC->INIT = InitVal;
}


/******************************************************************************
 * @brief        CRC/Checksum Interrupt Setting
 * @param[in]    IntSel           CRC/Checksum Interrup Setting
 *
 * @explain      Set interrupt enable/disable
 ******************************************************************************/
void CRC_INTCmd(uint32_t IntSel)
{
	uint32_t	tempreg;
	
	tempreg = CRC->CR;
	
	tempreg &= ~(1<<9);
	
	if (IntSel)
	{
		tempreg |= (1<<9);
	}
	
	CRC->CR = tempreg;
}

/******************************************************************************
 * @brief        Get CRC/Checksum Interrupt Status
 * @return       CRC/Checksum Interrupt Status
 *
 * @explain      return CRC interrupt status
 ******************************************************************************/
uint32_t CRC_GetINTStatus(void)
{
	return (CRC->CR & (1<<8));
}

/******************************************************************************
 * @brief        Clear CRC/Checksum Interrupt Status
 * @explain      Clear CRC interrupt status
 ******************************************************************************/
void CRC_ClearINTStatus(void){
	CRC->CR |= (1<<8);
}


/* --------------------------------- End Of File ------------------------------ */

