/**********************************************************************//**
* @file				A31T21x_hal_pcu.c
* @brief			Contains all functions support for firmware library on A31T21x
* @version	1.00
* @date			29. JUNE. 2020
* @author	ABOV M team
*
* Copyright(C) 2019, ABOV Semiconductor
* All rights reserved.
*
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


/* Includes ------------------------------------------------------------ */
#include "A31T21x_hal_pcu.h"


/**********************************************************************//**
 * @brief						Configure Pin Mode
 * @param[in]	PCx	Pointer to selected PCU peripheral, should be: A,B,C,D,E,F
 * @param[in]	pin_no				Pin number, should be: 0..15
 * @param[in]	pin_mode		Pin mode/type, should be:
 *											- INPUT														: 0
 *											- PUSH_PULL_OUTPUT		: 1
 *											- ALTERN_FUNC								: 2
 *											- OPEN_DRAIN_OUTPUT	: 3
 * @return				None
 **********************************************************************/
void HAL_GPIO_ModeConfig(PORT_Type *Pn, uint8_t pin_no, PIN_MODE_Type pin_mode)
{
	uint8_t pin_offset;
	uint32_t reg32;
	uint32_t output_type;

	/* Cal. Pin Offset */
	output_type = pin_mode;
	if(output_type == OPEN_DRAIN_OUTPUT)
	{
		pin_mode = PUSH_PULL_OUTPUT;
	}
	pin_offset = (pin_no <<1);		// pin_offset = pin_no *2

	/* Port Mode Set */
	reg32 = Pn->MOD; 
	reg32 &= ~(PORT_MODE_MASK << pin_offset);
	reg32 |= (pin_mode << pin_offset); 
	Pn->MOD = reg32;

	/* Output Type Set */
	reg32 = Pn->TYP;
	reg32 &= ~(1 << pin_no);
	if(output_type == OPEN_DRAIN_OUTPUT)
	{
		reg32 |= (1 << pin_no);
	}
	Pn->TYP = reg32;
}

/**********************************************************************//**
 * @brief						Configure Alternative Function
 * @param[in]	PCx	Pointer to selected PCU peripheral, should be: A,B,C,D,E,F
 * @param[in]	pin_no		Pin number, should be: 0..15
 * @param[in]	func_no	Function number, should be:
 *											- ALTERN_FUNC0		:		AF0
 *											- ALTERN_FUNC1		:		AF1
 *											- ALTERN_FUNC2		:		AF2
 *											- ALTERN_FUNC3		:		AF3
 *											- ALTERN_FUNC4		:		AF4
 *											- ALTERN_FUNC5		:		AF5
 * @return				None
 **********************************************************************/
void HAL_GPIO_AlternativeConfig(PORT_Type *Pn, uint8_t pin_no, uint32_t func_no)
{
	uint8_t pin_offset1;
	uint8_t pin_offset2;
	uint32_t reg32;
	
	/* Set Alternative function selection register */
	if(pin_no < 8)	// Pin0~Pin7
	{
		pin_offset1 = (pin_no << 2);	// pin_offset = pin_no * 4

		reg32 = Pn->AFSR1;
		reg32 &= ~(PORT_FUNC_MASK << pin_offset1);
		reg32 |= (func_no << pin_offset1);

		Pn->AFSR1 = reg32;
	}
	else		// Pin8~Pin15
	{
		pin_offset2 = (pin_no << 2);	// pin_offset = pin_no * 4

		reg32 = Pn->AFSR2;
		reg32 &= ~(PORT_FUNC_MASK << pin_offset2);
		reg32 |= (func_no << pin_offset2);

		Pn->AFSR2 = reg32;
	}
}

/**********************************************************************//**
 * @brief						Configure pin pull-up/down
 * @param[in]	PCx	Pointer to selected PCU peripheral, should be: A,B,C,D,E,F
 * @param[in]	pin_no			pin number, should be: 0..15
 * @param[in]	pupd_sel	pull-up/down state, should be:
 *											- 0: PUPD_DISABLE
 *											- 1: PULLUP_ENABLE
 *											- 2: PULLDOWN_ENABLE
 *											- 3: ANALOG_INPUT
 * @return				None
 **********************************************************************/
void HAL_GPIO_PUPDConfig(PORT_Type *Pn, uint8_t pin_no, uint8_t pupd_sel)
{
	uint8_t pin_offset;
	uint32_t reg32; 

	/* Cal. Pin Offset */
	pin_offset = (pin_no << 1);		// pin_offset = pin_no *2
	
	/* Set PUPD register */
	reg32 = Pn->PUPD;
	reg32 &= ~(PORT_PUPD_MASK << pin_offset); 
	reg32 |= (pupd_sel << pin_offset);
	Pn->PUPD = reg32;
}

