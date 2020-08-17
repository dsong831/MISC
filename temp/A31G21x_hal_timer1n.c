/**********************************************************************//**
* @file		A31G21x_hal_timer1n.c
* @brief	       Contains all macro definitions and function prototypes support
* @version	1.00
* @date: 
* @author	ABOV Application Team
*
* Copyright(C) 2019, ABOV Semiconductor. All rights reserved.

************************************************************************
* ABOV Disclaimer
*
*IMPORTANT NOTICE ? PLEASE READ CAREFULLY
*ABOV Semiconductor ("ABOV") reserves the right to make changes, corrections, enhancements, 
*modifications, and improvements to ABOV products and/or to this document at any time without notice. 
*ABOV does not give warranties as to the accuracy or completeness of the information included herein.
*Purchasers should obtain the latest relevant information of ABOV products before placing orders. 
*Purchasers are entirely responsible for the choice, selection, and use of ABOV products and 
*ABOV assumes no liability for application assistance or the design of purchasers¡¯ products. No license, 
*express or implied, to any intellectual property rights is granted by ABOV herein. 
*ABOV disclaims all express and implied warranties and shall not be responsible or
*liable for any injuries or damages related to use of ABOV products in such unauthorized applications. 
*ABOV and the ABOV logo are trademarks of ABOV.
*All other product or service names are the property of their respective owners. 
*Information in this document supersedes and replaces the information previously
*supplied in any former versions of this document.
*2020 ABOV Semiconductor  All rights reserved
*
**********************************************************************/



#include "A31G21x_hal_timer1n.h"
#include "A31G21x_hal_scu.h"



