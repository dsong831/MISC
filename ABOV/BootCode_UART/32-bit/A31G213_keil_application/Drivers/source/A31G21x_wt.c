/**********************************************************************
* @file		A31G21x_wt.c
* @brief	Contains all functions support for WT firmware library
* 			on A31G21x
* @version	1.0
* @date		
* @authorABOV M team
*
* Copyright(C) 2015, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/* Includes ------------------------------------------------------------------- */
#include "A31G21x_wt.h"


uint32_t _WTCLK;

/* Public Functions ----------------------------------------------------------- */
/**********************************************************************
* @brief 		Initial for Watch Timer function
* @param[in]	WT_CFG_Type 
* @return 		None
 **********************************************************************/
void WT_Init(WT_CFG_Type wtCfg)
{
	SCU->PER1 &= ~(1UL<<31); // WT 31 bit
	SCU->PCER1 &= ~(1UL<<31);	

	SCU->PER1 |= (1UL<<31);
	SCU->PCER1 |= (1UL<<31);	
	

	WT->CR = 0
		|((wtCfg.wtClkDiv&0x03)<<4) // WTINTV[1:0]
		|WTCR_WTCLR
		;
	WT->DR = (wtCfg.wtTmrConst & 0xFFF);
}

/**********************************************************************
* @brief 		deinit for Watch Timer function
* @param[in]	None
* @return 		None
**********************************************************************/
void WT_DeInit(void)
{
	WT->CR = 0;
}

/**********************************************************************
* @brief 		Enable WT activity
* @param[in]	FunctionalState ctrl 
*						- DISABLE: wt enable
*						- ENABLE: wt disable 
* @return 		None
 **********************************************************************/
void WT_Start(FunctionalState ctrl)
{
	uint32_t tmp_reg;
	
	tmp_reg = WT->CR&0xFF;
	tmp_reg &= ~(0x1<<7); // Disable watch-dog timer counter
	
	if (ctrl == ENABLE){
		tmp_reg |= (0x1<<7); // Enable watch timer counter
	}
	
	tmp_reg |= WTCR_WTCLR; // clear the counter and divider

	WT->CR = tmp_reg; 
}

/*********************************************************************
 * @brief 		Get the current value of WT
 * @param[in]	None
 * @return		current value of WT
 *********************************************************************/
uint32_t WT_GetCurrentCount(void)
{
	return (WT->CNT & 0xFFF);
}

/*********************************************************************
 * @brief 		Get the timer status register of WT
 * @param[in]	None
 * @return		the status register of WT
 *********************************************************************/
uint32_t WT_GetStatus(void)
{
	return (WT->CR & WT_STATUS_BITMASK);
}

/*********************************************************************
 * @brief 		Clear the timer status register of WT
 * @param[in]	None
 * @return		None
 *********************************************************************/
void WT_ClearStatus(void)
{
	WT->CR |= WTCR_WTIFLAG; //(1<<1) : clear for WTIFLAG
}

/********************************************************************
 * @brief 		Enable or disable WT interrupt.
 * @param[in]	NewState New state of specified WT interrupt type,
 * 				should be:
 * 					- ENALBE	:Enable this interrupt type.
 * 					- DISALBE	:Disable this interrupt type.
 * @return 		None
 *********************************************************************/
void WT_IntConfig(FunctionalState NewState)
{
	uint32_t reg_val=0;
	
	reg_val =(WT->CR &0xFF);
	
	if (NewState == ENABLE) {
		reg_val |= WTCR_WTIEN; // WTIEN bit
	}
	else	{
		reg_val &= ~WTCR_WTIEN;
	}
	
	WT->CR = reg_val | WTCR_WTIFLAG; //(1<<1) : clear for WTIFLAG
}

/* --------------------------------- End Of File ------------------------------ */
