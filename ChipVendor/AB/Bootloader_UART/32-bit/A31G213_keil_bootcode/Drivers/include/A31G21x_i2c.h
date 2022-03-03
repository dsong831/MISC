/**********************************************************************
* @file		A31G216X_i2c.h
* @brief	Contains all macro definitions and function prototypes
* 			support for I2C firmware library on A31G216X
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2015, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

#ifndef _I2C_H_
#define _I2C_H_

/* Includes ------------------------------------------------------------------- */
#include "A31G21x.h"
#include "aa_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

#if defined(__ARMCC_VERSION)
  #pragma anon_unions
#endif

/** I2C Slave Address registers bit mask */
#define I2C_I2ADR_BITMASK			((0xFF))

/* I2C state handle return values */
#define RECEIVE_MODE		1
#define TRANS_MODE			2
#define RECEIVE_DATA		3
#define TRANS_DATA			4
#define LOST_BUS				5
#define STOP_DECT			6


/* Public Types --------------------------------------------------------------- */
/**
 * @brief Master transfer setup data structure definitions
 */
typedef struct
{
  uint32_t          sl_addr7bit;				/**< Slave address in 7bit mode */
  uint8_t*          tx_data;					/**< Pointer to Transmit data - NULL if data transmit is not used */
  uint32_t          tx_length;					/**< Transmit data length - 0 if data transmit is not used*/
  uint32_t          tx_count;					/**< Current Transmit data counter */
  uint8_t*          rx_data;					/**< Pointer to Receive data - NULL if data receive is not used */
  uint32_t          rx_length;					/**< Receive data length - 0 if data receive is not used */
  uint32_t          rx_count;					/**< Current Receive data counter */
} I2C_M_SETUP_Type;

/**
 * @brief Slave transfer setup data structure definitions
 */
typedef struct
{
  uint8_t*          tx_data;					/**< Pointer to transmit data - NULL if data transmit is not used */
  uint32_t          tx_length;					/**< Transmit data length - 0 if data transmit is not used */
  uint32_t          tx_count;					/**< Current transmit data counter	*/
  uint8_t*          rx_data;					/**< Pointer to receive data - NULL if data received is not used */
  uint32_t          rx_length;					/**< Receive data length - 0 if data receive is not used */
  uint32_t          rx_count;					/**< Current receive data counter */
} I2C_S_SETUP_Type;

/**
 * @brief Transfer option type definitions
 */
typedef enum {
	I2C_TRANSFER_POLLING = 0,		/**< Transfer in polling mode */
	I2C_TRANSFER_INTERRUPT			/**< Transfer in interrupt mode */
} I2C_TRANSFER_OPT_Type;


/* Public Functions ----------------------------------------------------------- */

/* I2C Init/DeInit functions ---------- */
void I2C_Init(I2C_Type *I2Cx, uint32_t clockrate);
void I2C_DeInit(I2C_Type* I2Cx);

/* I2C transfer data functions -------- */
Status I2C_MasterTransferData(I2C_Type *I2Cx, I2C_M_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt);
Status I2C_SlaveTransferData(I2C_Type *I2Cx, I2C_S_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt);
uint32_t I2C_MasterTransferComplete(I2C_Type *I2Cx);
uint32_t I2C_SlaveTransferComplete(I2C_Type *I2Cx);

void I2C_Interrupt_MasterHandler(I2C_Type  *I2Cx);
void I2C_Interrupt_SlaveHandler (I2C_Type  *I2Cx);

Status	I2C_MasterTransmitData(I2C_Type* I2Cx, I2C_M_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt);
Status	I2C_MasterReceiveData(I2C_Type* I2Cx, I2C_M_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt);
Status	I2C_SlaveReceiveData(I2C_Type* I2Cx, I2C_S_SETUP_Type *TransferCfg, I2C_TRANSFER_OPT_Type Opt);

void I2C_SetOwnSlaveAddr0(I2C_Type *I2Cx, uint8_t SlaveAddr_7bit, uint8_t GeneralCallState);
void I2C_SetOwnSlaveAddr1(I2C_Type *I2Cx, uint8_t SlaveAddr_7bit, uint8_t GeneralCallState);

/* I2C Interrupt handler functions ------*/
void I2C_IntCmd(I2C_Type *I2Cx, Bool NewState);

#ifdef __cplusplus
}
#endif

#endif /* _I2C_H_ */

/* --------------------------------- End Of File ------------------------------ */
