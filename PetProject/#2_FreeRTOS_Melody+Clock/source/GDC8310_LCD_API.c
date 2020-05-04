/***************************************************************************//**
*   @file					GDC8310_LCD_API.c
*   @brief				LCD Function
*   @author		ABOV Semiconductor Co., Ltd.
*   @version		V0.90
*   @date				23. Apr. 2020
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
#include "GDC8310_LCD_API.h"
/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/

/*******************************************************************************
* Private Typedef
*******************************************************************************/

/*******************************************************************************
* Private Variable
*******************************************************************************/
unsigned char gu8SEG1;
unsigned char gu8SEG2;
unsigned char *pu8LCD_Buff;
/*******************************************************************************
* Private Function Prototype
*******************************************************************************/

/*******************************************************************************
* Public Function
*******************************************************************************/
/**
* @brief GDC8310 (1/4 duty, 1/3 bias LCD, 3.3V)
Number : SEG(n),SEG(n+1)
0 : 0xF,0xA
1 : 0x0,0xA
2 : 0xB,0xC
3 : 0x9,0xE
4 : 0x4,0xE
5 : 0xD,0x6
6 : 0xF,0x6
7 : 0xC,0xA
8 : 0xF,0xE
9 : 0xD,0xE
* @param   
* @return
*/
void LCD_TO_NUM(unsigned char dec_num)
{
	switch(dec_num)
	{
		case 0:
			gu8SEG1 = 0x0F;
			gu8SEG2 = 0x0A;
			break;

		case 1:
			gu8SEG1 = 0x00;
			gu8SEG2 = 0x0A;
			break;

		case 2:
			gu8SEG1 = 0x0B;
			gu8SEG2 = 0x0C;
			break;

		case 3:
			gu8SEG1 = 0x09;
			gu8SEG2 = 0x0E;
			break;

		case 4:
			gu8SEG1 = 0x04;
			gu8SEG2 = 0x0E;
			break;

		case 5:
			gu8SEG1 = 0x0D;
			gu8SEG2 = 0x06;
			break;

		case 6:
			gu8SEG1 = 0x0F;
			gu8SEG2 = 0x06;
			break;

		case 7:
			gu8SEG1 = 0x0C;
			gu8SEG2 = 0x0A;
			break;

		case 8:
			gu8SEG1 = 0x0F;
			gu8SEG2 = 0x0E;
			break;

		case 9:
			gu8SEG1 = 0x0D;
			gu8SEG2 = 0x0E;
			break;

		default:
			gu8SEG1 = 0x0F;
			gu8SEG2 = 0x0A;
			break;
	}
}

/**
* @brief
* @param   
* @return
*/
void LCD_Buff_Init(void)
{
	unsigned char i;
	
	pu8LCD_Buff = (unsigned char *)(0x40005010);

	for(i=0; i<27; i++)
	{
		pu8LCD_Buff[i] = 0x00;
	}
}

