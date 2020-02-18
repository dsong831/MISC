/**********************************************************************
* @file		A31G21x_wdt.c
* @brief	Contains all functions support for WDT firmware library
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
#include "A31G21x_wdt.h"

uint32_t _WDTCLK;

/* Public Functions ----------------------------------------------------------- */
/**********************************************************************
* @brief 		Initial for Watchdog function
* @param[in]	none
* @return 		None
 **********************************************************************/
void WDT_Init(WDT_CFG_Type wdtCfg)
{
	uint32_t reg_val=0;

	WDT->DR = (wdtCfg.wdtTmrConst & 0x00FFFFFF);
	WDT->WINDR = (wdtCfg.wdtWTmrConst & 0x00FFFFFF);
	
	reg_val = wdtCfg.wdtClkDiv;

	if(wdtCfg.wdtResetEn == ENABLE) {
		reg_val &= ~(0x3f<<10);
	}
	else {
		reg_val |= (0x25<<10);		
	}

	WDT->CR = (0x5A69 << 16) | (0x1a<<4) | reg_val;  //  /w Write Identification Key
}

/**********************************************************************
* @brief 		Close ADC
* @param[in]	None
* @return 		None
**********************************************************************/
void WDT_DeInit(void)
{
	WDT->CR = 0
	|(0x5A69 << 16) //Write Identification Key
	|(0x25  << 10)    //Disable watch-dog timer reset
	|(0x1A << 4)      //Disable watch-dog timer counter	
	;
}

/*********************************************************************
 * @brief 		Reload WDT counter
 * @param[in]	None
 * @return		None
 *********************************************************************/
void WDT_ReloadTimeCounter(void)
{
	WDT->CNTR = 0x6a;
}

/**********************************************************************
* @brief 		Enable WDT activity
* @param[in]	FunctionalState ctrl 
*						- DISABLE: wdt enable
*						- ENABLE: wdt disable 
* @return 		None
 **********************************************************************/
void WDT_Start(FunctionalState ctrl)
{
	uint32_t tmp_reg;
	
	tmp_reg = WDT->CR&0xFFFF;
	tmp_reg |= (0x1a<<4); // Disable watch-dog timer counter
	
	if (ctrl == ENABLE){
		tmp_reg &= ~(0x3f<<4); // Enable watch-dog timer counter, 
	}

	 WDT->CR = (0x5A69<<16) | tmp_reg; // Write Identification Key 0x5A69
}

/*********************************************************************
 * @brief 		Get the current value of WDT
 * @param[in]	None
 * @return		current value of WDT
 *********************************************************************/
uint32_t WDT_GetCurrentCount(void)
{
	return WDT->CNT;
}

/*********************************************************************
 * @brief 		Get the timer status register of WDT
 * @param[in]	None
 * @return		the status register of WDT
 *********************************************************************/
uint32_t WDT_GetStatus(void)
{
	return WDT->SR;
}

/*********************************************************************
 * @brief 		Clear the timer status register of WDT
 * @param[in]	clear bit
 *						- WDTSR_UNFIFLAG : UNFIFLAG Interrupt flag
 *						- WDTSR_WINMIFLAG : WINMIFLAG Interrupt flag
 * @return		None
 *********************************************************************/
void WDT_ClearStatus(uint32_t clrbit)
{
	WDT->SR = clrbit;
}

/********************************************************************
 * @brief 		Enable or disable WDT interrupt.
 * @param[in]	WDTIntCfg	Specifies the interrupt flag,
 * 				should be one of the following:
 *						- WDT_INTCFG_UNFIEN : UNFIEN Interrupt enable
 *						- WDT_INTCFG_WINMIEN : WINMIEN Interrupt enable
 * @param[in]	NewState New state of specified WDT interrupt type,
 * 				should be:
 * 					- ENALBE	:Enable this interrupt type.
 * 					- DISALBE	:Disable this interrupt type.
 * @return 		None
 *********************************************************************/
void WDT_IntConfig(WDT_INT_Type WDTIntCfg, FunctionalState NewState)
{
	uint32_t reg_val=0;
	uint32_t tmp=0;
	
	reg_val =(WDT->CR &0xFFFF);

	switch(WDTIntCfg){
		case WDT_INTCFG_UNFIEN:
			tmp = WDTCR_UNFIEN;
			break;
		case WDT_INTCFG_WINMIEN:
			tmp = WDTCR_WINMIEN;
			break;	
	}
	
	if (NewState == ENABLE)
	{
		reg_val |= (tmp & WDT_INTERRUPT_BITMASK);
	}
	else
	{
		reg_val &= ((~tmp) & WDT_INTERRUPT_BITMASK);
	}
	
	WDT->CR = (0x5A69<<16) | reg_val; // Write Identification Key 0x5A69	
}

/* --------------------------------- End Of File ------------------------------ */
