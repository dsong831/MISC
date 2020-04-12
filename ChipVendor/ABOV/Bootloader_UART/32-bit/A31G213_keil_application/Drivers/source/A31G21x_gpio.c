/**********************************************************************
* @file		A31G216X_gpio.c
* @brief	Contains all functions support for GPIO firmware library
* 			on A31G216X
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2015, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
#include "A31G314_gpio.h"

/* GPIO ------------------------------------------------------------------------------ */

/**********************************************************************
 * @brief		Set Value for bits that have output direction on GPIO port.
 * @param[in]	Px selected port, should be in range from PA to PF
 * @param[in]	bitValue Value that contains all bits on GPIO to set, should
 * 				be in range from 0 to 0xFFFF.
 * 				example: value 0x5 to set bit 0 and bit 1.
 * @return		None
 *
 * Note:
 * - For all bits that has been set as input direction, this function will
 * not effect.
 * - For all remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void GPIO_SetValue(PCU_Type *Px, uint16_t bitValue)
{
	Px->PnBSR = bitValue;
}


/**********************************************************************
 * @brief		Clear Value for bits that have output direction on GPIO port.
 * @param[in]	Px selected port, should be in range from PA to PF
 * @param[in]	bitValue Value that contains all bits on GPIO to clear, should
 * 				be in range from 0 to 0xFFFF.
 * 				example: value 0x5 to clear bit 0 and bit 1.
 * @return		None
 *
 * Note:
 * - For all bits that has been set as input direction, this function will
 * not effect.
 * - For all remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void GPIO_ClearValue(PCU_Type *Px, uint16_t bitValue)
{
	Px->PnBCR = bitValue;
}


/**********************************************************************
 * @brief		Read Current state on port pin that have input direction of GPIO
 * @param[in]	Px selected port, should be in range from PA to PF
 * @return		Current value of GPIO port.
 *
 * Note: Return value contain state of each port pin (bit) on that GPIO regardless
 * its direction is input or output.
 **********************************************************************/
uint16_t GPIO_ReadValue(PCU_Type *Px)
{
	return Px->PnINDR;
}

/**********************************************************************
 * @brief		Write Value on port that have output direction of GPIO
 * @param[in]	Px selected port, should be in range from PA to PF
 * @return		None
 **********************************************************************/
void GPIO_WriteValue(PCU_Type *Px, uint16_t Value)
{
	Px->PnOUTDR = Value;
}

/* --------------------------------- End Of File ------------------------------ */
