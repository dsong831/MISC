/**********************************************************************//**
* @file				A31T21x_hal_usart1n.c
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
#include "A31T21x_hal_usart1n.h"


/* Private variable -------------------------------------------------------- */
uint8_t u8UstDummy;

/* Private Types --------------------------------------------------------------- */
ustRingBuffer_Type	tx10_RingBuffer;
ustRingBuffer_Type	tx11_RingBuffer;
ustRingBuffer_Type	rx10_RingBuffer;
ustRingBuffer_Type	rx11_RingBuffer;

ustSPIBuffer_Type	spi_tx10_Buffer;
ustSPIBuffer_Type	spi_tx11_Buffer;
ustSPIBuffer_Type	spi_rx10_Buffer;
ustSPIBuffer_Type	spi_rx11_Buffer;


/* Public Functions -------------------------------------------------------- */
/**********************************************************************//**
* @brief						Read a single data from USART peripheral (Polling mode)
* @param[in]	USARTn	USART peripheral selected, should be:
*											- USART10 : USART10 peripheral
*											- USART11 : USART11 peripheral
* @return					Received data
 **********************************************************************/
uint8_t HAL_USART_ReadBuffer(USART_Type* USARTn)
{
	if(!(USARTn->CR1 & (3<<14)))
	{
		u8UstDummy = USARTn->DR;		// Dummy read
	}

	while(!(USARTn->ST & USART_ST_RXC));
	return USARTn->DR;
}

/**********************************************************************//**
 * @brief						Write a single data through USART peripheral (Polling mode)
 * @param[in]	USARTn		USART peripheral selected, should be:
 *											- USART0 : USART10 peripheral
 *											- USART1 : USART11 peripheral
 * @param[in]	tx_data		Data to transmit
 * @return				None
 **********************************************************************/
void HAL_USART_WriteBuffer(USART_Type* USARTn, uint8_t tx_data)
{
	while(!(USARTn->ST & USART_ST_DRE));	// Wait for current transmit complete
	USARTn->DR = tx_data;
}

