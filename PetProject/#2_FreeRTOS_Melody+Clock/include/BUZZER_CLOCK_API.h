/***************************************************************************//**
*   @file					BUZZER_CLOCK_API.h
*   @brief				BUZZER Function
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
#include "a31G31x.h"
#include "A31G31x_timer1n.h"
#include "A31G31x_pcu.h"

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
extern unsigned long gu32Clock_msec;
extern unsigned long gu32Buzzer_msec;
/*******************************************************************************
* Private Function Prototype
*******************************************************************************/
void clock_delay_ms(unsigned int cnt);
void buzzer_delay_ms(unsigned int cnt);
void G(void);
void A(void);
void B(void);
void C(void);
void D(void);
void E(void);
void F(void);
void G_H(void);
void BUZZER_API_RUN(void);
void CLOCK_API_RUN(void);

/************************ (C) COPYRIGHT ABOV SEMICONDUCTOR *****END OF FILE****/
