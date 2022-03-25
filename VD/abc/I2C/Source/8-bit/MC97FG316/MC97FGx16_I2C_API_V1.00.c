/***************************************************************************//**
*   @file					MC97FGx16_I2C_API_V1.00.c
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

/*******************************************************************************
* Included File
*******************************************************************************/
#include "MC97FGx16_I2C_API_V1.00.h"
#include "MC97FG316.h"
/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/

/*******************************************************************************
* Private Typedef
*******************************************************************************/

/*******************************************************************************
* Private Variable
*******************************************************************************/
unsigned char i2c_mode[I2C_MAX_CHANNEL] = {0, };

unsigned char receiveBuffer0[I2C_MAX_BUFFER_SIZE];
unsigned char receiveBuffer1[I2C_MAX_BUFFER_SIZE];
unsigned char receiveCount[I2C_MAX_CHANNEL] = {0, };
unsigned char receiveNum[I2C_MAX_CHANNEL] = {0, };

unsigned char transmitBuffer0[I2C_MAX_BUFFER_SIZE];
unsigned char transmitBuffer1[I2C_MAX_BUFFER_SIZE];
unsigned char transmitCount[I2C_MAX_CHANNEL] = {0, };
unsigned char transmitNum[I2C_MAX_CHANNEL] = {0, };
/*******************************************************************************
* Private Function Prototype
*******************************************************************************/
static void USI_I2C_Start(unsigned char ch, unsigned char enable);
static void USI_I2C_Stop(unsigned char ch, unsigned char enable);
static void USI_I2C_Send7bitAddress(unsigned char ch, unsigned char dev_addr, unsigned char direction);
static void USI_I2C_MasterProcess(unsigned char ch);
static void USI_I2C_SlaveProcess(unsigned char ch);

unsigned long SystemClock = 16000000;	// Internal RC 16MHz
/*******************************************************************************
* Public Function
*******************************************************************************/

/**
* @brief		Initialize usi(i2c) peripheral
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   speed			This parameter contains the buadrate of i2c.
* @param   addr	This parameter contains the slave address of i2c(address[7:1]bit)
* @param   ack	This parameter contains the enable of acknowledge signal. 
* 					- I2C_ACK_DISABLE = 0
*					- I2C_ACK_ENABLE = 1
* @return		None
*/
void USI_I2C_Initial(unsigned char ch, unsigned long speed, unsigned char addr, unsigned char ack)
{	
	/* Variable Initialization */
	receiveNum[ch] = 0;
	transmitNum[ch] = 0;
	receiveCount[ch] = 0;
	transmitCount[ch] = 0;

	/* I2C Initial */
	if(ch == I2C_CH0)
	{
		USI_I2C_SoftwareReset(ch, I2C_TRUE);													// I2C Block Reset
		USI_I2C_Enable(ch, I2C_TRUE);																			// I2C Enable
		USI_I2C_ConfigureInterrupt(ch, I2C_TRUE);										// I2C Interrupt Enable
		USI_I2C_ConfigureAcknowledge(ch, ack);											// I2C Ack Enable

		I2CSAR = addr;																																// Set slave own address

		I2CSCLLR = ((SystemClock/speed)-4)/4;												// Set I2C speed_low
		I2CSCLHR = ((SystemClock/speed)-4)/4;											// Set I2C speed_high
		I2CSDAHR = 1;																																// Set I2C data hold time
		
		i2c_mode[ch] = I2C_IDLE;
	}
}

/**
* @brief		Enables or disables the I2C Software reset
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   enable   This parameter contains the enable of this function. 
* 					- I2C_FALSE = 0
*					- I2C_TRUE = 1
* @return		None
*/
void USI_I2C_SoftwareReset(unsigned char ch, unsigned char enable)
{
	/* I2C Block Reset */
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
			I2CMR |= fI2CnRESET;
		}
		else
		{
			I2CMR &= ~fI2CnRESET;
		}
	}
}

/**
* @brief		Enables or disables the I2C peripheral.
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   enable   This parameter contains the enable of this function. 
* 					- I2C_FALSE = 0
*					- I2C_TRUE = 1
* @return		None
*/
void USI_I2C_Enable(unsigned char ch, unsigned char enable)
{
	/* I2C Enable */
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
			I2CMR |= (fI2CnEN);
		}
		else
		{
			I2CMR &= ~(fI2CnEN);
		}
	}
}

