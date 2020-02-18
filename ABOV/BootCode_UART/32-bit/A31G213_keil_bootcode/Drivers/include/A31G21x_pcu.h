/**********************************************************************
* @file		A31G21x_pcu.h
* @brief	Contains all macro definitions and function prototypes
* 			support for PCU firmware library on A31G21x
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2017, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

#ifndef _PCU_H_
#define _PCU_H_

/* Includes ------------------------------------------------------------------- */
#include "A31G21x.h"
#include "aa_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Private macros ------------------------------------------------------------- */
#define PORT_ACCESS_EN()    do { PCU->PORTEN=0x15; PCU->PORTEN=0x51; } while(0)
#define PORT_ACCESS_DIS()  do { PCU->PORTEN=0x00; } while(0) 

#define PCU_FUNC_Msk                        (0x0FUL)                            
#define PCU_MODE_Msk                        (0x03UL)     

/* ================================================================================ */
/* ================          struct 'PCU' Position & Mask          ================ */
/* ================================================================================ */


/* Pin function */
#define FUNC0 			0x0				/** Function 0 	*/
#define FUNC1 			0x1				/** Function 1 	*/
#define FUNC2 			0x2				/** Function 2	*/
#define FUNC3 			0x3				/** Function 3	*/
#define FUNC4 			0x4				/** Function 4	*/


/*
 * @brief 	PCU port mode enumerate definition
 */
typedef enum {
	INPUT=0,
	PUSH_PULL_OUTPUT,
	ALTERN_FUNC,	
	OPEN_DRAIN_OUTPUT
}PCU_PORT_MODE;


#define DISPUPD			0x0				/** disable pu_pd */
#define ENPU   			0x1				/** enable pull up */
#define ENPD   	 		0x2				/** enable pull down */


enum {
	IER_DISABLE = 0,
	IER_LEVEL,
	IER_EDGE = 3
};

enum {
	ICR_PROHIBIT_INT = 0,
	ICR_LOW_LEVEL_INT = 1,
	ICR_FALLING_EDGE_INT = ICR_LOW_LEVEL_INT,
	ICR_HIGH_LEVEL_INT =2,
	ICR_RISING_EDGE_INT = ICR_HIGH_LEVEL_INT,
	ICR_BOTH_EDGE_INT =3	
};


/* Public Functions ----------------------------------------------------------- */
void GPIO_SetValue(PORT_Type *Px, uint16_t bitValue);
void GPIO_ClearValue(PORT_Type *Pn, uint16_t bitValue);
uint16_t GPIO_ReadValue(PORT_Type *Pn);
void GPIO_WriteValue(PORT_Type *Pn, uint16_t Value);

void PCU_ConfigureFunction(PORT_Type *PCx, uint8_t pin_no, uint32_t func);
void PCU_SetDirection(PORT_Type *PCx, uint8_t pin_no, PCU_PORT_MODE dir_type);
void PCU_ConfigurePullupdown (PORT_Type *PCx, uint8_t pin_no, uint8_t pullupdown);
void PCU_ConfigureOutDataMask (PORT_Type *PCx, uint8_t pin_no, FunctionalState maskctrl);

void GPIO_IntCmd(PORT_Type *PCx, uint8_t pin_no, uint8_t pin_en, uint8_t int_mode);
uint32_t GPIO_GetIntStatus(PORT_Type *PCx);
void GPIO_ClearInt(PORT_Type *PCx, uint32_t value);

void PCU_SetDebounce(PORT_Type *Pn, uint32_t u32Pins);
#ifdef __cplusplus
}
#endif

#endif /* end _PCU_H_ */

/* --------------------------------- End Of File ------------------------------ */
