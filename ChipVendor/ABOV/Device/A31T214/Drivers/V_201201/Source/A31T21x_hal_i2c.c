/**********************************************************************//**
* @file				A31T21x_hal_i2c.c
* @brief			Contains all functions support for firmware library on A31T21x
* @version	1.00
* @date			29. JUNE. 2020
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
*ABOV assumes no liability for application assistance or the design of purchasers¡¯ products. No license, 
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


/* Includes ------------------------------------------------------------ */
#include "A31T21x_hal_i2c.h"


/*******************************************************************************
* Private Variable
*******************************************************************************/
uint8_t i2c_mode[I2C_MAX_CHANNEL] = {0, };

uint8_t receiveBuffer0[I2C_MAX_BUFFER_SIZE];
uint8_t receiveBuffer1[I2C_MAX_BUFFER_SIZE];
uint8_t receiveCount[I2C_MAX_CHANNEL] = {0, };
uint8_t receiveNum[I2C_MAX_CHANNEL] = {0, };

uint8_t transmitBuffer0[I2C_MAX_BUFFER_SIZE];
uint8_t transmitBuffer1[I2C_MAX_BUFFER_SIZE];
uint8_t transmitCount[I2C_MAX_CHANNEL] = {0, };
uint8_t transmitNum[I2C_MAX_CHANNEL] = {0, };
/*******************************************************************************
* Private Function Prototype
*******************************************************************************/
static void HAL_I2C_Start(uint8_t ch, uint8_t enable);
static void HAL_I2C_Stop(uint8_t ch, uint8_t enable);
static void HAL_I2C_Send7bitAddress(uint8_t ch, uint8_t dev_addr, uint8_t direction);
static void HAL_I2C_MasterProcess(uint8_t ch);
static void HAL_I2C_SlaveProcess(uint8_t ch);

/*******************************************************************************
* Public Function
*******************************************************************************/

/**
* @brief			Initialize i2c peripheral
* @param	ch					This parameter contains the channel of USI peripheral.
* @param	speed		This parameter contains the buadrate of i2c.
* @param	addr	This parameter contains the slave address of i2c(address[7:1]bit)
* @param	ack	This parameter contains the enable of acknowledge signal. 
*								- I2C_ACK_DISABLE = 0
*								- I2C_ACK_ENABLE = 1
* @return		None
*/
HAL_Status_Type HAL_I2C_Init(uint8_t ch, uint32_t speed, uint8_t addr, uint8_t ack)
{
	/* Variable Initialization */
	receiveNum[ch] = 0;
	transmitNum[ch] = 0;
	receiveCount[ch] = 0;
	transmitCount[ch] = 0;

	/* I2C0 Initial */
	if(ch == I2C_CH0)
	{
		SCU->PER2 |= (0x1UL<<4);
		SCU->PCER2 |= (0x1UL<<4);

		HAL_I2C_SoftwareReset(ch, I2C_TRUE);														// I2C Block Reset
		HAL_I2C_Enable(ch, I2C_TRUE);																			// I2C Enable
#ifdef I2C_INTERRUPT
		HAL_I2C_ConfigureInterrupt(ch, I2C_TRUE);											// I2C Interrupt Enable
#else
		HAL_I2C_ConfigureInterrupt(ch, I2C_FALSE);										// I2C Interrupt Enable
#endif
		HAL_I2C_ConfigureAcknowledge(ch, ack);												// I2C Ack Enable

		I2C0->SAR1 = addr;																													// Set slave own address
		I2C0->SAR2 = addr;																													// Set slave own address

		I2C0->SCLR = ((SystemPeriClock/speed) - 2) / 4;						// Set I2C speed_low
		I2C0->SCHR = ((SystemPeriClock/speed) - 2) / 4;						// Set I2C speed_high
		I2C0->SDHR = 1;																															// Set I2C data hold time

		i2c_mode[ch] = I2C_IDLE;
	}

	/* I2C1 Initial */
	else if(ch == I2C_CH1)
	{
		SCU->PER2 |= (0x1UL<<5);
		SCU->PCER2 |= (0x1UL<<5);

		HAL_I2C_SoftwareReset(ch, I2C_TRUE);														// I2C Block Reset
		HAL_I2C_Enable(ch, I2C_TRUE);																			// I2C Enable
#ifdef I2C_INTERRUPT
		HAL_I2C_ConfigureInterrupt(ch, I2C_TRUE);											// I2C Interrupt Enable
#else
		HAL_I2C_ConfigureInterrupt(ch, I2C_FALSE);										// I2C Interrupt Enable
#endif
		HAL_I2C_ConfigureAcknowledge(ch, I2C_ACK_ENABLE);	// I2C Ack Enable

		I2C1->SAR1 = addr;																													// Set slave own address
		I2C1->SAR2 = addr;																													// Set slave own address

		I2C1->SCLR = ((SystemPeriClock/speed) - 2) / 4;						// Set I2C speed_low
		I2C1->SCHR = ((SystemPeriClock/speed) - 2) / 4;						// Set I2C speed_high
		I2C1->SDHR = 1;																															// Set I2C data hold time
		
		i2c_mode[ch] = I2C_IDLE;
	}

	return HAL_OK;
}