/**********************************************************************//**
 * @brief						Initializes the USARTn peripheral according to the specified
 *											parameters in the USART_ConfigStruct.
 * @param[in]	USARTn			USART peripheral selected, should be:
 *											- USART10 : USART10 peripheral
 *											- USART11 : USART11 peripheral
 * @param[in]	USARTModeConfig		USART mode selected, should be:
 *											- USART_UART_MODE
 *											- USART_USART_MODE
 *											- USART_SPI_MODE
 * @param[in]	USARTConfigStruct		Pointer to a USART_CFG_Type structure
 *											that contains the configuration information for the
 *											specified USART peripheral.
 *											- tBaudRate / tDataLength / tStopbit / tParity
 * @param[in]	USARTIntStruct		USART Interrupt selected, should be:
 *											- UART_INT_POLLING
 *											- USART_INT_RXCIE
 *											- USART_INT_DRIE
 *											- USART_INT_RXCIE_DRIE
 * @return					HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_USART_Init(USART_Type *USARTn, USART_MODE_Type USARTModeConfig, USART_CFG_Type *USARTConfigStruct, USART_INTCFG_Type USARTIntConfig)
{
	uint32_t u32Reg = 0;

	/* USART Block Reset */
	if(USARTn == USART10)
	{
		SCU->PER2 &= ~(1<<8);
		SCU->PCER2 &= ~(1<<8);
		SCU->PER2 |= (1<<8);
		SCU->PCER2 |= (1<<8);		
	}
	else if(USARTn == USART11)
	{
		SCU->PER2 &= ~(1<<9);
		SCU->PCER2 &= ~(1<<9);
		SCU->PER2 |= (1<<9);
		SCU->PCER2 |= (1<<9);
	}
	else
	{
		return HAL_ERROR;
	}	

	/* USART Register Initialize */
	USARTn->CR1 = 0;
	USARTn->CR2 = 0;
	u8UstDummy = USARTn->ST;

	///////////////////////////////////////////////////
	/* UART/USART MODE */
	if((USARTModeConfig == USART_UART_MODE) || (USARTModeConfig == USART_USART_MODE))
	{
		/* USART RingBuffer Initialize */
		HAL_USART_RingBuffer_Init(USARTn);

		/* Set BaudRate */
		if(USARTModeConfig == USART_UART_MODE)
		{
			HAL_USART_BaudrateSet(USARTn, USART_UART_MODE, (USARTConfigStruct->tBaudRate));
		}
		else
		{
			HAL_USART_BaudrateSet(USARTn, USART_USART_MODE, (USARTConfigStruct->tBaudRate));
		}

		/* Set Mode */
		if(USARTModeConfig == USART_UART_MODE)
		{
			u32Reg &= ~(3<<14);	// UART mode
		}
		else
		{
			u32Reg |= (1<<14);		// USART mode
		}

		/* Set Parity */
		if(USARTConfigStruct->tParity == USART_PARITY_NONE)
		{
			u32Reg &= ~(3<<12);
		}
		else
		{
			switch(USARTConfigStruct->tParity)
			{
				case USART_PARITY_EVEN :
					u32Reg |= (2<<12);
					break;
				case USART_PARITY_ODD :
					u32Reg |= (3<<12);
					break;
				default :
					u32Reg &= ~(3<<12);
					break;
			}
		}

		/* Set DataLength */
		switch(USARTConfigStruct->tDataLength)
		{
			case USART_DATALEN_5 :
				u32Reg &= ~(7<<9);
				break;
			case USART_DATALEN_6 :
				u32Reg |= (1<<9);
				break;
			case USART_DATALEN_7 :
				u32Reg |= (2<<9);
				break;
			case USART_DATALEN_8 :
				u32Reg |= (3<<9);
				break;
			case USART_DATALEN_9 :
				u32Reg |= (7<<9);
				break;
			default :
				u32Reg |= (3<<9);
				break;
		}
		USARTn->CR1 = u32Reg | (3<<0);	// Set Mode/Parity/DataLength/TRX

		/* Set StopBit */
		switch(USARTConfigStruct->tStopbit)
		{
			case USART_STOPBIT_2 :
				USARTn->CR2 |= (1<<2);
				break;
			case USART_STOPBIT_1 :
			default:
				USARTn->CR2 &= ~(1<<2);
				break;
		}
		USARTn->CR2 |= (1<<9);	// Enable USARTn block

		/* UART Interrupt Setting */
		u32Reg = 0;
		switch(USARTIntConfig)
		{
			case USART_INT_POLLING:
				u32Reg = 0;
				break;
			case USART_INT_RXCIE:
				u32Reg |= (1<<3);
				break;
			case USART_INT_TXCIE:
				u32Reg |= (1<<4);
				break;
			case USART_INT_RXCIE_TXCIE:
				u32Reg |= (1<<3) | (1<<4);
				break;
			default :
				u32Reg = 0;
				break;
		}
		USARTn->CR1 |= u32Reg;		// Set interrupt

		/* Set USART Additional Function */
		// MS Config
		if(USARTConfigStruct->tMS == USART_MS_MASTER)
		{
			USARTn->CR2 |= (1<<7);			// Master mode
			USARTn->CR2 |= (1<<5);			// Config SCK output (0: free-running, 1: SCK is active while any frame is on transferring.)
		}
		else
		{
			USARTn->CR2 &= ~(1<<7);		// Slave mode
			USARTn->CR2 |= (1<<5);			// Config SCK output (0: free-running, 1: SCK is active while any frame is on transferring.)
		}
		// CPOL Config
		if(USARTConfigStruct->tCPOL == USART_CPOL_ActiveLow)
		{
			USARTn->CR1 &= ~(1<<7);
		}
		else
		{
			USARTn->CR1 |= (1<<7);
		}
	}

	///////////////////////////////////////////////////
	/* SPI MODE */
	else if(USARTModeConfig == USART_SPI_MODE)
	{
		u32Reg = 0;

		// Set SPI Mode
		u32Reg = (3<<14);
		
		// DataLength Config
		switch(USARTConfigStruct->tDataLength)
		{
			case USART_DATALEN_5 :
				u32Reg &= ~(7<<9);
				break;
			case USART_DATALEN_6 :
				u32Reg |= (1<<9);
				break;
			case USART_DATALEN_7 :
				u32Reg |= (2<<9);
				break;
			case USART_DATALEN_8 :
				u32Reg |= (3<<9);
				break;
			case USART_DATALEN_9 :
				u32Reg |= (7<<9);
				break;
			default :
				u32Reg |= (3<<9);
				break;
		}
		// MSBF Config
		if(USARTConfigStruct->tMSBF == USART_MSBF_MSB)
		{
			u32Reg |= (1<<8);
		}
		else
		{
			u32Reg &= ~(1<<8);
		}
		// CPOL Config
		if(USARTConfigStruct->tCPOL == USART_CPOL_ActiveLow)
		{
			u32Reg &= ~(1<<7);
		}
		else
		{
			u32Reg |= (1<<7);
		}
		// CPHA Config
		if(USARTConfigStruct->tCPHA == USART_CPHA_EVEN)
		{
			u32Reg |= (1<<6);
		}
		else
		{
			u32Reg &= ~(1<<6);
		}
		USARTn->CR1 = u32Reg | (3<<0);	// Set Mode/DataLength/ORD/CPOL/CPHA/TRX

		// MS Config
		if(USARTConfigStruct->tMS == USART_MS_MASTER)
		{
			USARTn->CR2 |= (1<<7);			// Master mode
		}
		else
		{
			USARTn->CR2 &= ~(1<<7);		// Slave mode
		}

		/* Set BaudRate */
		HAL_USART_BaudrateSet(USARTn, USART_SPI_MODE, (USARTConfigStruct->tBaudRate));

		/* SPI Interrupt Setting */
		u32Reg = 0;
		switch(USARTIntConfig)
		{
			case USART_INT_POLLING:
				u32Reg = 0;
				break;
			case USART_INT_RXCIE:
				u32Reg |= (1<<3);
				break;
			case USART_INT_TXCIE:
				// Enable TXIE at Transmit Function
				break;
			case USART_INT_RXCIE_TXCIE:
				u32Reg |= (1<<3);
				// Enable TXIE at Transmit Function
				break;
			default :
				u32Reg = 0;
				break;
		}
		USARTn->CR1 |= u32Reg;		// Set interrupt
	}

	else
	{
		return HAL_ERROR;
	}

	return HAL_OK;
}

