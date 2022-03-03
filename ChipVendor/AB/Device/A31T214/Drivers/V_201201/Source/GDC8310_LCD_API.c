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
void LCD_NUM_CAL(unsigned char dec_num)
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
void LCD_BUFFER_INIT(unsigned char init_value)
{
	unsigned char i;
	
	pu8LCD_Buff = (unsigned char *)(0x40005010);

	for(i=0; i<32; i++)
	{
		pu8LCD_Buff[i] = init_value;
	}
}

/**
* @brief
* @param   
* @return
*/
void GDC8310_LCD_SET(unsigned char number, unsigned char seg_num1, unsigned char seg_num2)
{
	/* User application */
	LCD_NUM_CAL(number);
	pu8LCD_Buff[seg_num1] = gu8SEG1;
	pu8LCD_Buff[seg_num2] = gu8SEG2;
}


/************************ (C) COPYRIGHT ABOV SEMICONDUCTOR *****END OF FILE****/
