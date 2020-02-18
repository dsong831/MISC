/**********************************************************************
* @file		A31G21x_crc.h
* @brief	Contains all macro definitions and function prototypes
* 			support for ADC firmware library on A31G21x
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2015, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

#ifndef _CRCCHECK_H_
#define _CRCCHECK_H_

/* Includes ------------------------------------------------------------------- */
#include "A31G21x.h"
#include "aa_types.h"


#ifdef __cplusplus
extern "C"
{
#endif
//-------- CRC/Checksum Interrupt Enable Constant Definition ------------
#define CRC_INTR_DIS                  (0x0uL<<9)
#define CRC_INTR_EN                   (0x1uL<<9)

//-------- CRC/Checksum Interrupt Flag Constant Definition ------------
#define CRC_FLAG_OFF                  (0x0uL<<8)
#define CRC_FLAG	                  (0x1uL<<8)

//-------- CRC/Checksum User/Auto Mode Constant Definition ------------
#define CRC_USER_M                    (0x0uL<<7)
#define CRC_AUTO_M                    (0x1uL<<7)

//------------ CRC/Checksum Selection Constant Definition -------------
#define CRC_CRC                       (0x0uL<<5)
#define CRC_CHECKSUM                  (0x1uL<<5)

//---------------- CRC Polynominal Constant Definition ----------------
#define CRC_CCITT                     (0x0uL<<4)
#define CRC_16                        (0x1uL<<4)

//--- CRC/Checksum Start Address Auto Increment Constant Definition ---
#define CRC_NOINC                     (0x0uL<<3)
#define CRC_AUTOINC                   (0x1uL<<3)

//-------- CRC/Checksum 1st Shifted-in Bit Constant Definition --------
#define CRC_MSB                       (0x0uL<<1)
#define CRC_LSB                       (0x1uL<<1)

/*----------------------------------------------------------------------------
   @brief        CRC In Data
   @param[in]    u32InData          CRC/Checksum Input Data
   @explain      This macro puts input data for calculation
 *----------------------------------------------------------------------------*/
#define CRC_InData(u32InData)                  (CRC->IN = u32InData)

/*----------------------------------------------------------------------------
   @brief        CRC/Checksum Run
   @param[in]    None
   @explain      This macro starts CRC/Checksum calculation
 *----------------------------------------------------------------------------*/
#define CRCRun()								               {CRC->CR |= (1<<0); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();}

/*----------------------------------------------------------------------------
   @brief        CRC/Checksum Stop
   @param[in]    None
   @explain      This macro stops forcingly CRC/Checksum calculation
 *----------------------------------------------------------------------------*/
#define CRCStop()								               (CRC->CR &= ~(1<<0))

/*----------------------------------------------------------------------------
   @brief        CRC/Checksum Finish Check
   @param[in]    None
   @explain      This macro checks CRC/Checksum finish
 *----------------------------------------------------------------------------*/
#define ChkCRCFinish()                         (CRC->CR&0x1)
 
//------------------------- Internal Functions ------------------------
void CRC_Init(void);
uint32_t CRC_AutoGet(uint32_t u32SEL, uint32_t u32POLY, uint32_t u32FirstBit);
void CRC_UserSet(uint32_t u32SEL, uint32_t u32POLY, uint32_t u32FirstBit);
uint32_t CRC_UserInput(uint32_t u32Input);
uint32_t CRC_GetData(void);
void CRC_SetINIT(uint32_t InitVal);
void CRC_INTCmd(uint32_t IntSel);
uint32_t CRC_GetINTStatus(void);
void CRC_ClearINTStatus(void);

#ifdef __cplusplus
}
#endif


#endif /* _CRCCHECK_H_ */

/* --------------------------------- End Of File ------------------------------ */