/*********************************************************************//**
 * @brief						Determines best dividers to get a target clock rate
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @param[in]	USARTModeConfig		USART mode selected, should be:
 *											- USART_UART_MODE
 *											- USART_USART_MODE
 *											- USART_SPI_MODE
 * @param[in]	baudrate Desired USART baud rate.
 * @return				None
 **********************************************************************/
void HAL_USART_BaudrateSet(USART_Type *USARTn, USART_MODE_Type USARTModeConfig, uint32_t baudrate)
{
	uint32_t numerator;
	uint32_t denominator; 
	uint32_t bdr;

	if(USARTModeConfig == USART_UART_MODE)
	{
		//------------------------------------------------------------------------------
		// Baud Rate = PCLK / (16(USTnBD+1))	@ Normal Mode
		// Baud Rate = PCLK / (8(USTnBD+1))		@ Double Speed Mode
		//------------------------------------------------------------------------------
		numerator = SystemPeriClock;
		if(USARTn->CR2 & (1<<8))
		{
			denominator = 8 * baudrate;		// Double Speed Mode
		}
		else
		{
			denominator = 16 * baudrate;	// Normal Mode
		}

		bdr = (numerator / denominator) - 1;

		/* Baudrate Error Check */
		if(bdr <= 5)
		{
			// Error then to change speed mode
			USARTn->CR2 |= (1<<8);
			denominator = 8 * baudrate;	// Double Speed Mode
			bdr = (numerator / denominator) - 1;
		}

		USARTn->BDR= (uint16_t)(bdr&0xffff);
	}
	else if((USARTModeConfig == USART_SPI_MODE) || (USARTModeConfig == USART_USART_MODE))
	{
		//------------------------------------------------------------------------------
		// Baud Rate = PCLK / (2(USTnBD+1))	@ SPI Mode
		//------------------------------------------------------------------------------
		numerator = SystemPeriClock;
		denominator = 2 * baudrate;		// SPI Mode
		bdr = (numerator / denominator) - 1;
		USARTn->BDR= (uint16_t)(bdr&0xffff);
	}
}

/**********************************************************************//**
 * @brief						Enable or disable USART peripheral's operation (For using SPI mode)
 * @param[in]	USARTn	USART peripheral, should be:
 *											- USART	: USARTI10~11 peripheral
 * @param[in]	usart_en New State of USARTn peripheral's operation, should be:
 *											- ENABLE
 *											- DISABLE
 * @return				None
 **********************************************************************/
void HAL_USART_Command(USART_Type* USARTn, EN_DIS_Type usart_en)
{
	if(usart_en == ENABLE)
	{
		USARTn->CR2 |= (1<<9);	// Data must be written to the DR before this bit is set enable @ SPI mode
	}
	else
	{
		USARTn->CR2 &= ~(1<<9);
	}
}