/**
* @brief			Enables or disables the I2C Software reset
* @param	ch		This parameter contains the channel of USI peripheral.
* @param	enable   This parameter contains the enable of this function. 
*								- I2C_FALSE = 0
*								- I2C_TRUE = 1
* @return		None
*/
void HAL_I2C_SoftwareReset(uint8_t ch, uint8_t enable)
{
	uint8_t i;

	/* I2C0 Block Reset */
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
			SCU->PER2 &= ~(0x1UL<<4);
			SCU->PER2 |= (0x1UL<<4);
			SCU->PCER2 &= ~(0x1UL<<4);
			SCU->PCER2 |= (0x1UL<<4);
			SCU->PRER2 |= (0x1UL<<4);
			 for( i = 0 ; i < 10 ; i++ )
			 {
					__NOP();
			 }
			SCU->PRER2 &= ~(0x1UL<<4);
		}
		else
		{
			SCU->PRER2 &= ~(0x1UL<<4);
		}
	}
	/* I2C1 Block Reset */
	else if(ch == I2C_CH1)
	{
		if(enable == I2C_TRUE)
		{
			SCU->PER2 &= ~(0x1UL<<5);
			SCU->PER2 |= (0x1UL<<5);
			SCU->PCER2 &= ~(0x1UL<<5);
			SCU->PCER2 |= (0x1UL<<5);
			SCU->PRER2 |= (0x1UL<<5);
			 for( i = 0 ; i < 10 ; i++ )
			 {
					__NOP();
			 }
			SCU->PRER2 &= ~(0x1UL<<5);
		}
		else
		{
			SCU->PRER2 &= ~(0x1UL<<5);
		}
	}
}

/**
* @brief			Enables or disables the I2C peripheral.
* @param	ch		This parameter contains the channel of USI peripheral.
* @param	enable   This parameter contains the enable of this function. 
*								- I2C_FALSE = 0
*								- I2C_TRUE = 1
* @return		None
*/
void HAL_I2C_Enable(uint8_t ch, uint8_t enable)
{
	/* I2C0 Enable */
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
			I2C0->CR |= (fI2CnEN);
		}
		else
		{
			I2C0->CR &= ~(fI2CnEN);
		}
	}
	/* I2C1 Enable */
	else if(ch == I2C_CH1)
	{
		if(enable == I2C_TRUE)
		{
			I2C1->CR |= (fI2CnEN);
		}
		else
		{
			I2C1->CR &= ~(fI2CnEN);
		}
	}
}

