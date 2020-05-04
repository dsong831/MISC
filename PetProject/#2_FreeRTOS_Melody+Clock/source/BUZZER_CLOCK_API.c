/***************************************************************************//**
*   @file					BUZZER_CLOCK_API.c
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
#include "BUZZER_CLOCK_API.h"
/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/

/*******************************************************************************
* Private Typedef
*******************************************************************************/

/*******************************************************************************
* Private Variable
*******************************************************************************/
unsigned long gu32Clock_msec;
unsigned long gu32Buzzer_msec;
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
void clock_delay_ms(unsigned int cnt)
{
	gu32Clock_msec = cnt;
	while(gu32Clock_msec);
}

/**
* @brief
* @param   
* @return
*/
void buzzer_delay_ms(unsigned int cnt)
{
	gu32Buzzer_msec = cnt;
	while(gu32Buzzer_msec);
}

/**
* @brief
* @param   
* @return
*/
void G(void)
{
	TIMER10->ADR = 1277;
}

void A(void)
{
	TIMER10->ADR = 1136;
}

void B(void)
{
	TIMER10->ADR = 1013;	
}

void C(void)
{
	TIMER10->ADR = 956;
}

void D(void)
{
	TIMER10->ADR = 851;
}

void E(void)
{
	TIMER10->ADR = 758;
}

void F(void)
{
	TIMER10->ADR = 716;
}

void G_H(void)
{
	TIMER10->ADR = 638;
}

void BUZZER_API_RUN(void)
{
	unsigned char i;

	for(i=0; i<2; i++)
	{
		G();
		TIMER1n_Cmd(TIMER10, ENABLE);
		buzzer_delay_ms(1000);
		C();
		buzzer_delay_ms(1000);
		B();
		buzzer_delay_ms(500);
		C();
		buzzer_delay_ms(500);
		D();
		buzzer_delay_ms(1000);
		C();
		buzzer_delay_ms(500);
		D();
		buzzer_delay_ms(500);
		E();
		buzzer_delay_ms(1000);
		F();
		buzzer_delay_ms(500);
		E();
		buzzer_delay_ms(500);
		A();
		buzzer_delay_ms(1000);
		D();
		buzzer_delay_ms(250);
		TIMER1n_Cmd(TIMER10, DISABLE);
		buzzer_delay_ms(250);
		TIMER1n_Cmd(TIMER10, ENABLE);
		D();
		buzzer_delay_ms(500);
		C();
		buzzer_delay_ms(1000);
		TIMER1n_Cmd(TIMER10, DISABLE);
		C();
		TIMER1n_Cmd(TIMER10, ENABLE);
		buzzer_delay_ms(250);
		TIMER1n_Cmd(TIMER10, DISABLE);
		buzzer_delay_ms(250);
		TIMER1n_Cmd(TIMER10, ENABLE);
		C();
		buzzer_delay_ms(500);
		B();
		buzzer_delay_ms(1000);
		A();
		buzzer_delay_ms(500);
		B();
		buzzer_delay_ms(500);
		C();
		buzzer_delay_ms(1000);
		TIMER1n_Cmd(TIMER10, DISABLE);
		buzzer_delay_ms(1000);
	}
	E();
	TIMER1n_Cmd(TIMER10, ENABLE);
	buzzer_delay_ms(500);
	F();
	buzzer_delay_ms(500);
	G_H();
	buzzer_delay_ms(600);
	TIMER1n_Cmd(TIMER10, DISABLE);
	buzzer_delay_ms(400);
	E();
	TIMER1n_Cmd(TIMER10, ENABLE);
	buzzer_delay_ms(500);
	D();
	buzzer_delay_ms(500);	
	C();
	buzzer_delay_ms(600);
	TIMER1n_Cmd(TIMER10, DISABLE);
	buzzer_delay_ms(400);
	B();
	TIMER1n_Cmd(TIMER10, ENABLE);
	buzzer_delay_ms(500);	
	C();
	buzzer_delay_ms(500);	
	D();
	buzzer_delay_ms(500);	
	C();
	buzzer_delay_ms(500);
	B();
	buzzer_delay_ms(500);	
	A();
	buzzer_delay_ms(500);
	G();
	buzzer_delay_ms(500);
	TIMER1n_Cmd(TIMER10, DISABLE);
	buzzer_delay_ms(500);

	E();
	TIMER1n_Cmd(TIMER10, ENABLE);
	buzzer_delay_ms(500);
	F();
	buzzer_delay_ms(500);
	G_H();
	buzzer_delay_ms(600);
	TIMER1n_Cmd(TIMER10, DISABLE);
	buzzer_delay_ms(400);
	E();
	TIMER1n_Cmd(TIMER10, ENABLE);
	buzzer_delay_ms(500);
	D();
	buzzer_delay_ms(500);	
	C();
	buzzer_delay_ms(600);
	TIMER1n_Cmd(TIMER10, DISABLE);
	buzzer_delay_ms(400);
	B();
	TIMER1n_Cmd(TIMER10, ENABLE);
	buzzer_delay_ms(500);	
	C();
	buzzer_delay_ms(500);	
	D();
	buzzer_delay_ms(1000);
	TIMER1n_Cmd(TIMER10, DISABLE);
	buzzer_delay_ms(1000);

	G();
	TIMER1n_Cmd(TIMER10, ENABLE);
	buzzer_delay_ms(1000);
	C();
	buzzer_delay_ms(2000);	
	D();
	buzzer_delay_ms(2000);
	E();
	buzzer_delay_ms(250);
	TIMER1n_Cmd(TIMER10, DISABLE);
	buzzer_delay_ms(250);
	TIMER1n_Cmd(TIMER10, ENABLE);
	E();
	buzzer_delay_ms(500);
	F();
	buzzer_delay_ms(500);
	E();
	buzzer_delay_ms(500);
	A();
	buzzer_delay_ms(500);	
	TIMER1n_Cmd(TIMER10, DISABLE);	
	buzzer_delay_ms(500);

	D();
	TIMER1n_Cmd(TIMER10, ENABLE);	
	buzzer_delay_ms(250);
	TIMER1n_Cmd(TIMER10, DISABLE);
	buzzer_delay_ms(250);
	TIMER1n_Cmd(TIMER10, ENABLE);
	D();
	buzzer_delay_ms(500);
	C();
	buzzer_delay_ms(1000);
	TIMER1n_Cmd(TIMER10, DISABLE);
	C();
	TIMER1n_Cmd(TIMER10, ENABLE);
	buzzer_delay_ms(250);
	TIMER1n_Cmd(TIMER10, DISABLE);
	buzzer_delay_ms(250);
	TIMER1n_Cmd(TIMER10, ENABLE);
	C();
	buzzer_delay_ms(500);
	B();
	buzzer_delay_ms(1000);
	A();
	buzzer_delay_ms(500);
	B();
	buzzer_delay_ms(500);
	C();
	buzzer_delay_ms(1000);
	TIMER1n_Cmd(TIMER10, DISABLE);
	buzzer_delay_ms(1000);

	TIMER1n_Cmd(TIMER10, DISABLE);
}