/**********************************************************************//**
 * @brief						control SS Output in SPI peripheral (Only SPI Slave Mode)
 * @param[in]	USARTn	USART peripheral selected, should be:
 *											- USARTn	:	USART10~11 peripheral
 * @param[in]	ss_en	 State of Slave Output , should be:
 *											- ENABLE
 *											- DISABLE
 * @return				None
 **********************************************************************/
void HAL_USART_SetSSOutput(USART_Type* USARTn, EN_DIS_Type ss_en)
{
	if(ss_en == ENABLE)
	{
		USARTn->CR2 |= (1<<4);
	}
	else
	{
		USARTn->CR2 &= ~(1<<4);
	}
}

/**********************************************************************//**
 * @brief						Transmit a single data through USART_SPIn peripheral (Polling mode)
 * @param[in]	USARTn	USART peripheral selected, should be:
 *											- USARTn	:		USART10~11 peripheral
 * @param[in]	tx_data		Data to transmit
 * @return				None
 **********************************************************************/
void HAL_USART_SPI_TransmitData_POL(USART_Type* USARTn, uint32_t tx_data)
{
	while(!(USARTn->ST & USART_ST_DRE));	// Wait until transmit buffer is ready for use.
	HAL_USART_Command(USARTn, DISABLE);
	USARTn->DR = tx_data;
	HAL_USART_Command(USARTn, ENABLE);
}

/**********************************************************************//**
 * @brief						Receive a single data from USART_SPIn peripheral (Polling mode)
 * @param[in]	USARTn	USART peripheral selected, should be
 *											- USARTn	:		USART10~11 peripheral
 * @return				Received data
 **********************************************************************/
