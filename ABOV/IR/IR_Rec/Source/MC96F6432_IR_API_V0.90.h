/***************************************************************************//**
*   @file					MC96F6432_IR_API_V0.90.h
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

/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/
/* NEC format @ To use timer1 period 8us */
// LEAD : (9000+4500)us +- 20%
#define	Lead_Max		2025		// 16200us / 8us = 2025
#define	Lead_Min			1350		// 10800us / 8us = 1350
// Bit1 : (560+1680)us +- 20%
#define	Bit1_Max			336			// 2688us / 8us = 336
#define	Bit1_Min				224			// 1792us / 8us = 224
// Bit0 : (560+560)us +- 20%
#define	Bit0_Max			168			// 1344us / 8us = 168
#define	Bit0_Min				112			// 896us / 8us = 112
/*******************************************************************************
* Private Typedef
*******************************************************************************/

/*******************************************************************************
* Private Variable
*******************************************************************************/

/*******************************************************************************
* Private Function Prototype
*******************************************************************************/

/*******************************************************************************
* Public Function
*******************************************************************************/
void IR_Rec_Init();
void IR_API_Run();
void IR_Rec_Handler();

/************************ (C) COPYRIGHT ABOV SEMICONDUCTOR *****END OF FILE****/
