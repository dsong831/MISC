/**********************************************************************
* @file		A31G216X_pwr.h
* @brief	Contains all macro definitions and function prototypes
* 			support for Power Control firmware library on A31G216X
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2015, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

#ifndef _A31G216X_PWR_H_
#define _A31G216X_PWR_H_

/* Includes ------------------------------------------------------------------- */
#include "A31G21x.h"
#include "aa_types.h"



/* LVI Definition ------------------------------------------------------------- */
#define LVI_DISABLE					(0x0uL<<7)
#define LVI_ENABLE					(0x1uL<<7)

#define LVI_INTDIS					(0x0uL<<5)
#define LVI_INTEN					(0x1uL<<5)

#define LVI_LVL_1_88V				(0x3uL<<0)
#define LVI_LVL_2_00V				(0x4uL<<0)
#define LVI_LVL_2_13V				(0x5uL<<0)
#define LVI_LVL_2_28V				(0x6uL<<0)
#define LVI_LVL_2_46V				(0x7uL<<0)
#define LVI_LVL_2_67V				(0x8uL<<0)
#define LVI_LVL_3_04V				(0x9uL<<0)
#define LVI_LVL_3_20V				(0xAuL<<0)
#define LVI_LVL_3_55V				(0xBuL<<0)
#define LVI_LVL_3_75V				(0xCuL<<0)
#define LVI_LVL_3_99V				(0xDuL<<0)
#define LVI_LVL_4_25V				(0xEuL<<0)
#define LVI_LVL_4_55V				(0xFuL<<0)




typedef struct {
	uint32_t	EnSel;
	uint32_t	IntSel;
	
	uint32_t	LvlSel;
} LVI_CFG_Type;



#ifdef __cplusplus
extern "C"
{
#endif

/* Clock Generator */
void PWR_Sleep(void);
void PWR_PowerDown(void);
void LVI_Init(LVI_CFG_Type *LVI_ConfigStruct);


#ifdef __cplusplus
}
#endif

#endif /* _A31G216X_PWR_H_ */
/* --------------------------------- End Of File ------------------------------ */