uint32_t HAL_USART_SPI_ReceiveData_POL(USART_Type* USARTn)
{
	while(!(USARTn->ST & USART_ST_DRE));	// Wait until transmit buffer is ready for use.
	USARTn->DR = 0x00;																	// Dummy data
	while(!(USARTn->ST & USART_ST_RXC));	// Wait until receive buffer holds data.
	return ((uint32_t) (USARTn->DR));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


/*********************************************************************//**
 * @brief						Initialize RingBuffer Parameters
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @return				None
 **********************************************************************/
void HAL_USART_RingBuffer_Init(USART_Type *USARTn)
{
	if(USARTn == USART10)
	{
		tx10_RingBuffer.HeadPtr = 0;
		tx10_RingBuffer.TailPtr = 0;
		tx10_RingBuffer.State = USART_UART_TX_IDLE;
		
		rx10_RingBuffer.HeadPtr = 0;
		rx10_RingBuffer.TailPtr = 0;
		rx10_RingBuffer.State = USART_UART_RX_IDLE;
	}
	else if(USARTn == USART11)
	{
		tx11_RingBuffer.HeadPtr = 0;
		tx11_RingBuffer.TailPtr = 0;
		tx11_RingBuffer.State = USART_UART_TX_IDLE;

		rx11_RingBuffer.HeadPtr = 0;
		rx11_RingBuffer.TailPtr = 0;
		rx11_RingBuffer.State = USART_UART_RX_IDLE;
	}
}

/*********************************************************************//**
 * @brief						USART Transmit Data (Interrupt mode)
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @param[in]	tx_data		USART Transmit Data
 * @return				None
 **********************************************************************/
void HAL_USART_TransmitData(USART_Type *USARTn, uint8_t tx_data)
{
	USARTn->CR1 &= ~(1<<4);		// Disable txc interrupt
	
	/* USART10 Unit */
	if(USARTn == USART10)
	{
		if(tx10_RingBuffer.HeadPtr == tx10_RingBuffer.TailPtr)
		{
			if(tx10_RingBuffer.State == USART_UART_TX_IDLE)
			{
				HAL_USART_WriteBuffer(USARTn, tx_data);		// First data to transmit
				tx10_RingBuffer.State = USART_UART_TX_BUSY;
			}
			else
			{
				tx10_RingBuffer.Buffer[tx10_RingBuffer.HeadPtr++] = tx_data;
				// Check buffer max length
				if(tx10_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
				{
					tx10_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			tx10_RingBuffer.Buffer[tx10_RingBuffer.HeadPtr++] = tx_data;
			// Check buffer max length
			if(tx10_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
			{
				tx10_RingBuffer.HeadPtr = 0;
			}
		}
	}
	/* USART11 Unit */
	else if(USARTn == USART11)
	{
		if(tx11_RingBuffer.HeadPtr == tx11_RingBuffer.TailPtr)
		{
			if(tx11_RingBuffer.State == USART_UART_TX_IDLE)
			{
				HAL_USART_WriteBuffer(USARTn, tx_data);		// First data to transmit
				tx11_RingBuffer.State = USART_UART_TX_BUSY;
			}
			else
			{
				tx11_RingBuffer.Buffer[tx11_RingBuffer.HeadPtr++] = tx_data;
				// Check buffer max length
				if(tx11_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
				{
					tx11_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			tx11_RingBuffer.Buffer[tx11_RingBuffer.HeadPtr++] = tx_data;
			// Check buffer max length
			if(tx11_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
			{
				tx11_RingBuffer.HeadPtr = 0;
			}
		}
	}

	USARTn->CR1 |= (1<<4);		// Enable txc interrupt
}

/*********************************************************************//**
 * @brief						USART Receive Data (Interrupt mode)
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @return				Received data
 **********************************************************************/
int8_t HAL_USART_ReceiveData(USART_Type *USARTn)
{
	/* USART10 Unit */
	if(USARTn == USART10)
	{
		if(rx10_RingBuffer.HeadPtr != rx10_RingBuffer.TailPtr)
		{
			// Check buffer max length
			if(rx10_RingBuffer.TailPtr > ustRING_BUFFER_LENGTH)
			{
				rx10_RingBuffer.TailPtr = 0;
				return rx10_RingBuffer.Buffer[rx10_RingBuffer.TailPtr];
			}
			return rx10_RingBuffer.Buffer[rx10_RingBuffer.TailPtr++];
		}
		else
		{
				// No received data
				rx10_RingBuffer.State = USART_UART_RX_IDLE;
				return -1;
		}
	}
	/* USART11 Unit */
	else if(USARTn == USART11)
	{
		if(rx11_RingBuffer.HeadPtr != rx11_RingBuffer.TailPtr)
		{
			// Check buffer max length
			if(rx11_RingBuffer.TailPtr > ustRING_BUFFER_LENGTH)
			{
				rx11_RingBuffer.TailPtr = 0;
				return rx11_RingBuffer.Buffer[rx11_RingBuffer.TailPtr];
			}
			return rx11_RingBuffer.Buffer[rx11_RingBuffer.TailPtr++];
		}
		else
		{
				// No received data
				rx11_RingBuffer.State = USART_UART_RX_IDLE;
				return -1;
		}
	}
	/* No Unit */
	else
	{
		return -1;
	}
}

/*********************************************************************//**
 * @brief						USART Handler (Interrupt Handler)
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @return				None
 **********************************************************************/
void HAL_USART_UART_Handler(USART_Type *USARTn)
{
	volatile uint32_t int_status;
	volatile uint32_t line_error;
	
	int_status = USARTn->ST;

	/* Line Interrupt */
	if(int_status & 0x07)
	{
		line_error = USARTn->ST;									// Clear line Error Interrupt
	}
	
	/* Rx Interrupt */
	if(int_status & 0x20)
	{
		HAL_USART_UART_RX_Handler(USARTn);		// Rx Process
	}
	
	/* Tx Interrupt */
	if(int_status & 0x40)
	{
		HAL_USART_UART_TX_Handler(USARTn);		// Tx Process
		USARTn->ST &= ~(0x80);										// Clear TXC Flag
	}
}

/*********************************************************************//**
 * @brief						USART Transmit Handler (Interrupt Handler)
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @return				None
 **********************************************************************/
void HAL_USART_UART_TX_Handler(USART_Type *USARTn)
{
	/* USART10 Unit */
	if(USARTn == USART10)
	{
		if(tx10_RingBuffer.HeadPtr != tx10_RingBuffer.TailPtr)
		{
			if(USARTn->ST & 0x80)
			{
				USARTn->DR = tx10_RingBuffer.Buffer[tx10_RingBuffer.TailPtr++];
				// Check buffer max length
				if(tx10_RingBuffer.TailPtr > ustRING_BUFFER_LENGTH)
				{
					tx10_RingBuffer.TailPtr = 0;
				}
			}
		}
		else
		{
				tx10_RingBuffer.State = USART_UART_TX_IDLE;
		}
	}
	/* USART11 Unit */
	else if(USARTn == USART11)
	{
		if(tx11_RingBuffer.HeadPtr != tx11_RingBuffer.TailPtr)
		{
			if(USARTn->ST & 0x80)
			{
				USARTn->DR = tx11_RingBuffer.Buffer[tx11_RingBuffer.TailPtr++];
				// Check buffer max length
				if(tx11_RingBuffer.TailPtr > ustRING_BUFFER_LENGTH)
				{
					tx11_RingBuffer.TailPtr = 0;
				}
			}
		}
		else
		{
				tx11_RingBuffer.State = USART_UART_TX_IDLE;
		}
	}
}

/*********************************************************************//**
 * @brief						USART Receive Handler (Interrupt Handler)
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @return				None
 **********************************************************************/
void HAL_USART_UART_RX_Handler(USART_Type *USARTn)
{
	/* USART10 Unit */
	if(USARTn == USART10)
	{
		if(rx10_RingBuffer.HeadPtr == rx10_RingBuffer.TailPtr)
		{
			if(rx10_RingBuffer.State == USART_UART_RX_IDLE)
			{
				rx10_RingBuffer.Buffer[rx10_RingBuffer.HeadPtr++] = USARTn->DR;		// First data to receive
				rx10_RingBuffer.State = USART_UART_RX_BUSY;
				// Check buffer max length
				if(rx10_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
				{
					rx10_RingBuffer.HeadPtr = 0;
				}
			}
			else
			{
				rx10_RingBuffer.Buffer[rx10_RingBuffer.HeadPtr++] = USARTn->DR;
				// Check buffer max length
				if(rx10_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
				{
					rx10_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			rx10_RingBuffer.Buffer[rx10_RingBuffer.HeadPtr++] = USARTn->DR;
			// Check buffer max length
			if(rx10_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
			{
				rx10_RingBuffer.HeadPtr = 0;
			}
		}
	}
	/* USART11 Unit */
	else if(USARTn == USART11)
	{
		if(rx11_RingBuffer.HeadPtr == rx11_RingBuffer.TailPtr)
		{
			if(rx11_RingBuffer.State == USART_UART_RX_IDLE)
			{
				rx11_RingBuffer.Buffer[rx11_RingBuffer.HeadPtr++] = USARTn->DR;		// First data to receive
				rx11_RingBuffer.State = USART_UART_RX_BUSY;
				// Check buffer max length
				if(rx11_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
				{
					rx11_RingBuffer.HeadPtr = 0;
				}
			}
			else
			{
				rx11_RingBuffer.Buffer[rx11_RingBuffer.HeadPtr++] = USARTn->DR;
				// Check buffer max length
				if(rx11_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
				{
					rx11_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			rx11_RingBuffer.Buffer[rx11_RingBuffer.HeadPtr++] = USARTn->DR;
			// Check buffer max length
			if(rx11_RingBuffer.HeadPtr > ustRING_BUFFER_LENGTH)
			{
				rx11_RingBuffer.HeadPtr = 0;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/*********************************************************************//**
 * @brief						Initialize Buffer Parameters
 * @param[in]	USARTn	Pointer to selected USART-SPI peripheral, should be:
 *											- USART10	:		USART10 peripheral
 *											- USART11	:		USART11 peripheral
 * @return				None
 **********************************************************************/
void HAL_USART_SPIBuffer_Init(USART_Type *USARTn)
{
	if(USARTn == USART10)
	{
		spi_tx10_Buffer.DataLength = 0;
		spi_tx10_Buffer.State = USART_SPI_TX_IDLE;

		spi_rx10_Buffer.DataLength = 0;
		spi_rx10_Buffer.State = USART_SPI_RX_IDLE;
	}
	else if(USARTn == USART11)
	{
		spi_tx11_Buffer.DataLength = 0;
		spi_tx11_Buffer.State = USART_SPI_TX_IDLE;

		spi_rx11_Buffer.DataLength = 0;
		spi_rx11_Buffer.State = USART_SPI_RX_IDLE;
	}
}

/*********************************************************************//**
 * @brief						USART-SPI Transmit/Receive Data (Interrupt mode)
 * @param[in]	USARTn	Pointer to selected USART peripheral, should be:
 *											- USART10	:		USART10 peripheral
 *											- USART11	:		USART11 peripheral
 * @param[in]	*p_txdata		This parameter contains the write data of SPI (pointer variable)
 * @param[in]	tx_length		This parameter contains the number of write length.
 * @param[in]	*p_rxdata		This parameter contains the read data of SPI (pointer variable)
 * @param[in]	rx_length		This parameter contains the number of read length.
 * @return				None
 **********************************************************************/
void HAL_USART_SPI_TransmitReceiveData_INT(USART_Type *USARTn, uint32_t *p_txdata, uint8_t tx_length, uint32_t *p_rxdata, uint8_t rx_length)
{
	uint8_t i;
	uint32_t timekeeper = 0x5FFFF;

	/* SPI Buffer Initialize */
	HAL_USART_SPIBuffer_Init(USARTn);
	
	/* USART10 Unit */
	if(USARTn == USART10)
	{
		// Variable Initialization
		spi_tx10_Buffer.DataLength = tx_length;
		spi_rx10_Buffer.DataLength = rx_length;
		spi_tx10_Buffer.State = USART_SPI_TX_BUSY;
		spi_rx10_Buffer.State = USART_SPI_RX_BUSY;

		// load tx_data to global buffer from local buffer
		for(i=0; i<tx_length; i++)
		{
			spi_tx10_Buffer.Buffer[i] = p_txdata[i];
		}

		// Enable SPI Tx Interrupt
		USARTn->CR1 |= (1<<5);		// Enable tx interrupt

		// SPI timeout check
		while((spi_tx10_Buffer.State == USART_SPI_TX_BUSY) || (spi_rx10_Buffer.State == USART_SPI_RX_BUSY))
		{
			timekeeper--;
			if(timekeeper == 0)
			{
				HAL_USART_Command(USARTn, DISABLE);
				USARTn->CR1 &= ~(1<<5);		// Disable tx interrupt
				HAL_USART_SPIBuffer_Init(USARTn);
			}
		}

		// load rx_data to local buffer from global buffer
		for(i=0; i<rx_length; i++)
		{
			p_rxdata[i] = spi_rx10_Buffer.Buffer[i+1];
		}
	}

	/* USART11 Unit */
	else if(USARTn == USART11)
	{
		// Variable Initialization
		spi_tx11_Buffer.DataLength = tx_length;
		spi_rx11_Buffer.DataLength = rx_length;
		spi_tx11_Buffer.State = USART_SPI_TX_BUSY;
		spi_rx11_Buffer.State = USART_SPI_RX_BUSY;

		// load tx_data to global buffer from local buffer
		for(i=0; i<tx_length; i++)
		{
			spi_tx11_Buffer.Buffer[i] = p_txdata[i];
		}

		// Enable SPI Tx Interrupt
		USARTn->CR1 |= (1<<5);		// Enable tx interrupt

		// SPI timeout check
		while((spi_tx11_Buffer.State == USART_SPI_TX_BUSY) || (spi_rx11_Buffer.State == USART_SPI_RX_BUSY))
		{
			timekeeper--;
			if(timekeeper == 0)
			{
				HAL_USART_Command(USARTn, DISABLE);
				USARTn->CR1 &= ~(1<<5);		// Disable tx interrupt
				HAL_USART_SPIBuffer_Init(USARTn);
			}
		}

		// load rx_data to local buffer from global buffer
		for(i=0; i<rx_length; i++)
		{
			p_rxdata[i] = spi_rx11_Buffer.Buffer[i+1];
		}
	}
}

/*********************************************************************//**
 * @brief						USART-SPI Handler (Interrupt Handler)
 * @param[in]	USARTn	Pointer to selected SPI peripheral, should be:
 *											- USART10	:USART10 peripheral
 *											- USART11	:USART11 peripheral
 * @return				None
 **********************************************************************/
void HAL_USART_SPI_Handler(USART_Type *USARTn)
{
	uint32_t i;
	uint32_t spi_status;
	
	spi_status = USARTn->ST;

	/* Line Error Check */
	if(spi_status & 0x07)
	{
		USARTn->ST |= 0x07;	// Clear Flags
	}

	/* USART10 Unit */
	if(USARTn == USART10)
	{
		/* Master Mode */
		if(USARTn->CR2 & (1<<7))
		{
			// Transmit Only
			if((spi_tx10_Buffer.DataLength != 0) && (spi_rx10_Buffer.DataLength == 0))
			{
				for(i=0; i<spi_tx10_Buffer.DataLength; i++)
				{
					while(!(USARTn->ST & USART_ST_DRE));
					HAL_USART_Command(USARTn, DISABLE);
					USARTn->DR = spi_tx10_Buffer.Buffer[i];
					HAL_USART_Command(USARTn, ENABLE);
				}
			}
			// Transmit then Receive
			else if((spi_tx10_Buffer.DataLength != 0) && (spi_rx10_Buffer.DataLength != 0))
			{
				for(i=0; i<spi_tx10_Buffer.DataLength; i++)
				{
					while(!(USARTn->ST & USART_ST_DRE));
					HAL_USART_Command(USARTn, DISABLE);
					USARTn->DR = spi_tx10_Buffer.Buffer[i];
					HAL_USART_Command(USARTn, ENABLE);
				}
				for(i=0; i<spi_rx10_Buffer.DataLength; i++)
				{
					while(!(USARTn->ST & USART_ST_DRE));
					USARTn->DR = 0x00;	// Dummy data transmit
					while(!(USARTn->ST & USART_ST_RXC));
					spi_rx10_Buffer.Buffer[i] = (USARTn->DR);
				}
				while(!(USARTn->ST & USART_ST_RXC));
				spi_rx10_Buffer.Buffer[i] = (USARTn->DR);
			}
			// Receive Only
			else
			{
				for(i=0; i<spi_rx10_Buffer.DataLength; i++)
				{
					while(!(USARTn->ST & USART_ST_DRE));
					USARTn->DR = 0x00;	// Dummy data transmit
					while(!(USARTn->ST & USART_ST_RXC));
					spi_rx10_Buffer.Buffer[i] = (USARTn->DR);
				}
			}
			// END TRX (Buffer Clear)
			HAL_USART_SPIBuffer_Init(USARTn);
		}
		/* Slave Mode */
		else
		{
			while(!(USARTn->ST & USART_ST_RXC));
			spi_rx10_Buffer.Buffer[spi_rx10_Buffer.DataLength] = (USARTn->DR);
			spi_rx10_Buffer.DataLength++;

			// User specific code
		}
	}
	/* USART11 Unit */
	else if(USARTn == USART11)
	{
		/* Master Mode */
		if(USARTn->CR2 & (1<<7))
		{
			// Transmit Only
			if((spi_tx11_Buffer.DataLength != 0) && (spi_rx11_Buffer.DataLength == 0))
			{
				for(i=0; i<spi_tx11_Buffer.DataLength; i++)
				{
					while(!(USARTn->ST & USART_ST_DRE));
					HAL_USART_Command(USARTn, DISABLE);
					USARTn->DR = spi_tx11_Buffer.Buffer[i];
					HAL_USART_Command(USARTn, ENABLE);
				}
			}
			// Transmit then Receive
			else if((spi_tx11_Buffer.DataLength != 0) && (spi_rx11_Buffer.DataLength != 0))
			{
				for(i=0; i<spi_tx11_Buffer.DataLength; i++)
				{
					while(!(USARTn->ST & USART_ST_DRE));
					HAL_USART_Command(USARTn, DISABLE);
					USARTn->DR = spi_tx11_Buffer.Buffer[i];
					HAL_USART_Command(USARTn, ENABLE);
				}
				for(i=0; i<spi_rx11_Buffer.DataLength; i++)
				{
					while(!(USARTn->ST & USART_ST_DRE));
					USARTn->DR = 0x00;	// Dummy data transmit
					while(!(USARTn->ST & USART_ST_RXC));
					spi_rx11_Buffer.Buffer[i] = (USARTn->DR);
				}
				while(!(USARTn->ST & USART_ST_RXC));
				spi_rx11_Buffer.Buffer[i] = (USARTn->DR);
			}
			// Receive Only
			else
			{
				for(i=0; i<spi_rx11_Buffer.DataLength; i++)
				{
					while(!(USARTn->ST & USART_ST_DRE));
					USARTn->DR = 0x00;	// Dummy data transmit
					while(!(USARTn->ST & USART_ST_RXC));
					spi_rx11_Buffer.Buffer[i] = (USARTn->DR);
				}
			}
			// END TRX (Buffer Clear)
			HAL_USART_SPIBuffer_Init(USARTn);
		}
		/* Slave Mode */
		else
		{
			while(!(USARTn->ST & USART_ST_RXC));
			spi_rx11_Buffer.Buffer[spi_rx11_Buffer.DataLength] = (USARTn->DR);
			spi_rx11_Buffer.DataLength++;

			// User specific code
		}
	}

	/* Disable Tx interrupt */
	USARTn->CR1 &= ~(1<<5);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/* --------------------------------- End Of File ------------------------------ */
