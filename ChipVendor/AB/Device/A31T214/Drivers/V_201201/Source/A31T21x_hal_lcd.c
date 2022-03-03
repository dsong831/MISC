/**********************************************************************//**
* @file				A31T21x_hal_lcd.c
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
#include "A31T21x_hal_lcd.h"


/**********************************************************************
 * @brief						HAL_LCD_Init
 * @param[in]	LCD Type, LCD Config Struct
 * @return				None
 **********************************************************************/
void HAL_LCD_Init(LCD_Type *LCDn, LCD_CFG_Type *LCD_Config)
{
	/* LCD Block Enable */
	SCU->PER2 &= ~(1<<28);
	SCU->PCER2 &= ~(1<<28);
	SCU->PER2 |= (1<<28);
	SCU->PCER2 |= (1<<28);

	/* LCD Configure */
	LCDn->CR = 0
	| (LCD_Config->tLCDDR)		// LCD Driving Resistor (Internal or External)
	| (LCD_Config->tIRSEL)			// Internal LCD Bias (RLCD1 ~ RLCD4)
	| (LCD_Config->tDBS)				// LCD Duty & Bias
	| (LCD_Config->tLCLK)				// LCD Clock Selection (128Hz ~ 1024Hz)
	;

	LCDn->BCCR = 0
	| (LCD_Config->tLCDABC)	// LCD Automatic bias On/Off
	| (LCD_Config->tBMSEL)		// Bias mode time selection
	| (LCD_Config->tVLCD)			// VLC0 Voltage Control
	;
}

/**********************************************************************
 * @brief						LCD Display On
 * @param[in]	None
 * @return				None
 **********************************************************************/
void HAL_LCD_DISP_ON(void)
{
	LCD->CR |= LCD_CR_DISP_ON;
}

/**********************************************************************
 * @brief						LCD Display Off
 * @param[in]	None
 * @return				None
 **********************************************************************/
void HAL_LCD_DISP_OFF(void)
{
	LCD->CR &= ~(LCD_CR_DISP_ON);
}


/* --------------------------------- End Of File ------------------------------ */