/**
* @brief			Configure the enable or disable I2C interrupt.
* @param	ch		This parameter contains the channel of USI peripheral.
* @param	enable   This parameter contains the enable of this function. 
*								- FALSE = 0
*								- I2C_TRUE = 1
* @return		None
*/
void HAL_I2C_ConfigureInterrupt(uint8_t ch, uint8_t enable)
{
	/* I2C0 Interrupt Enable */
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
      NVIC_ClearPendingIRQ( I2C0_IRQn );
      NVIC_EnableIRQ( I2C0_IRQn );
			I2C0->CR |= (fI2CnIEN);
		}
		else
		{
      NVIC_DisableIRQ( I2C0_IRQn );
			I2C0->CR &= ~(fI2CnIEN);
		}
	}
	/* I2C1 Interrupt Enable */
	else if(ch == I2C_CH1)
	{
		if(enable == I2C_TRUE)
		{
      NVIC_ClearPendingIRQ( I2C1_IRQn );
      NVIC_EnableIRQ( I2C1_IRQn );
			I2C1->CR |= (fI2CnIEN);
		}
		else
		{
      NVIC_DisableIRQ( I2C1_IRQn );
			I2C1->CR &= ~(fI2CnIEN);
		}
	}
}

/**
* @brief			Configure the I2C Acknowledge signal.
* @param	ch		This parameter contains the channel of USI peripheral.
* @param	enable   This parameter contains the enable of this function. 
*								- I2C_FALSE = 0
*								- I2C_TRUE = 1
* @return		None
*/
void HAL_I2C_ConfigureAcknowledge(uint8_t ch, uint8_t enable)
{
	/* I2C0 ACK Enable */
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
			I2C0->CR |= (fACKnEN);
		}
		else
		{
			I2C0->CR &= ~(fACKnEN);
		}
	}
	/* I2C1 ACK Enable */
	else if(ch == I2C_CH1)
	{
		if(enable == I2C_TRUE)
		{
			I2C1->CR |= (fACKnEN);
		}
		else
		{
			I2C1->CR &= ~(fACKnEN);
		}
	}
}

/**
* @brief			Enables or disables the I2C Start condition.
* @param	ch		This parameter contains the channel of USI peripheral.
* @param	enable   This parameter contains the enable of this function. 
*								- I2C_FALSE = 0
*								- I2C_TRUE = 1
* @return		None
*/
void HAL_I2C_Start(uint8_t ch, uint8_t enable)
{
	/* I2C0 Start Signal Generate */
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
			I2C0->CR |= (fSTARTCn);
		}
		else
		{
			I2C0->CR &= ~(fSTARTCn);
		}
	}
	/* I2C1 Start Signal Generate */
	else if(ch == I2C_CH1)
	{
		if(enable == I2C_TRUE)
		{
			I2C1->CR |= (fSTARTCn);
		}
		else
		{
			I2C1->CR &= ~(fSTARTCn);
		}
	}
}

/**
* @brief			Enables or disables the I2C Stop condition.
* @param	ch		This parameter contains the channel of USI peripheral.
* @param	enable   This parameter contains the enable of this function. 
*								- I2C_FALSE = 0
*								- I2C_TRUE = 1
* @return		None
*/
void HAL_I2C_Stop(uint8_t ch, uint8_t enable)
{
	/* I2C0 Stop Signal Generate */
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
			I2C0->CR |= (fSTOPCn);
		}
		else
		{
			I2C0->CR &= ~(fSTOPCn);
		}
	}
	/* I2C1 Stop Signal Generate */
	else if(ch == I2C_CH1)
	{
		if(enable == I2C_TRUE)
		{
			I2C1->CR |= (fSTOPCn);
		}
		else
		{
			I2C1->CR &= ~(fSTOPCn);
		}
	}
}

