/**********************************************************************//**
* @file				A31T21x_hal_pwr.c
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
#include "A31T21x_hal_pwr.h"
#include "A31T21x_hal_scu.h"


/*********************************************************************//**
 * @brief						Enter Sleep mode with co-operated instruction by the Cortex-M0+
 * @param[in]	None
 * @return				None
 **********************************************************************/
void HAL_PWR_EnterSleepMode(void)
{
	/* Sleep Mode*/
	SCB->SCR = 0;
	__WFI();		// Wait For Interrupt
	__NOP();	// No Operation
	__NOP();
	__NOP();
	__NOP();
	__NOP();	
}

/*********************************************************************//**
 * @brief						Enter Power Down mode with co-operated instruction by the Cortex-M0+
 * @param[in]	stop_select  (Select stop mode)
 *											- 0	: STOP1 Mode
 *											- 1	: STOP2 Mode
 * @param[in]	aon_select		(Select always on block)
 *											- 0					: Clear always on block
 *											- 1					: Set always on block
 *											- 8					: VDC always on
 *											- 9					: BGR always on
 *											- 10				: ROSC always on
 * @return				None
 **********************************************************************/
void HAL_PWR_EnterPowerDownMode(unsigned char stop_select, unsigned char aon_select)
{
	unsigned char i;
	unsigned int prev_vdccon;

	/* Change System Clock to LSI */
	HAL_SCU_LSI_ClockConfig(LSIOSC_EN);
	SystemCoreClock = 500000;
	SystemPeriClock = 500000;
	for(i=0;i<10;i++);
	HAL_SCU_SystemClockChange(SC_LSIOSC);
	
	/* Clear Clock Source */
	HAL_SCU_ClearClockSource(SCU_CLEAR_HSE);
	HAL_SCU_ClearClockSource(SCU_CLEAR_LSE);
	HAL_SCU_ClearClockSource(SCU_CLEAR_HSI);

	/* Select Stop Mode */
	prev_vdccon = SCU->VDCCON;
	if(stop_select)
	{
		SCU->VDCCON = (0x5<<20)|prev_vdccon|(1<<17);		// STOP2 Mode
	}
	else
	{
		SCU->VDCCON = (0x5<<20)|(prev_vdccon&(0xFFFDFFFF));	// STOP1 Mode
	}

	/* LVD Disable	*/
	SCULV->LVRCR = 0x55;
	SCULV->LVRCNFIG = ((0x72A5<<16) | (0xAA<<8));

	/* Set System Mode */
	switch(aon_select)
	{
		case 1 :
			SCU->SMR |= (7<<8);		// Set always on block
			break;
		case 8 :
			SCU->SMR |= (1<<8);		// VDCAON
			break;
		case 9 :
			SCU->SMR |= (1<<9);		// BGRAON
			break;
		case 10 :
			SCU->SMR |= (1<<10);		// ROSCAON
			break;
		default :
			SCU->SMR &= ~(7<<8);	// Clear always on block
			break;
	}

	/* Deep-Sleep Mode, set SLEEPDEEP bit */
	SCB->SCR = 0x4;
	/* Power Down Mode*/
	__WFI();		// Wait For Interrupt
	__NOP();	// No Operation
	__NOP();
	__NOP();
	__NOP();
}

/*********************************************************************//**
 * @brief						Low Voltage Indicator Control
 * @param[in]	LVI Configure Structure
 * @return				HAL Status
 **********************************************************************/
HAL_Status_Type HAL_LVI_Init(LVI_CFG_Type *LVI_ConfigStruct)
{
	uint32_t reg32;

	reg32 = 0
		| (LVI_ConfigStruct->LVIEN)
		| (LVI_ConfigStruct->LVIINTEN)
		| (LVI_ConfigStruct->LVIVS)
		;

	SCULV->LVICR = reg32;

	if(SCULV->LVICR & 0x80)
	{
		return HAL_OK;
	}
	else
	{
		return HAL_ERROR;
	}
}

/**********************************************************************//**
* @brief			Get Value of LVI Interrupt Flag
* @param	None
* @return		Return LVI Interrupt Flag
**********************************************************************/
uint32_t HAL_SCU_LVI_GetFlag(void)
{
	return (SCULV->LVICR & (1<<4));	
}

/**********************************************************************//**

* @brief			Clear Value of LVI Interrupt Flag
* @param	None
* @return		None
**********************************************************************/
void HAL_SCU_LVI_ClearFlag(void)
{
	SCULV->LVICR |= (1<<4);
}


/* --------------------------------- End Of File ------------------------------ */