/**********************************************************************//**
 * @brief						Read Current state on port pin that have input direction of GPIO
 * @param[in]	Pn selected port, should be in range from PA to PF
 * @return				Current value of GPIO port.
 *
 * Note: Return value contain state of each port pin (bit) on that GPIO regardless
 * its direction is input or output.
 **********************************************************************/
uint16_t HAL_GPIO_ReadPin(PORT_Type *Pn)
{
	return Pn->INDR;
}

/**********************************************************************//**
 * @brief						Write Value on port that have output direction of GPIO
 * @param[in]	Pn selected port, should be in range from PA to PF
 * @param[in]	bitValue Value that contains all bits on GPIO to set, should
 *											be in range from 0 to 0xFF.
 *											example: value 0x3 to set bit 0 and bit 1.
 * @return				None
 **********************************************************************/
void HAL_GPIO_WritePin(PORT_Type *Pn, uint16_t Value)
{
	Pn->OUTDR = Value;
}

/**********************************************************************//**
 * @brief						Set Value for bits that have output direction on GPIO port.
 * @param[in]	Pn selected port, should be in range from PA to PF
 * @param[in]	bitValue Value that contains all bits on GPIO to set, should
 *											be in range from 0 to 0xFF.
 *											example: value 0x3 to set bit 0 and bit 1.
 * @return				None
 **********************************************************************/
void HAL_GPIO_SetPin(PORT_Type *Pn, uint16_t bitValue)
{
	Pn->BSR |= (1 << bitValue);
}


/**********************************************************************//**
 * @brief						Clear Value for bits that have output direction on GPIO port.
 * @param[in]	Pn selected port, should be in range from PA to PF
 * @param[in]	bitValue Value that contains all bits on GPIO to clear, should
 *											be in range from 0 to 0xFF.
 *											example: value 0x3 to clear bit 0 and bit 1.
 * @return				None
 **********************************************************************/
void HAL_GPIO_ClearPin(PORT_Type *Pn, uint16_t bitValue)
{
	Pn->BCR |= (1 << bitValue);
}

/**********************************************************************//**
 * @brief						Configure out data Mask
 * @param[in]	PCx	Pointer to selected PCU peripheral, should be: A,B,C,D,E,F
 * @param[in]	pin_no		Pin number, should be: 0..15
 * @param[in]	mask_sel state, should be:
 *											- 0: DISABLE(Unmask)-changed 
 *											- 1: ENABLE(Mask)-protected
 * @return				None
 **********************************************************************/
void HAL_GPIO_OutDataMaskConfig(PORT_Type *Pn, uint8_t pin_no, uint8_t mask_sel)
{
	uint32_t reg32; 

	reg32 = Pn->OUTDMSK;
	reg32 &= ~(1 << pin_no);
	reg32 |= (mask_sel << pin_no);
	Pn->OUTDMSK = reg32;
}

/**********************************************************************//**
 * @brief						Set Port Debounce Function
 * @param[in]	PCx		Pointer to selected PCU peripheral, should be: A,B,C,D,E,F
 * @param[in]	pin_no		Pin number, should be: 0..15
 * @return				None
 **********************************************************************/
void HAL_GPIO_SetDebouncePin(PORT_Type *Pn, uint8_t pin_no)
{
	uint32_t reg32;

	reg32 = (Pn->DBCR & 0x00FF);
	reg32 |= (0x01 << pin_no);
	Pn->DBCR = reg32;
}

/**********************************************************************//**
 * @brief						Configure GPIO interrupt
 * @param[in]	Pn	Pointer to selected PCU peripheral, should be: A,B,C,D,E,F
 * @param[in]	pin_no		Pin number, should be: 0..15
* @param[in]	eint_en		external interrupt enable/disable, should be:
 *											- IER_DISABLE_EINT = 0
 *											- IER_LEVEL_EINT = 1
 *											- IER_EDGE_EINT = 3				
 * @param[in]	eint_mode		external interrupt mode, should be:
 *											- ICR_PROHIBIT_INT = 0
 *											if level
 *											- ICR_LOW_LEVEL_INT = 1
 *											- ICR_HIGH_LEVEL_INT = 2
 *											if edge
 *											- ICR_FALLING_EDGE_INT = 1
 *											- ICR_RISING_EDGE_INT = 2
 *											- ICR_BOTH_EDGE_INT = 3
 * @return				None
 **********************************************************************/