/**
* @brief			Send slave address as i2c master.
* @param	ch		This parameter contains the channel of USI peripheral.
* @param	dev_addr			This parameter contains the slave address of target.
* @param	direction			This parameter contains the mode of i2c(read or write).
*								- I2C_WRITE_MODE = 0
*								- I2C_READ_MODE = 1
* @return		None
*/
void HAL_I2C_Send7bitAddress(uint8_t ch, uint8_t dev_addr, uint8_t direction)
{
	/* I2C0 write/read direction setting */
	if(ch == I2C_CH0)
	{
		I2C0->DR = dev_addr | direction;
	}
	/* I2C1 write/read direction setting */
	else if(ch == I2C_CH1)
	{
		I2C1->DR = dev_addr | direction;
	}
	/* I2C Start Signal Generate */
	HAL_I2C_Start(ch, I2C_TRUE);
}

/**
* @brief			Write multi bytes by writing as i2c master.
* @param	ch		This parameter contains the channel of USI peripheral.
* @param	dev_addr			This parameter contains the slave address of target.
* @param	*write_data			This parameter contains the write data of i2c(pointer variable)
* @param	write_len			This parameter contains the number of write length.
* @param	*read_data			This parameter contains the read data of i2c(pointer variable)
* @param	read_len			This parameter contains the number of read length.
* @return		None
*/
void HAL_I2C_MasterTransferData(uint8_t ch, uint8_t dev_addr, uint8_t *write_data, uint8_t write_len, uint8_t *read_data, uint8_t read_len)
{
	uint8_t i;
	uint32_t timekeeper;

	/* Variable Initialization */
	receiveNum[ch] = 0;
	transmitNum[ch] = 0;
	receiveCount[ch] = read_len;
	transmitCount[ch] = write_len;

	timekeeper = 0x5FFFF;								// i2c timeout setting 60ms @ Sysclk 40MHz
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
		if(I2C0->ST & fBUSY)
		{
			HAL_I2C_Init(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
		}
		// i2c start at write mode
		if(write_len)
		{
			HAL_I2C_Send7bitAddress(ch, dev_addr, I2C_WRITE_MODE);
		}
		// i2c start at read mode
		else
		{
			HAL_I2C_Send7bitAddress(ch, dev_addr, I2C_READ_MODE);
		}
		// i2c timeout check
		while(i2c_mode[ch] == I2C_BUSY)
		{
			timekeeper--;
			if(timekeeper == 0)
			{
				i2c_mode[ch] = I2C_IDLE;
				HAL_I2C_Init(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
			}
			#ifdef	I2C_POLLING
			HAL_I2C_InterruptHandler(ch);		// Activate when polling mode
			#endif
		}
		// load rx_data to local buffer from global buffer
		for(i = 0; i < read_len; i++)
		{
			read_data[i] = receiveBuffer0[i];
		}
	}
////////////////////////////////////////////////////////////////////////////////////
	/* I2C1 Master TransferData */
	else if(ch == I2C_CH1)
	{
		// load tx_data to global buffer from local buffer
		for(i = 0; i < write_len; i++)
		{
			transmitBuffer1[i] = write_data[i];
		}
		// if status of i2c is BUSY, it needs initialization
		if(I2C1->ST & fBUSY)
		{
			HAL_I2C_Init(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
		}
		// i2c start at write mode
		if(write_len)
		{
			HAL_I2C_Send7bitAddress(ch, dev_addr, I2C_WRITE_MODE);
		}
		// i2c start at read mode
		else
		{
			HAL_I2C_Send7bitAddress(ch, dev_addr, I2C_READ_MODE);
		}
		// i2c timeout check
		while(i2c_mode[ch] == I2C_BUSY)
		{
			timekeeper--;
			if(timekeeper == 0)
			{
				i2c_mode[ch] = I2C_IDLE;
				HAL_I2C_Init(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
			}
			#ifdef	I2C_POLLING
			HAL_I2C_InterruptHandler(ch);		// Activate when polling mode
			#endif
		}
		// load rx_data to local buffer from global buffer
		for(i = 0; i < read_len; i++)
		{
			read_data[i] = receiveBuffer1[i];
		}
	}
}

/**
* @brief			Configure the enable or disable I2C interrupt.
* @param	ch		This parameter contains the channel of USI peripheral.
* @return		None
*/
void HAL_I2C_InterruptHandler(uint8_t ch)
{
	uint8_t i2c_status;
	uint8_t i2c_control;

	/* To check i2c status register */
	if(ch == I2C_CH0)
	{
		i2c_status = I2C0->ST;
		i2c_control = I2C0->CR;
	}
	else if(ch == I2C_CH1)
	{
		i2c_status = I2C1->ST;
		i2c_control = I2C1->CR;
	}

	/* To check i2c error */
	if((i2c_status & fMLOST) || (i2c_status & fSTOPD))
	{
		if(ch == I2C_CH0)
		{
			i2c_mode[ch] = I2C_IDLE;
			HAL_I2C_Init(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
		}
		else if(ch == I2C_CH1)
		{
			i2c_mode[ch] = I2C_IDLE;
			HAL_I2C_Init(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
		}
	}

	/* To enter i2c irq process */
	else
	{
		if(i2c_control & fIMASTERn)
		{
			HAL_I2C_MasterProcess(ch);	// I2C Master Mode
		}
		else
		{
			HAL_I2C_SlaveProcess(ch);		// I2C Slave Mode
		}
	}
}

/**
* @brief			Response specific bytes as i2c master.(custom user)
* @param	ch		This parameter contains the channel of USI peripheral.
* @return		None
*/
static void HAL_I2C_MasterProcess(uint8_t ch)
{
	uint8_t i2c_status;

	/* To check i2c status register */
	if(ch == I2C_CH0)
	{
		i2c_status = I2C0->ST;
	}
	else if(ch == I2C_CH1)
	{
		i2c_status = I2C1->ST;
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
					I2C0->DR = transmitBuffer0[transmitNum[ch]];
				}
				else
				{
					I2C1->DR = transmitBuffer1[transmitNum[ch]];
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
					HAL_I2C_Send7bitAddress(ch, I2C_DEVICE_ADDRESS, I2C_READ_MODE);
				}
				// Master Rx Count = 0
				else
				{
					// Stop signal generate
					transmitNum[ch] = 0;
					HAL_I2C_Stop(ch, I2C_TRUE);
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
					HAL_I2C_ConfigureAcknowledge(ch, I2C_ACK_DISABLE);
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
						receiveBuffer0[receiveNum[ch]] = I2C0->DR;
					}
					else if(ch == I2C_CH1)
					{
						receiveBuffer1[receiveNum[ch]] = I2C1->DR;
					}
					receiveNum[ch] += 1;
				}
				// Master Rx Count = 1 (last rx_data)
				else if(receiveCount[ch] == 1)
				{
					if(ch == I2C_CH0)
					{
						receiveBuffer0[receiveNum[ch]] = I2C0->DR;
					}
					else if(ch == I2C_CH1)
					{
						receiveBuffer1[receiveNum[ch]] = I2C1->DR;
					}
					receiveNum[ch] += 1;
					HAL_I2C_ConfigureAcknowledge(ch, I2C_ACK_DISABLE);		// NoAck generate
				}
				// Master Rx Count = 0
				else
				{
					// Stop signal generate
					receiveNum[ch] = 0;
					HAL_I2C_Stop(ch, I2C_TRUE);
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
				receiveBuffer0[receiveNum[ch]] = I2C0->DR;
			}
			else if(ch == I2C_CH1)
			{
				receiveBuffer1[receiveNum[ch]] = I2C1->DR;
			}
		}
		// NoAck Stop
		receiveNum[ch] = 0;
		HAL_I2C_Stop(ch, I2C_TRUE);		// Stop signal generate
	}
	// It needs to clear status register for next step
	if(ch == I2C_CH0)
	{
		I2C0->ST = 0xFF;
	}
	else if(ch == I2C_CH1)
	{
		I2C1->ST = 0xFF;
	}
}

/**
* @brief			Set specific bytes as i2c slave.(custom user)
* @param	ch		This parameter contains the channel of USI peripheral.
* @return		None
*/
void HAL_I2C_SetSlaveData(uint8_t ch)
{
	uint8_t i;
	uint32_t timekeeper;

	receiveNum[ch] = 0;
	transmitNum[ch] = 0;

	timekeeper = 0x5FFFF;								// i2c timeout setting 60ms @ Sysclk 40MHz
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
	else if(ch == I2C_CH1)
	{
		for(i=0; i<I2C_MAX_BUFFER_SIZE; i++)
		{
			transmitBuffer1[i] = (0x00 + i);
			receiveBuffer1[i] = 0x00;
		}
	}

	/* i2c timeout check */
	while(i2c_mode[ch] == I2C_BUSY)
	{
//		timekeeper--;
		if(timekeeper == 0)
		{
			i2c_mode[ch] = I2C_IDLE;
			HAL_I2C_Init(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
		}
		#ifdef I2C_POLLING
		HAL_I2C_InterruptHandler(ch);		// Activate when polling mode
		#endif
	}
}

/**
* @brief			Response specific bytes as i2c slave.(custom user)
* @param	ch		This parameter contains the channel of USI peripheral.
* @return		None
*/
static void HAL_I2C_SlaveProcess(uint8_t ch)
{
	uint8_t i2c_status;

	/* To check i2c status register */
	if(ch == I2C_CH0)
	{
		i2c_status = I2C0->ST;
	}
	else if(ch == I2C_CH1)
	{
		i2c_status = I2C1->ST;
	}

	// address data is received
	if(i2c_status & fGCALL)
	{
		/* Insert user specific slave code */
		__nop();
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
				I2C0->DR = transmitBuffer0[transmitNum[ch]];
				transmitNum[ch] += 1;
			}
			else if(ch == I2C_CH1)
			{
				I2C1->DR = transmitBuffer1[transmitNum[ch]];
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
				receiveBuffer0[receiveNum[ch]] = I2C0->DR;
				receiveNum[ch] += 1;
			}
			else if(ch == I2C_CH1)
			{
				receiveBuffer1[receiveNum[ch]] = I2C1->DR;
				receiveNum[ch] += 1;
			}
		}
	}
	// It needs to clear status register for next step
	if(ch == I2C_CH0)
	{
		I2C0->ST = 0xFF;
	}
	else if(ch == I2C_CH1)
	{
		I2C1->ST = 0xFF;
	}
}

/**
* @brief			Get the I2C interrupt status.
* @param	ch		This parameter contains the channel of USI peripheral.
* @return		Value of status(interrupt flag)
*/
uint8_t HAL_I2C_GetInterruptStatus(uint8_t ch)
{
	uint8_t result;

	if(ch == I2C_CH0)
	{
		result = I2C0->ST;
	}
	else if(ch == I2C_CH1)
	{
		result = I2C1->ST;
	}
	
	return result;
}

/**
* @brief			Enables or disables the I2C general call function.
* @param	ch		This parameter contains the channel of USI peripheral.
* @param	enable   This parameter contains the enable of this function. 
*								- I2C_FALSE = 0
*								- I2C_TRUE = 1
* @return		None
*/
void HAL_I2C_ConfigureGerneralCall(uint8_t ch, uint8_t enable)
{
	/* I2C0 Config. GCALL */	
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
			I2C0->SAR1 |= (0x1);
		}
		else
		{
			I2C0->SAR1 &= ~(0x1);
		}
	}
	/* I2C1 Config. GCALL */	
	else if(ch == I2C_CH1)
	{
		if(enable == I2C_TRUE)
		{
			I2C1->SAR1 |= (0x1);
		}
		else
		{
			I2C1->SAR1 &= ~(0x1);
		}
	}
}


/* --------------------------------- End Of File ------------------------------ */
