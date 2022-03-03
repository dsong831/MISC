/***************************************************************************//**
*   @file					GDC8310_LCD_API.h
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

/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/

/*******************************************************************************
* Private Typedef
*******************************************************************************/

/*******************************************************************************
* Private Variable
*******************************************************************************/
extern unsigned char gu8SEG1;
extern unsigned char gu8SEG2;
extern unsigned char *pu8LCD_Buff;
/*******************************************************************************
* Private Function Prototype
*******************************************************************************/
void LCD_NUM_CAL(unsigned char dec_num);
void LCD_BUFFER_INIT(unsigned char init_value);
void GDC8310_LCD_SET(unsigned char number, unsigned char seg_num1, unsigned char seg_num2);

/************************ (C) COPYRIGHT ABOV SEMICONDUCTOR *****END OF FILE****/