void CLOCK_API_RUN(void)
{
	unsigned char i;
	unsigned char u8Index = 0;
	unsigned char u8SwitchInput = 1;
	unsigned char au8CountNumber[4];
	
	for(i=0; i<4; i++)
	{
		au8CountNumber[i] = 0;
	}

	while(1)
	{
		u8SwitchInput = (GPIO_ReadValue(PE) & (1<<7));	

		if(!u8SwitchInput)
		{
			u8SwitchInput = 1;
			u8Index = 1;
		}
		if(u8Index == 1)
		{
			/* 1s clock application */
			clock_delay_ms(1004);
			au8CountNumber[3]++;
			if(au8CountNumber[3] > 9)
			{
				au8CountNumber[3] = 0;
				au8CountNumber[2]++;
			}
			if(au8CountNumber[2] > 9)
			{
				au8CountNumber[2] = 0;
				au8CountNumber[1]++;
			}
			if(au8CountNumber[1] > 9)
			{
				au8CountNumber[1] = 0;
				au8CountNumber[0]++;
			}
			if(au8CountNumber[0] > 9)
			{
				au8CountNumber[0] = 0;
				au8CountNumber[1] = 0;
				au8CountNumber[2] = 0;
				au8CountNumber[3] = 0;
			}
			
			// LCD Array 1
			LCD_TO_NUM(au8CountNumber[0]);
			pu8LCD_Buff[13] = gu8SEG1;
			pu8LCD_Buff[14] = gu8SEG2;
			// LCD Array 2
			LCD_TO_NUM(au8CountNumber[1]);
			pu8LCD_Buff[15] = gu8SEG1;
			pu8LCD_Buff[16] = gu8SEG2;
			// LCD Array 3
			LCD_TO_NUM(au8CountNumber[2]);
			pu8LCD_Buff[17] = gu8SEG1;
			pu8LCD_Buff[18] = gu8SEG2;
			// LCD Array 4
			LCD_TO_NUM(au8CountNumber[3]);
			pu8LCD_Buff[19] = gu8SEG1;
			pu8LCD_Buff[20] = gu8SEG2;
		}
	}
}

/************************ (C) COPYRIGHT ABOV SEMICONDUCTOR *****END OF FILE****/