/**
* @brief
* @param   
* @return
*/
void GDC8310_LCD_RUN(void)
{
	// A31G31x_LCD_Init
	A31G31x_LCD_Init();

	// LCD Buffer Initialization
	LCD_Buff_Init();
	
	/* User application */
	LCD_TO_NUM(0);
	// LCD Array 1 - PC12/PC11
	pu8LCD_Buff[13] = gu8SEG1;
	pu8LCD_Buff[14] = gu8SEG2;
	// LCD Array 2 - PC10/PC9
	pu8LCD_Buff[15] = gu8SEG1;
	pu8LCD_Buff[16] = gu8SEG2;
	// LCD Array 3 - PC8/PC7
	pu8LCD_Buff[17] = gu8SEG1;
	pu8LCD_Buff[18] = gu8SEG2;
	// LCD Array 4 - PC6/PC5
	pu8LCD_Buff[19] = gu8SEG1;
	pu8LCD_Buff[20] = gu8SEG2;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
* @brief
* @param   
* @return
*/
void A31G31x_LCD_Init(void)
{
	LCD_CFG_Type	LCD_Config;

	// COM0~3 Setting
	PCU_SetDirection(PE, 0, ALTERN_FUNC);
	PCU_ConfigureFunction(PE, 0, FUNC0);
	PCU_ConfigurePullupdown(PE, 0, DISPUPD);

	PCU_SetDirection(PE, 1, ALTERN_FUNC);
	PCU_ConfigureFunction(PE, 1, FUNC0);
	PCU_ConfigurePullupdown(PE, 1, DISPUPD);

	PCU_SetDirection(PE, 2, ALTERN_FUNC);
	PCU_ConfigureFunction(PE, 2, FUNC0);
	PCU_ConfigurePullupdown(PE, 2, DISPUPD);

	PCU_SetDirection(PE, 3, ALTERN_FUNC);
	PCU_ConfigureFunction(PE, 3, FUNC0);
	PCU_ConfigurePullupdown(PE, 3, DISPUPD);

	// SEG13~20 Setting
	PCU_SetDirection(PC, 12, ALTERN_FUNC);
	PCU_ConfigureFunction(PC, 12, FUNC0);
	PCU_ConfigurePullupdown(PC, 12, DISPUPD);

	PCU_SetDirection(PC, 11, ALTERN_FUNC);
	PCU_ConfigureFunction(PC, 11, FUNC0);
	PCU_ConfigurePullupdown(PC, 11, DISPUPD);

	PCU_SetDirection(PC, 10, ALTERN_FUNC);
	PCU_ConfigureFunction(PC, 10, FUNC0);
	PCU_ConfigurePullupdown(PC, 10, DISPUPD);

	PCU_SetDirection(PC, 9, ALTERN_FUNC);
	PCU_ConfigureFunction(PC, 9, FUNC0);
	PCU_ConfigurePullupdown(PC, 9, DISPUPD);

	PCU_SetDirection(PC, 8, ALTERN_FUNC);
	PCU_ConfigureFunction(PC, 8, FUNC0);
	PCU_ConfigurePullupdown(PC, 8, DISPUPD);
	
	PCU_SetDirection(PC, 7, ALTERN_FUNC);
	PCU_ConfigureFunction(PC, 7, FUNC0);
	PCU_ConfigurePullupdown(PC, 7, DISPUPD);

	PCU_SetDirection(PC, 6, ALTERN_FUNC);
	PCU_ConfigureFunction(PC, 6, FUNC0);
	PCU_ConfigurePullupdown(PC, 6, DISPUPD);

	PCU_SetDirection(PC, 5, ALTERN_FUNC);
	PCU_ConfigureFunction(PC, 5, FUNC0);
	PCU_ConfigurePullupdown(PC, 5, DISPUPD);

	// LCD Config Setting
	LCD_Config.LCDDRSel = LCDCR_INTBIAS;
	LCD_Config.IrSel = LCDCR_RLCD3;
	LCD_Config.DutyBias = LCDCR_Duty_div4_Bias_div3;
	LCD_Config.LCDCLK = LCDCR_LCDCLK_1024Hz;
	LCD_Config.DISPSel = LCDCR_DISP_OFF;
	
	LCD_Config.LCDAutoBias = LCDBCCR_AUTOBIAS_OFF;
	LCD_Config.BiasModeSel = LCDBCCR_BMSEL_4CLK;
	LCD_Config.LCTSel = DISABLE;
	LCD_Config.VLCDVal = LCDBCCR_VLCD_31STEP;
	
	LCD_Config.externBiasEn = DISABLE;

	//LCD Clock Setting
	SCU_SetLcdClk(LCDCLK_WDTRC); //LCDCLK_MCCR5, LCDCLK_SOSC, LCDCLK_WDTRC

	// LCD Block Enable
	SCU->PER2 &= ~(1<<28);
	SCU->PCER2 &= ~(1<<28);
	
	SCU->PER2 |= (1<<28);
	SCU->PCER2 |= (1<<28);
	
	LCD->CR = 0
	| ((LCD_Config.IrSel&(3))<<6)
	| ((LCD_Config.DutyBias&(7))<<3)
	| ((LCD_Config.LCDCLK&(3))<<1)
	| ((LCD_Config.DISPSel&(1))<<0)
	;
	
	LCD->BCCR = 0
	| ((LCD_Config.LCDAutoBias&(1))<<12)
	| ((LCD_Config.BiasModeSel&(7))<<8)
	| ((LCD_Config.LCTSel&(1))<<5)
	| ((LCD_Config.VLCDVal&(0xF))<<0)
	;

	if(LCD_Config.externBiasEn == ENABLE){
		LCD->BSSR = 0xBF0;
	}else{
		LCD->BSSR = 0;
	}
	
	LCD->CR |= (1<<0);
}

/************************ (C) COPYRIGHT ABOV SEMICONDUCTOR *****END OF FILE****/
