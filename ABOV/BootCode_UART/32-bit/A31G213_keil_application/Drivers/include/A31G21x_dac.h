/**********************************************************************
* @file		A31G21x_dac.h
* @brief	Contains all macro definitions and function prototypes
* 			support for DAC firmware library on A31G21x
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2015, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

#ifndef _DAC_H_
#define _DAC_H_

/* Includes ------------------------------------------------------------------- */
#include "A31G21x.h"
#include "aa_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

//-------- DAC Interrupt Enable/Disable Definition ------------
#define DAC_INTDIS                    (0x0uL<<7)
#define DAC_INTEN                    (0x1uL<<7)

	
//-------- DAC Automatically Data Incement/Decrement ------------
#define DAC_AUTOCHANGEDIS				(0x0uL<<5)
#define DAC_AUTOCHANGEEN				(0x1uL<<5)

//--------- DAC Control Definition -------------------
#define DAC_REFAVDD					(0x0uL)
#define DAC_REFAVREF				(0x1uL)

#define DAC_BUFFCLR					(0x1uL)

#define DAC_FETCHDIS				(0x0uL)
#define DAC_FETCHEN					(0x1uL)

#define DAC_RLDS_ALWAYS				(0x0uL)
#define DAC_RLDS_FADPCM				(0x1uL)
#define DAC_RLDS_TIMER10			(0x2uL)
#define DAC_RLDS_TIMER11			(0x3uL)

#define DAC_STOP					(0x0uL)
#define DAC_START					(0x1uL)


//-------- DAC Programmable Gain Control Definition ------------
#define DAC_PGAn30dB				(0x0uL)
#define DAC_PGAn24dB				(0x1uL)
#define DAC_PGAn18dB				(0x2uL)
#define DAC_PGAn12dB				(0x3uL)
#define DAC_PGAn6dB					(0x4uL)
#define DAC_PGAn0dB					(0x5uL)
#define DAC_PGAp6dB					(0x6uL)
#define DAC_PGAp12dB				(0x7uL)
#define DAC_PGAp18dB				(0x8uL)
#define DAC_PGAp24dB				(0x9uL)
#define DAC_PGAp30dB				(0xAuL)


//-------- DAC Converter Offset Control Definition ------------
#define DAC_OFFSETDIS				(0x0uL)
#define DAC_OFFSETEN				(0x1uL)
#define DAC_OFFSETDIRn				(0x0uL)
#define DAC_OFFSETDIRp				(0x1uL)
#define DAC_OFFSETVAL(n)			(((n)&0xF))


//-------- DAC Public Types -------------------------------------
/** When DONE is 1, this field contains result value of DAC conversion */
#define DAC_DR_RESULT(n)			((n)&0xFFFF)

typedef struct {
	// CR
	uint32_t	RefSel;
	uint32_t	IntSel;
	uint32_t	SoftOnOff;
	uint32_t	FetchSel;
	uint32_t	ReloadSel;
	
	// PGSR
	uint32_t	PGASel;
	
	// DAOFSCR
	uint32_t	OffsetSel;
	uint32_t	OffsetDIRSel;
	uint32_t	OffsetVal;
}DAC_CFG_Type;



/* Public Functions ----------------------------------------------------------- */
/* Init/DeInit DAC peripheral ----------------*/
void DAC_Init(DAC_Type *DACx, DAC_CFG_Type *DAC_ConfigStruct);
void DAC_DeInit(DAC_Type *DACx);
uint16_t	DAC_GetData(DAC_Type *DACx);
void	DAC_SetData(DAC_Type *DACx, uint16_t data_value);
void DAC_StartCmd(DAC_Type *DACx, FunctionalState NewState);
void DAC_IntConfig(DAC_Type *DACx, FunctionalState NewState);
uint32_t	DAC_GetStatus(DAC_Type *DACx);
void DAC_ClearStatus(DAC_Type *DACx);
uint16_t	DAC_GetBuffer(DAC_Type *DACx);
void DAC_SetOffset(uint32_t enable, uint32_t dir, uint32_t value);
void DAC_SetGain(uint32_t gain);

void DAC_EnableSoftOnOff(DAC_Type *DACx, FunctionalState NewState);
void DAC_EnableADPCMFetch(DAC_Type *DACx, FunctionalState NewState);
void DAC_SelReloadSrc(DAC_Type *DACx, uint32_t sel);


#ifdef __cplusplus
}
#endif


#endif /* _DAC_H_ */

/* --------------------------------- End Of File ------------------------------ */