/* Public Functions ------------------------------------------------------- */
/**********************************************************************//**
 * @brief 		Initial Timer/Counter device
 * 				 	Set Clock frequency for Timer
 * 					Set initial configuration for Timer
 * @param[in]	TIMER1n Timer selection, should be:
 * 					- TIMER1n peripheral n=0..6
 * @param[in]	TimerCounterMode Timer counter mode, should be:
 * 					- PERIODIC_MODE			:Timer mode
 * 					- PWM_MODE	:Counter rising mode
 * 					- ONESHOT_MODE	:Counter falling mode
 * 					- CAPTURE_MODE		:Counter on both edges
 * @param[in]	TIMER_ConfigStruct pointer to TIMER_ConfigStruct
 * 				that contains the configuration information for the
 *                    specified Timer peripheral.
 * @return 		HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_TIMER1n_Init(TIMER1n_Type *TIMER1n, TIMER1n_MODE_OPT TimerCounterMode, void *TIMER_ConfigStruct)
{
	TIMER1n_PERIODICCFG_Type *pTimeCfg;
	TIMER1n_PWMCFG_Type *pPwmOneshotCfg;
	TIMER1n_CAPTURECFG_Type *pCaptureCfg;
	uint16_t reg_val16;
	
	if (TIMER1n== TIMER10)
	{
		SCU->PER1 &= ~(1<<16);
		SCU->PCER1 &= ~(1<<16);
		
		SCU->PER1 |= (1<<16);
		SCU->PCER1 |= (1<<16);		
	}
	else if (TIMER1n== TIMER11)
	{
		SCU->PER1 &= ~(1<<17);
		SCU->PCER1 &= ~(1<<17);

		SCU->PER1 |= (1<<17);
		SCU->PCER1 |= (1<<17);	
	}
	else if (TIMER1n== TIMER12)
	{
		SCU->PER1 &= ~(1<<18);
		SCU->PCER1 &= ~(1<<18);
		
		SCU->PER1 |= (1<<18);
		SCU->PCER1 |= (1<<18);
	}
	else if (TIMER1n== TIMER13)
	{
		SCU->PER1 &= ~(1<<19);
		SCU->PCER1 &= ~(1<<19);

		SCU->PER1 |= (1<<19);
		SCU->PCER1 |= (1<<19);		
	}
	else{
	}

	if (TimerCounterMode == TIMER1n_PERIODIC_MODE)
	{
		pTimeCfg = (TIMER1n_PERIODICCFG_Type *)TIMER_ConfigStruct;
		
		reg_val16 = 0
			| TIMER1n_CR_CKSEL_SET(pTimeCfg->CkSel)
			| TIMER1n_CR_MODE_SET(TimerCounterMode)
			| TIMER1n_CR_STARTLVL_SET(pTimeCfg->StartLevel)
			;
		if (pTimeCfg->CkSel==1) {
			reg_val16 = reg_val16 | TIMER1n_CR_ECE_SET(pTimeCfg->ECE);
		}
		TIMER1n->CR = reg_val16;
		
		TIMER1n->PREDR = ((pTimeCfg->Prescaler-1) & TIMER1n_PRS_MASK);
		TIMER1n->ADR = pTimeCfg->ADR;
	}
	else if ((TimerCounterMode == TIMER1n_PWM_MODE) || (TimerCounterMode == TIMER1n_ONESHOT_MODE))
	{
		pPwmOneshotCfg = (TIMER1n_PWMCFG_Type *)TIMER_ConfigStruct;

		reg_val16 = 0
			| TIMER1n_CR_CKSEL_SET(pPwmOneshotCfg->CkSel)
			| TIMER1n_CR_MODE_SET(TimerCounterMode)
			| TIMER1n_CR_STARTLVL_SET(pPwmOneshotCfg->StartLevel)
			;
		if (pPwmOneshotCfg->CkSel==1) {
			reg_val16 = reg_val16 | TIMER1n_CR_ECE_SET(pPwmOneshotCfg->ECE);
		}		
		TIMER1n->CR = reg_val16;

		TIMER1n->PREDR = ((pPwmOneshotCfg->Prescaler-1) & TIMER1n_PRS_MASK);
		TIMER1n->ADR = pPwmOneshotCfg->ADR;
		TIMER1n->BDR = pPwmOneshotCfg->BDR;		
	}
	else if (TimerCounterMode == TIMER1n_CAPTURE_MODE)
	{
		pCaptureCfg = (TIMER1n_CAPTURECFG_Type *)TIMER_ConfigStruct;
		
		reg_val16 = 0
			| TIMER1n_CR_CKSEL_SET(pCaptureCfg->CkSel)
			| TIMER1n_CR_MODE_SET(TimerCounterMode)
			| TIMER1n_CR_CPOL_SET(pCaptureCfg->ClrMode)
			;
		if (pCaptureCfg->CkSel==1) {
			reg_val16 = reg_val16 | TIMER1n_CR_ECE_SET(pCaptureCfg->ECE);
		}
		TIMER1n->CR = reg_val16;
		
		TIMER1n->PREDR = ((pCaptureCfg->Prescaler-1) & TIMER1n_PRS_MASK);
		TIMER1n->ADR = pCaptureCfg->ADR;		
	}
	TIMER1n->CR |= 0x1; //  timer counter clear
	return HAL_OK;
}

/**********************************************************************//**
 * @brief 		Close Timer/Counter device
 * @param[in]	TIMER1n  Pointer to timer device, should be:
 * 					- TIMER1n peripheral n=0..6
 * @return 		HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_TIMER1n_DeInit (TIMER1n_Type *TIMER1n)
{
	// Disable timer/counter
	TIMER1n->CR = 0x00;

	if (TIMER1n== TIMER10)
	{
		SCU->PER1 &= ~(1<<16);
		SCU->PCER1 &= ~(1<<16);
	}
	else if (TIMER1n== TIMER11)
	{
		SCU->PER1 &= ~(1<<17);
		SCU->PCER1 &= ~(1<<17);
	}
	else if (TIMER1n== TIMER12)
	{
		SCU->PER1 &= ~(1<<18);
		SCU->PCER1 &= ~(1<<18);
	}
	else if (TIMER1n== TIMER13)
	{
		SCU->PER1 &= ~(1<<19);
		SCU->PCER1 &= ~(1<<19);
	}
        else{
	}
  	return HAL_OK;
}

/**********************************************************************//**
 * @brief	 	Start/Stop Timer/Counter device
 * @param[in]	TIMER1n Pointer to timer device, should be:
 * 					- TIMER1n peripheral n=0..6
 * @param[in]	NewState
 * 					- ENABLE  	:Set timer enable
 * 					- DISABLE 	:Disable timer
 * @return 		HAL Satus
 **********************************************************************/
HAL_Status_Type HAL_TIMER1n_Cmd(TIMER1n_Type *TIMER1n, FunctionalState NewState)
{
	if (NewState == ENABLE) {
		TIMER1n->CR |=  TIMER1n_ENABLE;	
	}
	else {
		TIMER1n->CR &= ~TIMER1n_ENABLE;
	}
	return HAL_OK;
}

