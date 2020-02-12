/***************************************************************************//**
*   @file					ABOV_USI_I2C.c
*   @brief				USI Peripheral Function 
*   @author		PTS Team, ABOV Semiconductor Co., Ltd.
*   @version		V0.0.1
*   @date				04. Feb. 2020
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
#include "ABOV_USI_I2C.h"
#include "A31G12x.h"
/*******************************************************************************
* Private Pre-processor Definition & Macro
*******************************************************************************/

/*******************************************************************************
* Private Typedef
*******************************************************************************/

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
static void USI_I2C_Start(uint8_t ch, uint8_t enable);
static void USI_I2C_Stop(uint8_t ch, uint8_t enable);
static void USI_I2C_Send7bitAddress(uint8_t ch, uint8_t dev_addr, uint8_t direction);
static void USI_I2C_SlaveProcessData(uint8_t ch);
static void USI_I2C_MasterProcessData(uint8_t ch);

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
void USI_I2C_Initial(uint8_t ch, uint32_t speed, uint8_t addr, uint8_t ack)
{
	/* Variable Initialization */
	receiveNum[ch] = 0;
	transmitNum[ch] = 0;
	receiveCount[ch] = 0;
	transmitCount[ch] = 0;

	if(ch == I2C_CH0)
	{
//		SCUCG->PPEN2 |= (0x1UL<<6);
		SCUCG->PPCLKEN2 |= (0x1UL<<6);																	// Peri. Clock Enable

		USI_I2C_SoftwareReset(ch, I2C_TRUE);														// I2C Block Reset
		USI_I2C_Enable(ch, I2C_TRUE);																				// I2C Enable
		USI_I2C_ConfigureInterrupt(ch, I2C_TRUE);											// I2C Interrupt Enable
		USI_I2C_ConfigureAcknowledge(ch, I2C_ACK_ENABLE);	// I2C Ack Enable

		I2C0->SAR1 = addr;																													// Set slave own address

		I2C0->SCLR = ((SystemPeriClock/speed) - 2) / 4;						// Set I2C speed_low
		I2C0->SCHR = ((SystemPeriClock/speed) - 2) / 4;						// Set I2C speed_high
		I2C0->SDHR = 1;																															// Set I2C data hold time
		
		i2c_mode[ch] = I2C_IDLE;
	}

	else if(ch == I2C_CH1)
	{
//		SCUCG->PPEN2 |= (0x1UL<<7);
		SCUCG->PPCLKEN2 |= (0x1UL<<7);																	// Peri. Clock Enable

		USI_I2C_SoftwareReset(ch, I2C_TRUE);														// I2C Block Reset
		USI_I2C_Enable(ch, I2C_TRUE);																				// I2C Enable
		USI_I2C_ConfigureInterrupt(ch, I2C_TRUE);											// I2C Interrupt Enable
		USI_I2C_ConfigureAcknowledge(ch, I2C_ACK_ENABLE);	// I2C Ack Enable

		I2C1->SAR1 = addr;																													// Set slave own address

		I2C1->SCLR = ((SystemPeriClock/speed) - 2) / 4;						// Set I2C speed_low
		I2C1->SCHR = ((SystemPeriClock/speed) - 2) / 4;						// Set I2C speed_high
		I2C1->SDHR = 1;																															// Set I2C data hold time
		
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
void USI_I2C_SoftwareReset(uint8_t ch, uint8_t enable)
{
	uint8_t i;
	
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
			SCUCG->PPRST2 |= (0x1UL<<6);
			 for( i = 0 ; i < 10 ; i++ )
			 {
					__NOP();
			 }
			SCUCG->PPRST2 &= ~(0x1UL<<6);
		}
		else
		{
			SCUCG->PPRST2 &= ~(0x1UL<<6);
		}
	}
	else if(ch == I2C_CH1)
	{
		if(enable == I2C_TRUE)
		{
			SCUCG->PPRST2 |= (0x1UL<<7);
			 for( i = 0 ; i < 10 ; i++ )
			 {
					__NOP();
			 }
			SCUCG->PPRST2 &= ~(0x1UL<<7);
		}
		else
		{
			SCUCG->PPRST2 &= ~(0x1UL<<7);
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
void USI_I2C_Enable(uint8_t ch, uint8_t enable)
{
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
* @brief		Configure the enable or disable I2C interrupt.
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   enable   This parameter contains the enable of this function. 
* 					- FALSE = 0
*					- I2C_TRUE = 1
* @return		None
*/
void USI_I2C_ConfigureInterrupt(uint8_t ch, uint8_t enable)
{
	if(ch == I2C_CH0)
	{
		if(enable == I2C_TRUE)
		{
      NVIC_ClearPendingIRQ( I2C0_IRQn );
      NVIC_EnableIRQ( I2C0_IRQn );
			INTC->MSK |= (0x1UL<<10);
			I2C0->CR |= (fI2CnIEN);
		}
		else
		{
      NVIC_DisableIRQ( I2C0_IRQn );
			I2C0->CR &= ~(fI2CnIEN);
		}
	}
	else if(ch == I2C_CH1)
	{
		if(enable == I2C_TRUE)
		{
      NVIC_ClearPendingIRQ( I2C1_IRQn );
      NVIC_EnableIRQ( I2C1_IRQn );
			INTC->MSK |= (0x1UL<<14);
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
* @brief		Configure the I2C Acknowledge signal.
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   enable   This parameter contains the enable of this function. 
* 					- I2C_FALSE = 0
*					- I2C_TRUE = 1
* @return		None
*/
void USI_I2C_ConfigureAcknowledge(uint8_t ch, uint8_t enable)
{
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
* @brief		Enables or disables the I2C general call function.
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   enable   This parameter contains the enable of this function. 
* 					- I2C_FALSE = 0
*					- I2C_TRUE = 1
* @return		None
*/
void USI_I2C_ConfigureGerneralCall(uint8_t ch, uint8_t enable)
{
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

/**
* @brief		Enables or disables the I2C Start condition.
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   enable   This parameter contains the enable of this function. 
* 					- I2C_FALSE = 0
*					- I2C_TRUE = 1
* @return		None
*/
void USI_I2C_Start(uint8_t ch, uint8_t enable)
{
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
* @brief		Enables or disables the I2C Stop condition.
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   enable   This parameter contains the enable of this function. 
* 					- I2C_FALSE = 0
*					- I2C_TRUE = 1
* @return		None
*/
void USI_I2C_Stop(uint8_t ch, uint8_t enable)
{
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
* @brief		Send slave address as i2c master.
* @param   ch		This parameter contains the channel of USI peripheral.
* @param   dev_addr			This parameter contains the slave address of target.
* @param   direction			This parameter contains the mode of i2c(read or write).
* 					- I2C_WRITE_MODE = 0
*					- I2C_READ_MODE = 1
* @return		None
*/
void USI_I2C_Send7bitAddress(uint8_t ch, uint8_t dev_addr, uint8_t direction)
{
	/* Set address & Set read/write direction */
	if(ch == I2C_CH0)
	{
		I2C0->DR = dev_addr | direction;
	}
	else if(ch == I2C_CH1)
	{
		I2C1->DR = dev_addr | direction;
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
void USI_I2C_MasterTransferData(uint8_t ch, uint8_t dev_addr, uint8_t *write_data, uint8_t write_len, uint8_t *read_data, uint8_t read_len)
{
	uint8_t i;
	uint32_t timekeeper;

	/* Variable Initialization */
	receiveNum[ch] = 0;
	transmitNum[ch] = 0;
	receiveCount[ch] = read_len;
	transmitCount[ch] = write_len;
	
	/* Set Tx Buffer */
	if(ch == I2C_CH0)
	{
		for(i = 0; i < write_len; i++)
		{
			transmitBuffer0[i] = write_data[i];
		}
		if(I2C0->ST & fBUSY)
		{
			USI_I2C_Initial(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
		}
	}
	else if(ch == I2C_CH1)
	{
		for(i = 0; i < write_len; i++)
		{
			transmitBuffer1[i] = write_data[i];
		}
		if(I2C1->ST & fBUSY)
		{
			USI_I2C_Initial(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
		}
	}

	timekeeper = 0x5FFFF;		// 60ms @ Sysclk 40MHz
	i2c_mode[ch] = I2C_BUSY;
	
	/* TRX Command */
	if(write_len)
	{
		USI_I2C_Send7bitAddress(ch, dev_addr, I2C_WRITE_MODE);
	}
	else
	{
		USI_I2C_Send7bitAddress(ch, dev_addr, I2C_READ_MODE);
	}

	/* Master transmit/receive wait */
	while(i2c_mode[ch] == I2C_BUSY)
	{
		timekeeper--;
		if(timekeeper == 0)		// Timeout
		{
			i2c_mode[ch] = I2C_IDLE;
			USI_I2C_Initial(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
		}

		#ifdef	I2C_POLLING
		USI_I2C_InterruptHandler(ch);		// Activate when polling mode
		#endif
	}

	/* Set Rx Buffer */
	if(ch == I2C_CH0)
	{
		for(i = 0; i < read_len; i++)
		{
			read_data[i] = receiveBuffer0[i];
		}
	}
	else if(ch == I2C_CH1)
	{
		for(i = 0; i < read_len; i++)
		{
			read_data[i] = receiveBuffer1[i];
		}
	}
}

/**
* @brief		Set specific bytes as i2c slave.(custom user)
* @param   ch		This parameter contains the channel of USI peripheral.
* @return		None
*/
void USI_I2C_SetSlaveData(uint8_t ch)
{
	uint8_t i;
	uint32_t timekeeper;

	receiveNum[ch] = 0;
	transmitNum[ch] = 0;

	/* Set Tx/Rx Buffer */
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
			receiveBuffer0[i] = 0x00;
		}
	}

	timekeeper = 0x5FFFF;		// 60ms @ Sysclk 40MHz
	i2c_mode[ch] = I2C_BUSY;

	/* Slave transmit/receive wait */
	while(i2c_mode[ch] == I2C_BUSY)
	{
		timekeeper--;
		if(timekeeper == 0)		// Timeout
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
* @brief		Configure the enable or disable I2C interrupt.
* @param   ch		This parameter contains the channel of USI peripheral.
* @return		None
*/
void USI_I2C_InterruptHandler(uint8_t ch)
{
	uint8_t i2c_status, i2c_operation;
	
	if(ch == I2C_CH0)
	{
		i2c_status = I2C0->ST;
		i2c_operation = I2C0->CR;
	}
	else if(ch == I2C_CH1)
	{
		i2c_status = I2C1->ST;
		i2c_operation = I2C1->CR;
	}

	// To check mastership and stop signal
	if((i2c_status & fMLOST) || (i2c_status & fSTOPD))
	{
		if(ch == I2C_CH0)
		{
			i2c_mode[ch] = I2C_IDLE;
			USI_I2C_Initial(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
		}
		else if(ch == I2C_CH1)
		{
			i2c_mode[ch] = I2C_IDLE;
			USI_I2C_Initial(ch, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);
		}
	}
	// To enter I2C trx process
	else
	{
			if(i2c_operation & fIMASTERn)
			{
				USI_I2C_MasterProcessData(ch);
			}
			else
			{
				USI_I2C_SlaveProcessData(ch);
			}
	}
}

/**
* @brief		Response specific bytes as i2c master.(custom user)
* @param   ch		This parameter contains the channel of USI peripheral.
* @return		None
*/
static void USI_I2C_MasterProcessData(uint8_t ch)
{
	uint8_t i;
	uint8_t i2c_status;
	
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
		// Master Tx mode
		if(i2c_status & fTMODE)
		{
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
			else
			{
				if(receiveCount[ch])
				{
					// Restart for Rx
					USI_I2C_Send7bitAddress(ch, I2C_DEVICE_ADDRESS, I2C_READ_MODE);
				}
				else
				{
					// TxCount 0 Stop
					transmitNum[ch] = 0;
					USI_I2C_Stop(ch, I2C_TRUE);
				}
			}
		}
		// Master Rx mode
		else
		{
			if(i2c_status&fGCALL)
			{
				if(receiveCount[ch] == 1)
				{
					// NoAck generate
					USI_I2C_ConfigureAcknowledge(ch, I2C_ACK_DISABLE);
				}
			}
			else
			{
				receiveCount[ch] -= 1;
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
					USI_I2C_ConfigureAcknowledge(ch, I2C_ACK_DISABLE);		// NoAck generate
				}
				else
				{
					// RxCount 0 Stop
					receiveNum[ch] = 0;
					USI_I2C_Stop(ch, I2C_TRUE);
				}
			}
		}
	}
	// No ACK
	else
	{
		// NoAck data is received
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
		USI_I2C_Stop(ch, I2C_TRUE);
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
* @brief		Response specific bytes as i2c slave.(custom user)
* @param   ch		This parameter contains the channel of USI peripheral.
* @return		None
*/
static void USI_I2C_SlaveProcessData(uint8_t ch)
{
	uint8_t i2c_status;
	
	if(ch == I2C_CH0)
	{
		i2c_status = I2C0->ST;
	}
	else if(ch == I2C_CH1)
	{
		i2c_status = I2C1->ST;
	}

	if(i2c_status & fGCALL)
	{
		/* Insert user specific slave code */
		__nop();
	}
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
* @brief		Get the I2C interrupt status.
* @param   ch		This parameter contains the channel of USI peripheral.
* @return		Value of status(interrupt flag)
*/
uint8_t USI_I2C_GetInterruptStatus(uint8_t ch)
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

/************************ (C) COPYRIGHT ABOV SEMICONDUCTOR *****END OF FILE****/