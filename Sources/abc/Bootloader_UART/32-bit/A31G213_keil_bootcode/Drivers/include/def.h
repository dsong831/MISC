/**********************************************************************
* @file		def.h
* @brief	general def header file
* @version	1.0
* @date: 	2017-02-24
* @author	ABOV Application Team
*
* Copyright(C) 2017, ABOV Semiconductor. All rights reserved.
**********************************************************************/

#ifndef _DEF_H_
#define _DEF_H_

#ifdef __cplusplus
extern "C"
{
#endif
	
#include "aa_types.h"


//=========================================================================
//	constants
//=========================================================================
#define ASCII_BACKSPACE					(0x08)
#define ASCII_LINEFEED					(0x0A)
#define ASCII_CARRIAGE_RETURN			(0x0D)




	
	
typedef	unsigned	char					u8;
typedef	unsigned	short					u16;
typedef	unsigned	long					u32;

typedef	char										s8;
typedef	short										s16;
typedef	long										s32;

typedef	volatile unsigned char	vu8;
typedef	volatile unsigned short	vu16;
typedef	volatile unsigned long	vu32;

typedef	volatile char						vs8;
typedef	volatile short					vs16;
typedef	volatile long						vs32;

#define	DI()										__disable_irq()											//__asm ("CPSID I")
#define	EI()										__enable_irq()											//__asm ("CPSIE I")

#define NOP()            				__nop()															//asm("nop")
#define WFI()										__wfi()
#define WFE()										__wfe()
#define APB_NOP         				{unsigned long tmp = CIDR;}

#define MASTER									1
#define	SLAVE										0


#ifdef __cplusplus
}
#endif

#endif /* _DEF_H_ */

