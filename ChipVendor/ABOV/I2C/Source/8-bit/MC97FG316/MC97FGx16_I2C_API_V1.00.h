/***************************************************************************//**
*   @file					MC97FGx16_I2C_API_V1.00.h
*   @brief				I2C Peripheral Function 
*   @author		ABOV Semiconductor Co., Ltd.
*   @version		V1.00
*   @date				20. Feb. 2020
*
* Copyright(C) 2020, ABOV Semiconductor
* All rights reserved.
*
********************************************************************************
* DISCLAIMER 
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, ABOV SEMICONDUCTOR DISCLAIMS ALL LIABILITIES FROM ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USI_I2C_H_
#define __USI_I2C_H_
/*******************************************************************************
* Included File
*******************************************************************************/
#include <Intrins.h>


/*******************************************************************************
* I2C Mode Selection
*******************************************************************************/
#if 1	// I2C Interrupt Mode
#define	I2C_INTERRUPT
#else	// I2C Polling Mode
#define	I2C_POLLING
#endif


/*******************************************************************************
* Public Macro
*******************************************************************************/
#define I2C_DEVICE_ADDRESS			0xA0				// at master mode, slave device address
#define I2C_SLAVE_OWN_ADDR		0xA0				// at slave mode, its own address
#define I2C_SPEED											100000		// i2c speed (100kHz)

#define I2C_MAX_BUFFER_SIZE		20					// i2c global buffer size (20-byte)
#define I2C_MAX_CHANNEL					1						// i2c count of channel (i2c0)

#define I2C_CH0														0						// i2c0

#define I2C_ACK_DISABLE						0
#define I2C_ACK_ENABLE							1

#define I2C_WRITE_MODE						0
#define I2C_READ_MODE							1

#define I2C_IDLE													0
#define I2C_BUSY												1

#define I2C_TRUE												1
#define I2C_FALSE												0
/*******************************************************************************
* Public Typedef
*******************************************************************************/
/* I2C Control Register Flags */
enum  i2c_control_flags{
	fI2CnIFLAG			= (1<<7),				// i2c interrupt flag
	fI2CnEN					= (1<<6),				// activate i2c block
	fI2CnRESET		= (1<<5),				// i2c reset
	fI2CnIEN					= (1<<4),				// i2c interrupt enable
	fACKnEN					= (1<<3),				// control ack signal generation at ninth scl period
	fIMASTERn			= (1<<2),				// represents operation mode of i2c (master or slave)
	fSTOPCn				= (1<<1),				// stop condition generation when i2c is master
	fSTARTCn			= (1<<0),				// start condition generation when i2c is master
};
/* I2C Status Register Flags */
enum  i2c_status_flags{
	fGCALL						= (1<<7),				// general call is detected
	fTEND							= (1<<6),				// byte complete
	fSTOPD					= (1<<5),				// stop condition is detected
	fSSEL							= (1<<4),				// acts as a slave
	fMLOST					= (1<<3),				// lost bus mastership
	fBUSY						= (1<<2),				// i2c bus is busy
	fTMODE					= (1<<1),				// i2c is transmitter
	fRXACK						= (1<<0),				// ack is received at ninth scl period
};

/*******************************************************************************
* Exported Public Function
*******************************************************************************/
void USI_I2C_Initial(unsigned char ch, unsigned long speed, unsigned char addr, unsigned char ack);
void USI_I2C_Enable(unsigned char ch, unsigned char enable);
void USI_I2C_ConfigureGerneralCall(unsigned char ch, unsigned char enable);
void USI_I2C_ConfigureAcknowledge(unsigned char ch, unsigned char enable);
void USI_I2C_SoftwareReset(unsigned char ch, unsigned char enable);

void USI_I2C_MasterTransferData(unsigned char ch, unsigned char dev_addr, unsigned char *write_data, unsigned char write_len, unsigned char *read_data, unsigned char read_len);
void USI_I2C_ConfigureInterrupt(unsigned char ch, unsigned char enable);
unsigned char USI_I2C_GetInterruptStatus(unsigned char ch);
void USI_I2C_InterruptHandler(unsigned char ch);

void USI_I2C_SetSlaveData(unsigned char ch);

#endif  /* End of __USI_I2C_H_ */
/************************ (C) COPYRIGHT ABOV SEMICONDUCTOR *****END OF FILE****/
