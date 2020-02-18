/**********************************************************************
* @file		A31G21x_timer1n.h
* @brief	      Timer 1n functions
* @version	1.0
* @date:       2017-08-29
* @author	ABOV Application Team
*
* Copyright(C) 2017, ABOV Semiconductor. All rights reserved.
**********************************************************************/

#include "A31G21x_timer1n.h"
#include "A31G21x_scu.h"

/*----------------------------------------------------------------------------
   @brief        Set Timer1n CR/PREDR Registers
   @param[in]    u32T1nSet           Timer 1n Control Register Setting Data 
   @param[in]    u32T1nClk           Timer 1n Prescaler Data  
 * @return       None
 *
 * @explain      This function sets the mode, external clock edge, Timer out polarity, 
 *                       Capture Polarity and Timer match/capture interrupt.
 *----------------------------------------------------------------------------*/
  void TIMER1n_SetReg(TIMER1n_Type *TIMER1n, uint32_t u32T1nSet, uint32_t u32T1nClk)
{
	TIMER1n->CR = u32T1nSet;  //Setting Timer 1n Control Register

	TIMER1n->PREDR = u32T1nClk;  // Setting Timer 1n Prescaler data
}

/* Public Functions ----------------------------------------------------------- */
/**********************************************************************
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
 * @return 		None
 **********************************************************************/
void TIMER1n_Init(TIMER1n_Type *TIMER1n, TIMER1n_MODE_OPT TimerCounterMode, void *TIMER_ConfigStruct)
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

	if (TimerCounterMode == PERIODIC_MODE)
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
		
		TIMER1n->PREDR = ((pTimeCfg->Prescaler-1) & T1n_PRS_MASK);
		TIMER1n->ADR = pTimeCfg->ADR;
	}
	else if ((TimerCounterMode == PWM_MODE) || (TimerCounterMode == ONESHOT_MODE))
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

		TIMER1n->PREDR = ((pPwmOneshotCfg->Prescaler-1) & T1n_PRS_MASK);
		TIMER1n->ADR = pPwmOneshotCfg->ADR;
		TIMER1n->BDR = pPwmOneshotCfg->BDR;		
	}
	else if (TimerCounterMode == CAPTURE_MODE)
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
		
		TIMER1n->PREDR = ((pCaptureCfg->Prescaler-1) & T1n_PRS_MASK);
		TIMER1n->ADR = pCaptureCfg->ADR;		
	}
	TIMER1n->CR |= 0x1; //  timer counter clear
}

/**********************************************************************
 * @brief 		Close Timer/Counter device
 * @param[in]	TIMER1n  Pointer to timer device, should be:
 * 					- TIMER1n peripheral n=0..6
 * @return 		None
 **********************************************************************/
void TIMER1n_DeInit (TIMER1n_Type *TIMER1n)
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
}

/**********************************************************************
 * @brief	 	Start/Stop Timer/Counter device
 * @param[in]	TIMER1n Pointer to timer device, should be:
 * 					- TIMER1n peripheral n=0..6
 * @param[in]	NewState
 * 					- ENABLE  	:Set timer enable
 * 					- DISABLE 	:Disable timer
 * @return 		None
 **********************************************************************/
void TIMER1n_Cmd(TIMER1n_Type *TIMER1n, FunctionalState NewState)
{
	if (NewState == ENABLE) {
		TIMER1n->CR |=  T1n_ENABLE;	
	}
	else {
		TIMER1n->CR &= ~T1n_ENABLE;
	}
}

/********************************************************************
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
 * @return 		None
 *********************************************************************/
void TIMER1n_IntConfig(TIMER1n_Type *TIMER1n, TIMER1n_INT_Type TIMERIntCfg, FunctionalState NewState)
{
	uint8_t tmp;

	switch(TIMERIntCfg){
		case TIMER1n_INTCFG_MIE:
			tmp = T1n_MATINTEN;
			break;
		case TIMER1n_INTCFG_CIE:
			tmp = T1n_CAPINTEN;
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
}

/*----------------------------------------------------------------------------
   @brief         Timer 1n Counter and Prescaler clear
   @param[in]    T1N_Type           TIMER1n peripheral n=0..6
   @explain      This macro clears Counter and Prescalrer of Timer 1n
 *----------------------------------------------------------------------------*/
void TIMER1n_ClrCnt(TIMER1n_Type *TIMER1n)
{
	TIMER1n->CR |= (1<<0);
}

/*----------------------------------------------------------------------------
   @brief        Get flags of Timer 1n Match Interrupt
   @param[in]    T1N_Type           TIMER1n peripheral n=0..6
   @explain      This macro gets interrupt flag of Timer 1n Match Interrupt
 *----------------------------------------------------------------------------*/
uint32_t TIMER1nMaInt_GetFg(TIMER1n_Type *TIMER1n)
{
	return (TIMER1n->CR & (1<<3));
}

/*----------------------------------------------------------------------------
   @brief        Clear flags of Timer 1n Match Interrupt
   @param[in]    T1N_Type           TIMER1n peripheral n=0..6
   @explain      This macro clears interrupt flag of Timer 1n Match Interrupt
 *----------------------------------------------------------------------------*/
void TIMER1nMaInt_ClrFg(TIMER1n_Type *TIMER1n)
{
	TIMER1n->CR |= (1<<3);
}

/*----------------------------------------------------------------------------
   @brief         Get Timer 1n Capture Data Register
   @param[in]    T1N_Type           TIMER1n peripheral n=0..6
   @explain      This macro gets Timer 1n Capture Data Register
 *----------------------------------------------------------------------------*/
uint32_t TIMER1n_GetCapData(TIMER1n_Type *TIMER1n)
{
	return (TIMER1n->CAPDR);
}

/*----------------------------------------------------------------------------
   @brief        Get flags of Timer 1n Capture Interrupt
   @param[in]    T1N_Type          TIMER1n peripheral n=0..6
   @explain      This macro gets interrupt flag of Timer 1n Capture Interrupt
 *----------------------------------------------------------------------------*/
uint32_t TIMER1nCapInt_GetFg(TIMER1n_Type *TIMER1n)
{
	if (TIMER1n->CR & (1<<2)){
		return 1;
	}
	else {
		return 0;
	}	
}

/*----------------------------------------------------------------------------
   @brief        Clear flags of Timer 1n Capture Interrupt
   @param[in]    T1N_Type          TIMER1n peripheral n=0..6
   @explain      This macro clears interrupt flag of Timer 1n Capture Interrupt
 *----------------------------------------------------------------------------*/
void TIMER1nCapInt_ClrFg(TIMER1n_Type *TIMER1n)
{
	TIMER1n->CR |= (1<<2);
}

