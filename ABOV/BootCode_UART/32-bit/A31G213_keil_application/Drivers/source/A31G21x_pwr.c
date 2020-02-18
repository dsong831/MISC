/**********************************************************************
* @file		A31G216X_pwr.c
* @brief	Contains all functions support for Power Control
* 			firmware library on A31G216X
* @version	1.0
* @date		
* @author		ABOV M team
*
* Copyright(C) 2015, ABOV Semiconductor
* All rights reserved.
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "A31G21x_scu.h"
#include "A31G21x_pwr.h"

/*********************************************************************//**
 * @brief 		Enter Sleep mode with co-operated instruction by the Cortex-M3.
 * @param[in]	None
 * @return		None
 **********************************************************************/
void PWR_Sleep(void)
{
	/* Sleep Mode*/
	SCB->SCR = 0;
	__DSB();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();	
}

/*********************************************************************//**
 * @brief 		Enter Power Down mode with co-operated instruction by the Cortex-M3.
 * @param[in]	None
 * @return		None
 **********************************************************************/
void PWR_PowerDown(void)
{
    /* Deep-Sleep Mode, set SLEEPDEEP bit */
	SCB->SCR = 0x4;
	/* Power Down Mode*/
	__WFI();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
}


/*********************************************************************//**
 * @brief 		Low Voltage Indicator Control
 * @param[in]	None
 * @return		None
 **********************************************************************/
void LVI_Init(LVI_CFG_Type *LVI_ConfigStruct)
{
	uint32_t	tempreg;
	
	tempreg = SCULV->LVICR;
	tempreg = 0
	| ((LVI_ConfigStruct->EnSel&(1<<7))<<7)
	| ((LVI_ConfigStruct->IntSel&(1<<5))<<5)
	| ((LVI_ConfigStruct->LvlSel&(0xF<<0))<<0)
	;
	
	SCULV->LVICR = tempreg;
}



/* --------------------------------- End Of File ------------------------------ */