/**
* @brief		Configure the enable or disable I2C interrupt.
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   enable   This parameter contains the enable of this function. 
* 					- FALSE = 0
*					- I2C_TRUE = 1
* @return		None
*/
void USI_I2C_ConfigureInterrupt(unsigned char ch, unsigned char enable)
{
	/* I2C Interrupt Enable */
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
			IE1 |= 0x08;
			I2CMR |= (fI2CnIEN);
		}
		else
		{
			IE1 &= ~(0x08);
			I2CMR &= ~(fI2CnIEN);
		}
	}
}

/**
* @brief		Configure the I2C Acknowledge signal.
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   enable   This parameter contains the enable of this function. 
* 					- I2C_FALSE = 0
*					- I2C_TRUE = 1
* @return		None
*/
void USI_I2C_ConfigureAcknowledge(unsigned char ch, unsigned char enable)
{
	/* I2C ACK Enable */
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
			I2CMR |= (fACKnEN);
		}
		else
		{
			I2CMR &= ~(fACKnEN);
		}
	}
}

/**
* @brief		Enables or disables the I2C Start condition.
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   enable   This parameter contains the enable of this function. 
* 					- I2C_FALSE = 0
*					- I2C_TRUE = 1
* @return		None
*/
void USI_I2C_Start(unsigned char ch, unsigned char enable)
{
	/* I2C Start Signal Generate */
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
			I2CMR |= (fSTARTCn);
		}
		else
		{
			I2CMR &= ~(fSTARTCn);
		}
	}
}

/**
* @brief		Enables or disables the I2C Stop condition.
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   enable   This parameter contains the enable of this function. 
* 					- I2C_FALSE = 0
*					- I2C_TRUE = 1
* @return		None
*/
void USI_I2C_Stop(unsigned char ch, unsigned char enable)
{
	/* I2C Stop Signal Generate */
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
			I2CMR |= (fSTOPCn);
		}
		else
		{
			I2CMR &= ~(fSTOPCn);
		}
	}
}

/**
* @brief		Send slave address as i2c master.
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   dev_addr			This parameter contains the slave address of target.
* @param   direction			This parameter contains the mode of i2c(read or write).
* 					- I2C_WRITE_MODE = 0
*					- I2C_READ_MODE = 1
* @return		None
*/
void USI_I2C_Send7bitAddress(unsigned char ch, unsigned char dev_addr, unsigned char direction)
{
	/* I2C write/read direction setting */
	if(ch == I2C_CH0)
	{
		I2CDR = dev_addr | direction;
	}
	/* I2C Start Signal Generate */
	USI_I2C_Start(ch, I2C_TRUE);
}