void HAL_GPIO_EXTI_Config(PORT_Type *Pn, uint8_t pin_no, uint8_t eint_en, uint8_t eint_mode)
{
	uint8_t pin_offset;
	uint32_t reg32;

	/* Cal. Pin Offset */
	pin_offset = (pin_no << 1);		// pin_offset = pin_no * 2

	/* Set interrupt enable register */
	reg32 = Pn->IER & (uint32_t)(~(3 << pin_offset));
	reg32 |= (uint32_t)(eint_en << pin_offset);	
	Pn->IER = reg32;

	/* Set interrupt control register */
	if(eint_en)
	{
		reg32 = Pn->ICR & (uint32_t)(~(3 << pin_offset));
		reg32 |= (uint32_t)(eint_mode << pin_offset);
		Pn->ICR = reg32;
	}
	else
	{
		Pn->ICR = Pn->ICR & (uint32_t)(~(3 << pin_offset));
	}
}

/**********************************************************************//**
 * @brief						Get GPIO Interrupt Status
 * @param[in]	Pn	Pointer to selected PCU peripheral, should be: A,B,C,D,E,F
 * @return				ISR register value 
 **********************************************************************/
uint32_t HAL_GPIO_EXTI_GetStatus(PORT_Type *Pn)
{
	return (Pn->ISR);
}

/**********************************************************************//**
 * @brief						Clear GPIO interrupt status
 * @param[in]	Pn	Pointer to selected PCU peripheral, should be: A,B,C,D,E,F
 * @param[in]	pin_no		Pin number, should be: 0..15
 * @return				None
 **********************************************************************/
void HAL_GPIO_EXTI_ClearStatus(PORT_Type *Pn, uint8_t pin_no)
{
	uint8_t pin_offset;

	/* Cal. Pin Offset */
	pin_offset = (pin_no << 1);		// pin_offset = pin_no * 2

	/* Clear interrupt flag */
	Pn->ISR |= (uint32_t)(3 << pin_offset);
}


//////////////////////////////////////////////////////////////////////////////////////////
// Previous PCU Initial Driver
//////////////////////////////////////////////////////////////////////////////////////////

/**********************************************************************//**
* @brief		Initialize GPIO(PCU) peripheral
* @param      	PCx : Pointer to selected PCU peripheral, should be: A,B,C,F
* @param      	GPIO_Init : Pointer contains configuration of GPIO
* @return		None
 **********************************************************************/
void HAL_GPIO_Init(PORT_Type *PCx, GPIO_CFG_Type *GPIO_Init)
{
	uint8_t pin_offset; 
	uint32_t reg_val; 
	uint32_t dir_type_temp;

	PORT_ACCESS_EN();	

	//Function
      if (GPIO_Init->Pin<16){ // 0~15
		//------------------------------------------------------------------------------
		// pin_offset = pin_no * 4
		//------------------------------------------------------------------------------
		pin_offset = (GPIO_Init->Pin << 2); 

		//------------------------------------------------------------------------------
		// MR
		//------------------------------------------------------------------------------
		reg_val = PCx->AFSR1;
		reg_val &= ~(PCU_FUNC_Msk << pin_offset); 
		reg_val |= (GPIO_Init->Function << pin_offset); 

		PCx->AFSR1=reg_val;
	}
	/* Output */
	
	dir_type_temp = GPIO_Init->Output;
	if (dir_type_temp==OPEN_DRAIN_OUTPUT)
		 GPIO_Init->Output = PUSH_PULL_OUTPUT;
	//------------------------------------------------------------------------------
	// pin_offset = pin_no * 2
	//------------------------------------------------------------------------------
	pin_offset = (GPIO_Init->Pin << 1); 
	
	PORT_ACCESS_EN();
	
	//------------------------------------------------------------------------------
	// PnMOD
	//------------------------------------------------------------------------------
	reg_val = PCx->MOD; 
	reg_val &= ~(PCU_MODE_Msk << pin_offset); 
	reg_val |= ( GPIO_Init->Output << pin_offset); 
	PCx->MOD=reg_val; 

	//------------------------------------------------------------------------------
	// PnTYP
	//------------------------------------------------------------------------------
	reg_val = PCx->TYP; 
	reg_val &= ~(1 << GPIO_Init->Pin); 	
	if (dir_type_temp==OPEN_DRAIN_OUTPUT){
		reg_val |= (1 << GPIO_Init->Pin); 
	}
	PCx->TYP=reg_val; 

	/*pullup*/
	pin_offset = (GPIO_Init->Pin << 1); 

	reg_val = PCx->PUPD; 
	reg_val &= ~(3<<pin_offset); 
	reg_val |= (GPIO_Init->Pull<<pin_offset); 
	PCx->PUPD=reg_val;
}