/********************************************************************//**
 * @brief 		Enable or disable TIMER interrupt.
 * @param[in]	TIMER1n Pointer to timer device, should be:
 * 					- TIMER1n peripheral n=0..6
 * @param[in]	TIMERIntCfg	Specifies the interrupt flag,
 * 				should be one of the following:
 *						- TIMER1n_INTCFG_MIE : OVIE Interrupt enable
 *						- TIMER1n_INTCFG_CIE : MBIE Interrupt enable
 * @param[in]	NewState New state of specified TIMER1n interrupt type,
 * 				should be:
 * 					- ENALBE	:Enable this interrupt type.
 * 					- DISALBE :Disable this interrupt type.
 * @return 		HAL Satus
 *********************************************************************/
HAL_Status_Type HAL_TIMER1n_ConfigInterrupt(TIMER1n_Type *TIMER1n, TIMER1n_INT_Type TIMERIntCfg, FunctionalState NewState)
{
	uint8_t tmp;

	switch(TIMERIntCfg){
		case TIMER1n_INTCFG_MIE:
			tmp = TIMER1n_MATINTEN;
			break;
		case TIMER1n_INTCFG_CIE:
			tmp = TIMER1n_CAPINTEN;
			break;			
	}

	if (NewState == ENABLE)
	{
		TIMER1n->CR |= tmp;
	}
	else // DISABLE
	{
		TIMER1n->CR &= (~tmp);
	}
	return HAL_OK;
}

/********************************************************************//**
   @brief         Timer 1n Counter and Prescaler clear
   @param[in]    TIMER1n_Type           TIMER1n peripheral n=0..6
   @explain      This macro clears Counter and Prescalrer of Timer 1n
   @return 		HAL Satus
 *********************************************************************/
HAL_Status_Type HAL_TIMER1n_ClearCounter(TIMER1n_Type *TIMER1n)
{
	TIMER1n->CR |= (1<<0);
	return HAL_OK;
}

/********************************************************************//**
   @brief        Get flags of Timer 1n Match Interrupt
   @param[in]    TIMER1n_Type           TIMER1n peripheral n=0..6
   @explain      This macro gets interrupt flag of Timer 1n Match Interrupt
 *********************************************************************/

uint32_t HAL_TIMER1n_GetMatchInterrupt(TIMER1n_Type *TIMER1n)
{
	if (TIMER1n->CR & (1<<3)){
		return (1);
	}
	else {
		return (0);
	}

}

/********************************************************************//**

   @brief        Clear flags of Timer 1n Match Interrupt
   @param[in]    TIMER1n_Type           TIMER1n peripheral n=0..6
   @explain      This macro clears interrupt flag of Timer 1n Match Interrupt
    @return 		HAL Satus
 *********************************************************************/

HAL_Status_Type HAL_TIMER1n_ClearMatchInterrupt(TIMER1n_Type *TIMER1n)
{
	TIMER1n->CR |= (1<<3);
	return HAL_OK;
}

/********************************************************************//**

   @brief         Get Timer 1n Capture Data Register
   @param[in]    TIMER1n_Type           TIMER1n peripheral n=0..6
   @explain      This macro gets Timer 1n Capture Data Register
 *********************************************************************/

uint32_t HAL_TIMER1n_GetCaptureData(TIMER1n_Type *TIMER1n)
{
	return (TIMER1n->CAPDR);
}

/********************************************************************//**

   @brief        Get flags of Timer 1n Capture Interrupt
   @param[in]    TIMER1n_Type          TIMER1n peripheral n=0..6
   @explain      This macro gets interrupt flag of Timer 1n Capture Interrupt
 *********************************************************************/

uint32_t HAL_TIMER1n_GetCaptureInterrupt(TIMER1n_Type *TIMER1n)
{
	if (TIMER1n->CR & (1<<2)){
		return 1;
	}
	else {
		return 0;
	}	
}

/********************************************************************//**

   @brief        Clear flags of Timer 1n Capture Interrupt
   @param[in]    TIMER1n_Type          TIMER1n peripheral n=0..6
   @explain      This macro clears interrupt flag of Timer 1n Capture Interrupt
   @return 		HAL Satus
 *********************************************************************/

HAL_Status_Type HAL_TIMER1n_ClearCaptureInterrupt(TIMER1n_Type *TIMER1n)
{
	TIMER1n->CR |= (1<<2);
	return HAL_OK;
}




