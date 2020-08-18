/**********************************************************************//**
* @file				A31T21x_hal_crc.h
* @brief			Contains all functions support for firmware library on A31T21x
* @version	1.00
* @date			26. JUNE. 2020
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
*ABOV assumes no liability for application assistance or the design of purchasers?? products. No license, 
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


#ifndef A31T21X_HAL_CRC_H
#define A31T21X_HAL_CRC_H


/* Includes -------------------------------------------------------------- */
#include "A31T21x.h"
#include "A31T21x_hal_aa_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Private macros -------------------------------------------------------- */
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
HAL_Status_Type HAL_CRC_Init(void);
uint32_t HAL_CRC_ConfigAutoMode(uint32_t u32SEL, uint32_t u32POLY, uint32_t u32FirstBit);
HAL_Status_Type HAL_CRC_ConfigUserMode(uint32_t u32SEL, uint32_t u32POLY, uint32_t u32FirstBit);
uint32_t HAL_CRC_UserInput(uint32_t u32Input);
uint32_t HAL_CRC_GetData(void);
HAL_Status_Type HAL_CRC_SetUserInput(uint32_t InitVal);
HAL_Status_Type HAL_CRC_ConfigInterrupt(uint32_t IntSel);
uint32_t HAL_CRC_GetState_IT(void);
HAL_Status_Type HAL_CRC_ClearState_IT(void);

#ifdef __cplusplus
}
#endif


#endif		/* A31T21X_HAL_CRC_H */


/* --------------------------------- End Of File ------------------------------ */