/**********************************************************************//**
 * @brief 		Configure pin function
 * @param[in]	PCx	Pointer to selected PCU peripheral, should be: A,B,C,D,E,F
 * @param[in]	pin_no		Pin number, should be: 0..15
 * @param[in]	func 	Function mode, should be:
 * 					- FUNC0		:Function 0
 * 					- FUNC1		:Function 1
 * 					- FUNC2		:Function 2
 * 					- FUNC3		:Function 3
 * 					- FUNC4		:Function 4 
 * @return		None
 **********************************************************************/
void HAL_GPIO_ConfigFunction(PORT_Type *PCx, uint8_t pin_no, uint32_t func)
{
	uint8_t pin_offset; 
	uint32_t reg_val; 
	
	PORT_ACCESS_EN();	
	
	if (pin_no<16){ // 0~15
		//------------------------------------------------------------------------------
		// pin_offset = pin_no * 4
		//------------------------------------------------------------------------------
		pin_offset = (pin_no << 2); 

		//------------------------------------------------------------------------------
		// MR
		//------------------------------------------------------------------------------
		reg_val = PCx->AFSR1;
		reg_val &= ~(PCU_FUNC_Msk << pin_offset); 
		reg_val |= (func << pin_offset); 

		PCx->AFSR1=reg_val;
	}
}

/**********************************************************************//**
 * @brief 		Configure pin mode
 * @param[in]	PCx	Pointer to selected PCU peripheral, should be: A,B,C,D,E,F
 * @param[in]	pin_no		Pin number, should be: 0..15
 * @param[in]	dir_type	Pin mode, should be:
 * 					- INPUT                           :0
 * 					- PUSH_PULL_OUTPUT   :1
 * 					- ALTERN_FUNC              :2
 * 					- Reserved :3
 * @return		None
 **********************************************************************/
void HAL_GPIO_ConfigOutput(PORT_Type *PCx, uint8_t pin_no, PIN_MODE_Type dir_type)
{
	uint8_t pin_offset; 
	uint32_t reg_val; 
	uint32_t dir_type_temp;
	
	dir_type_temp = dir_type;
	if (dir_type_temp==OPEN_DRAIN_OUTPUT)
		dir_type = PUSH_PULL_OUTPUT;
	//------------------------------------------------------------------------------
	// pin_offset = pin_no * 2
	//------------------------------------------------------------------------------
	pin_offset = (pin_no << 1); 
	
	PORT_ACCESS_EN();
	
	//------------------------------------------------------------------------------
	// MOD
	//------------------------------------------------------------------------------
	reg_val = PCx->MOD; 
	reg_val &= ~(PCU_MODE_Msk << pin_offset); 
	reg_val |= (dir_type << pin_offset); 
	PCx->MOD=reg_val; 

	//------------------------------------------------------------------------------
	// TYP
	//------------------------------------------------------------------------------
	reg_val = PCx->TYP; 
	reg_val &= ~(1 << pin_no); 	
	if (dir_type_temp==OPEN_DRAIN_OUTPUT){
		reg_val |= (1 << pin_no); 
	}
	PCx->TYP=reg_val;
}

/**********************************************************************//**
 * @brief 		Configure pin pullup
 * @param[in]	PCx	Pointer to selected PCU peripheral, should be: A,B,C,D,E,F
 * @param[in]	pin_no		Pin number, should be: 0..15
 * @param[in]	pullupdown state, should be:
 * 					- 0: DISABLE
 * 					- 1: Enable pull-up resistor 		
 * 					- 2: Enable pull-down resistor 
 * 					- 3: reserved 
 * @return		None
 **********************************************************************/
void HAL_GPIO_ConfigPullup (PORT_Type *PCx, uint8_t pin_no, uint8_t pullupdown)
{
	uint32_t	reg_val; 
	uint8_t pin_offset; 
	
	pin_offset = (pin_no << 1); 
	
	PORT_ACCESS_EN();
	
	reg_val = PCx->PUPD; 
	reg_val &= ~(3<<pin_offset); 
	reg_val |= (pullupdown<<pin_offset); 
	PCx->PUPD=reg_val;
}


/* --------------------------------- End Of File ------------------------------ */
