/***************************************************************************//**
*   @file					MC96F6432_IR_API_V0.90.c
*   @brief				IR Receive Function
*   @author		ABOV Semiconductor Co., Ltd.
*   @version		V0.90
*   @date				25. Mar. 2020
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
#include "MC96F6432_IR_API_V0.90.h"
#include "MC96F6432.h"
/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/

/*******************************************************************************
* Private Typedef
*******************************************************************************/

/*******************************************************************************
* Private Variable
*******************************************************************************/
unsigned int			gu16Timer_Buffer;
unsigned long	gu32IR_Data;
unsigned char	gu8IR_DataCnt;
unsigned char	gu8IR_LeadOn;

unsigned char gu8CustomData;
unsigned char	gu8UserData;
/*******************************************************************************
* Private Function Prototype
*******************************************************************************/

/*******************************************************************************
* Public Function
*******************************************************************************/
/**
* @brief
* @param   
* @return
*/
void IR_Rec_Init()
{
	gu32IR_Data = 0;
	gu8IR_DataCnt = 0;
	gu8IR_LeadOn = 0;
}

/**
* @brief
* @param   
* @return
*/
void IR_API_Run()
{
	gu8CustomData = (unsigned char)((gu32IR_Data >> 24) & (0xFF));
	gu8UserData = (unsigned char)((gu32IR_Data >> 8) & (0xFF));

	/* User Specific Code */	
	if(gu8UserData == 0x84)
	{
		if((P1 & 0x01) == 0x01)
			P10 = 0;
		else
			P10 = 1;
	}
	else if(gu8UserData == 0x44)
	{
		if((P2 & 0x01) == 0x01)
			P20 = 0;
		else
			P20 = 1;
	}
	else if(gu8UserData == 0xC4)
	{
		if((P2 & 0x02) == 0x02)
			P21 = 0;
		else
			P21 = 1;
	}
}

/**
* @brief
* @param   
* @return
*/
void IR_Rec_Handler()
{
	/* Load Timer Capture Data */
	gu16Timer_Buffer	= T1BDRH;
	gu16Timer_Buffer	= ((gu16Timer_Buffer << 8) & 0xFF00);
	gu16Timer_Buffer	|= T1BDRL;

	/* No IR Lead */
	if(gu8IR_LeadOn == 0)
	{
	 	if((Lead_Min <= gu16Timer_Buffer) && (gu16Timer_Buffer <= Lead_Max))
		{
			gu8IR_LeadOn = 1;
			gu8IR_DataCnt = 0;
		}
		else
		{
			IR_Rec_Init();
		} 
	}
	/* IR Lead On */
	else
	{
		gu32IR_Data = gu32IR_Data << 1;
		
		// Bit-1 detected
		if((Bit1_Min <= gu16Timer_Buffer) && (gu16Timer_Buffer <= Bit1_Max))
		{
			gu32IR_Data |= 1;
			gu8IR_DataCnt++;
		}
		//Bit-0 detected
		else if((Bit0_Min <= gu16Timer_Buffer) && (gu16Timer_Buffer <= Bit0_Max))
		{
			gu8IR_DataCnt++;
 		}
		// IR Data Error
		else
		{
			IR_Rec_Init();	
		}
	}
	/* IR Receive Complete */
	if(gu8IR_DataCnt == 32)
	{
		IR_API_Run();
		IR_Rec_Init();
	}
}

/************************ (C) COPYRIGHT ABOV SEMICONDUCTOR *****END OF FILE****/