/**
* @brief		Write multi bytes by writing as i2c master.
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   dev_addr			This parameter contains the slave address of target.
* @param   *write_data			This parameter contains the write data of i2c(pointer variable)
* @param   write_len			This parameter contains the number of write length.
* @param   *read_data			This parameter contains the read data of i2c(pointer variable)
* @param   read_len			This parameter contains the number of read length.
* @return		None
*/
void USI_I2C_MasterTransferData(unsigned char ch, unsigned char dev_addr, unsigned char *write_data, unsigned char write_len, unsigned char *read_data, unsigned char read_len)
{
	unsigned char i;
	unsigned int timekeeper;

	/* Variable Initialization */
	receiveNum[ch] = 0;
	transmitNum[ch] = 0;
	receiveCount[ch] = read_len;
	transmitCount[ch] = write_len;

	timekeeper = 0x5FFFF;								// i2c timeout setting about 100ms @ Sysclk 16MHz
	i2c_mode[ch] = I2C_BUSY;				// i2c mode setting

////////////////////////////////////////////////////////////////////////////////////
	/* I2C0 Master TransferData */
	if(ch == I2C_CH0)
	{
		// load tx_data to global buffer from local buffer
		for(i = 0; i < write_len; i++)
		{
			transmitBuffer0[i] = write_data[i];
		}
		// if status of i2c is BUSY, it needs initialization
		if(I2CSR & fBUSY)
		{
			USI_I2C_Initial(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
		}
		// i2c start at write mode
		if(write_len)
		{
			USI_I2C_Send7bitAddress(ch, dev_addr, I2C_WRITE_MODE);
		}
		// i2c start at read mode
		else
		{
			USI_I2C_Send7bitAddress(ch, dev_addr, I2C_READ_MODE);
		}
		// i2c timeout check
		while(i2c_mode[ch] == I2C_BUSY)
		{
			timekeeper--;
			if(timekeeper == 0)
			{
				i2c_mode[ch] = I2C_IDLE;
				USI_I2C_Initial(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
			}
			#ifdef	I2C_POLLING
			USI_I2C_InterruptHandler(ch);		// Activate when polling mode
			#endif
		}
		// load rx_data to local buffer from global buffer
		for(i = 0; i < read_len; i++)
		{
			read_data[i] = receiveBuffer0[i];
		}
	}
////////////////////////////////////////////////////////////////////////////////////
}

/**
* @brief		Configure the enable or disable I2C interrupt.
* @param   ch		This parameter contains the channel of USI peripheral.
* @return		None
*/
void USI_I2C_InterruptHandler(unsigned char ch)
{
	unsigned char i2c_status;

	/* To check i2c status register */
	if(ch == I2C_CH0)
	{
		i2c_status = I2CSR;
	}

	/* To check i2c error */
	if((i2c_status & fMLOST) || (i2c_status & fSTOPD))
	{
		if(ch == I2C_CH0)
		{
			i2c_mode[ch] = I2C_IDLE;
			USI_I2C_Initial(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
		}
	}

	/* To enter i2c irq process */
	else
	{
		if(!(i2c_status & fSSEL))
		{
			USI_I2C_MasterProcess(ch);	// I2C Master Mode
		}
		else
		{
			USI_I2C_SlaveProcess(ch);		// I2C Slave Mode
		}
	}
}

/**
* @brief		Response specific bytes as i2c master.(custom user)
* @param   ch		This parameter contains the channel of USI peripheral.
* @return		None
*/
static void USI_I2C_MasterProcess(unsigned char ch)
{
	unsigned char i2c_status;

	/* To check i2c status register */
	if(ch == I2C_CH0)
	{
		i2c_status = I2CSR;
	}

	// ACK OK
	if(i2c_status & fRXACK)
	{
		// Master Tx Mode
		if(i2c_status & fTMODE)
		{
			// Master Tx Count > 0
			if(transmitCount[ch])
			{
				if(ch == I2C_CH0)
				{
					I2CDR = transmitBuffer0[transmitNum[ch]];
				}
				transmitCount[ch] -= 1;
				transmitNum[ch] += 1;
			}
			// Master Tx Count = 0
			else
			{
				// Master Rx Count > 0
				if(receiveCount[ch])
				{
					// Restart for Rx
					USI_I2C_Send7bitAddress(ch, I2C_DEVICE_ADDRESS, I2C_READ_MODE);
				}
				// Master Rx Count = 0
				else
				{
					// Stop signal generate
					transmitNum[ch] = 0;
					USI_I2C_Stop(ch, I2C_TRUE);
				}
			}
		}
		// Master Rx mode
		else
		{
			// address data is received
			if(i2c_status&fGCALL)
			{
				// Master Rx Count = 1 (last rx_data)
				if(receiveCount[ch] == 1)
				{
					// NoAck generate
					USI_I2C_ConfigureAcknowledge(ch, I2C_ACK_DISABLE);
				}
			}
			// normal data is received
			else
			{
				receiveCount[ch] -= 1;
				// Master Rx Count > 1
				if(receiveCount[ch] > 1)
				{
					if(ch == I2C_CH0)
					{
						receiveBuffer0[receiveNum[ch]] = I2CDR;
					}
					receiveNum[ch] += 1;
				}
				// Master Rx Count = 1 (last rx_data)
				else if(receiveCount[ch] == 1)
				{
					if(ch == I2C_CH0)
					{
						receiveBuffer0[receiveNum[ch]] = I2CDR;
					}
					receiveNum[ch] += 1;
					USI_I2C_ConfigureAcknowledge(ch, I2C_ACK_DISABLE);		// NoAck generate
				}
				// Master Rx Count = 0
				else
				{
					// Stop signal generate
					receiveNum[ch] = 0;
					USI_I2C_Stop(ch, I2C_TRUE);
				}
			}
		}
	}
	// NoACK detect
	else
	{
		// Load last receive data to global rx_buffer
		if(receiveCount[ch] == 1)
		{
			receiveCount[ch] = 0;
			
			if(ch == I2C_CH0)
			{
				receiveBuffer0[receiveNum[ch]] = I2CDR;
			}
		}
		// NoAck Stop
		receiveNum[ch] = 0;
		USI_I2C_Stop(ch, I2C_TRUE);		// Stop signal generate
	}
	// It needs to clear status register for next step
	if(ch == I2C_CH0)
	{
		I2CSR = 0xFF;
	}
}

/**
* @brief		Set specific bytes as i2c slave.(custom user)
* @param   ch		This parameter contains the channel of USI peripheral.
* @return		None
*/
void USI_I2C_SetSlaveData(unsigned char ch)
{
	unsigned char i;
	unsigned int timekeeper;

	receiveNum[ch] = 0;
	transmitNum[ch] = 0;

	timekeeper = 0x5FFFF;								// i2c timeout setting about 100ms @ Sysclk 16MHz
	i2c_mode[ch] = I2C_BUSY;				// i2c mode setting
	
	/* I2C Slave Tx/Rx Buffer Set */
	if(ch == I2C_CH0)
	{
		for(i=0; i<I2C_MAX_BUFFER_SIZE; i++)
		{
			transmitBuffer0[i] = (0x00 + i);
			receiveBuffer0[i] = 0x00;
		}
	}
	/* i2c timeout check */
	while(i2c_mode[ch] == I2C_BUSY)
	{
		timekeeper--;
		if(timekeeper == 0)
		{
			i2c_mode[ch] = I2C_IDLE;
			USI_I2C_Initial(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
		}
		#ifdef I2C_POLLING
		USI_I2C_InterruptHandler(ch);		// Activate when polling mode
		#endif
	}
}

/**
* @brief		Response specific bytes as i2c slave.(custom user)
* @param   ch		This parameter contains the channel of USI peripheral.
* @return		None
*/
static void USI_I2C_SlaveProcess(unsigned char ch)
{
	unsigned char i2c_status;

	/* To check i2c status register */
	if(ch == I2C_CH0)
	{
		i2c_status = I2CSR;
	}

	// address data is received
	if(i2c_status & fGCALL)
	{
		/* Insert user specific slave code */
		_nop_();
	}
	// normal data is received
	else
	{
		// Slave Tx mode
		if(i2c_status & fTMODE)
		{
			// To check tx buffer size
			if(transmitNum[ch] >= I2C_MAX_BUFFER_SIZE)
			{
				transmitNum[ch] = 0;
			}
			// To write tx buffer
			if(ch == I2C_CH0)
			{
				I2CDR = transmitBuffer0[transmitNum[ch]];
				transmitNum[ch] += 1;
			}
		}
		// Slave Rx mode
		else
		{
			// To check rx buffer size
			if(receiveNum[ch] >= I2C_MAX_BUFFER_SIZE)
			{
				receiveNum[ch] = 0;
			}
			// To read rx buffer
			if(ch == I2C_CH0)
			{
				receiveBuffer0[receiveNum[ch]] = I2CDR;
				receiveNum[ch] += 1;
			}
		}
	}
	// It needs to clear status register for next step
	if(ch == I2C_CH0)
	{
		I2CSR = 0xFF;
	}
}

/**
* @brief		Get the I2C interrupt status.
* @param   ch		This parameter contains the channel of USI peripheral.
* @return		Value of status(interrupt flag)
*/
unsigned char USI_I2C_GetInterruptStatus(unsigned char ch)
{
	unsigned char result;

	if(ch == I2C_CH0)
	{
		result = I2CSR;
	}
	
	return result;
}

/**
* @brief		Enables or disables the I2C general call function.
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   enable   This parameter contains the enable of this function. 
* 					- I2C_FALSE = 0
*					- I2C_TRUE = 1
* @return		None
*/
void USI_I2C_ConfigureGerneralCall(unsigned char ch, unsigned char enable)
{
	/* I2C Config. GCALL */	
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
			I2CSAR |= (0x1);
		}
		else
		{
			I2CSAR &= ~(0x1);
		}
	}
}

/************************ (C) COPYRIGHT ABOV SEMICONDUCTOR *****END OF FILE****/
