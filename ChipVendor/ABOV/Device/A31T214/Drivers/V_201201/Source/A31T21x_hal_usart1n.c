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
/* Private Types --------------------------------------------------------------- */
USART_BUFFER_T USARTBuff10;	// For USART10 Interrupt
USART_BUFFER_T USARTBuff11;	// For USART11 Interrupt
/* Public Functions -------------------------------------------------------- */
/**********************************************************************//**
 * @brief						Initializes the USARTn peripheral according to the specified
 *											parameters in the USART_ConfigStruct.
 * @param[in]	USARTn			USART peripheral selected, should be:
 *											- USART10 : USART10 peripheral
 *											- USART11 : USART11 peripheral
 * @param[in]	USARTConfigStruct		Pointer to a USART_CFG_Type structure
 *											that contains the configuration information for the
 *											specified USART peripheral.
 *											- tBaudRate / tDataLength / tStopbit / tParity / tOpmode
 * @return				HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_USART_Init(USART_Type *USARTn, USART_CFG_Type *USARTConfigStruct)
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

	///////////////////////////////////////////////////
	/* UART/USART MODE */
	if((USARTConfigStruct->tOpMode == USART_UART_MODE) || (USARTConfigStruct->tOpMode == USART_USART_MODE))
	{
		/* Set BaudRate */
		if(USARTConfigStruct->tOpMode == USART_UART_MODE)
		{
			HAL_USART_BaudrateSet(USARTn, USART_UART_MODE, (USARTConfigStruct->tBaudRate));
		}
		else
		{
			HAL_USART_BaudrateSet(USARTn, USART_USART_MODE, (USARTConfigStruct->tBaudRate));
		}

		/* Set Mode */
		if(USARTConfigStruct->tOpMode == USART_UART_MODE)
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

		///////////////////////////////////////////////////
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
	else if(USARTConfigStruct->tOpMode == USART_SPI_MODE)
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
	}

	else
	{
		return HAL_ERROR;
	}

	return HAL_OK;
}

/*********************************************************************//**
 * @brief						Deinitializes the USARTx peripheral registers
 * @param[in]	USARTn			USART peripheral selected, should be:
 *											- USART10 : USART10 peripheral
 *											- USART11 : USART11 peripheral
 * @return				HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_USART_DeInit(USART_Type* USARTn)
{
	if(USARTn == USART10)
	{
		SCU->PER2 &= ~(1<<8);
		SCU->PCER2 &= ~(1<<8);
	}
	else if(USARTn == USART11)
	{
		SCU->PER2 &= ~(1<<9);
		SCU->PCER2 &= ~(1<<9);
	}
	else
	{
		return HAL_ERROR;
	}
	
	return HAL_OK;
}

/*********************************************************************//**
 * @brief						Enable or disable specified USART interrupt
 * @param[in]	USARTn			USART peripheral selected, should be:
 *											- USART10 : USART10 peripheral
 *											- USART11 : USART11 peripheral
 * @param[in]	USART_INTCFG_Type		USART Interrupt selected, should be:
 *											- UART_INT_POLLING
 *											- USART_INT_WAKEIE
 *											- USART_INT_RXCIE
 *											- USART_INT_TXCIE
 *											- USART_INT_RXCIE_TXCIE
 *											- USART_INT_DRIE
 * @param[in]	EN_DIS_Type int_en
 *											- ENABLE: wdt interrupt enable
 *											- DISABLE: wdt interrupt disable 
 * @return				HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_USART_ConfigInterrupt(USART_Type *USARTn, USART_INTCFG_Type USARTIntConfig, EN_DIS_Type int_en)
{
	uint32_t u32Reg;

	u32Reg = 0;
	switch(USARTIntConfig)
	{
		case USART_INT_POLLING:
			u32Reg = 0;
			break;
		case USART_INT_WAKEIE:
			u32Reg |= (1<<2);
			break;
		case USART_INT_RXCIE:
			u32Reg |= (1<<3);
			break;
		case USART_INT_TXCIE:							// If needs, Enable TXIE at Transmit Function
			u32Reg |= (1<<4);
			break;
		case USART_INT_RXCIE_TXCIE:		// If needs, Enable TXIE at Transmit Function
			u32Reg |= (1<<3) | (1<<4);
//			u32Reg |= (1<<3);
			break;
		case USART_INT_DRIE:
			u32Reg |= (1<<5);
			break;
		default :
			u32Reg = 0;
			break;
	}

	if(int_en == ENABLE)
	{
		USARTn->CR1 |= u32Reg;				// Set interrupt
	}
	else
	{
		USARTn->CR1 &= ~(u32Reg);	// Clear interrupt
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
void HAL_USART_BaudrateSet(USART_Type *USARTn, USART_OPMODE_Type USARTOPModeConfig, uint32_t baudrate)
{
	uint32_t numerator;
	uint32_t denominator; 
	uint32_t bdr;

	if(USARTOPModeConfig == USART_UART_MODE)
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
	else if((USARTOPModeConfig == USART_SPI_MODE) || (USARTOPModeConfig == USART_USART_MODE))
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
 * @brief						Write a single data through USART peripheral (Polling mode)
 * @param[in]	USARTn		USART peripheral selected, should be:
 *											- USART0 : USART10 peripheral
 *											- USART1 : USART11 peripheral
 * @param[in]	tx_data		Data to transmit
 * @return				None
 **********************************************************************/
void HAL_USART_TransmitByte(USART_Type* USARTn, uint8_t tx_data)
{
	USARTn->DR = tx_data;
}

/**********************************************************************//**
* @brief						Read a single data from USART peripheral (Polling mode)
* @param[in]	USARTn	USART peripheral selected, should be:
*											- USART10 : USART10 peripheral
*											- USART11 : USART11 peripheral
* @return					Received data
 **********************************************************************/
uint8_t HAL_USART_ReceiveByte(USART_Type* USARTn)
{
	return USARTn->DR;
}

/**********************************************************************//**
* @brief						Send a block of data via USART peripheral
* @param[in]	USARTn	USART peripheral selected, should be:
*											- USART10 : USART10 peripheral
*											- USART11 : USART11 peripheral
 * @param[in]	txbuf 	Pointer to Transmit buffer
 * @param[in]	buflen 	Length of Transmit buffer
 * @return 				Number of bytes sent.
 **********************************************************************/
uint32_t HAL_USART_Transmit(USART_Type *USARTn, uint8_t *txbuf, uint32_t buflen)
{
	uint32_t bToSend, bSent;
	uint8_t *pChar = txbuf;

	bToSend = buflen;
	bSent = 0;

	while(bToSend)
	{
		while(!(USARTn->ST & USART_ST_DRE));
		HAL_USART_TransmitByte(USARTn, (*pChar++));

		bToSend--;
		bSent++;
	}

	return bSent;
}

/**********************************************************************//**
* @brief						Receive a block of data via USART peripheral
* @param[in]	USARTn	USART peripheral selected, should be:
*											- USART10 : USART10 peripheral
*											- USART11 : USART11 peripheral
* @param[out] rxbuf 	Pointer to Received buffer
* @param[in]	buflen 	Length of Received buffer
 * @return 				Number of bytes sent.
 **********************************************************************/
uint32_t HAL_USART_Receive(USART_Type *USARTn, uint8_t *rxbuf, uint32_t buflen)
{
	uint32_t bToRecv, bRecv;
	uint8_t *pChar = rxbuf;

	bToRecv = buflen;
	bRecv = 0;

	while(bToRecv)
	{
		while(!(USARTn->ST & USART_ST_RXC));
		(*pChar++) = HAL_USART_ReceiveByte(USARTn);

		bRecv++;
		bToRecv--;
	}

	return bRecv;
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


////////////////////////////////////////////////////////////////////////////////////////////////////
// USART Interrupt API
////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************************
 * @brief						USARTSend
 * @param[in]	None
 * @return				None
 **********************************************************************/
uint32_t USARTSend(USART_Type *USARTn, uint8_t *txbuf, uint8_t buflen)
{
	uint8_t *tx_flag;
	uint32_t i, timeout;
	uint32_t tx_cnt = 0;

	if(USARTn == USART10)
	{
		tx_flag = &USARTBuff10.tx_flag;
	}
	else if(USARTn == USART11)
	{
		tx_flag = &USARTBuff11.tx_flag;
	}

	for(i=0; i<buflen; i++)
	{
		timeout = 10000;
		HAL_USART_TransmitByte(USARTn, (txbuf[i]));
		tx_cnt++;

		while(timeout--)
		{
			if(*tx_flag)
			{
				break;
			}
		}
		*tx_flag = 0;
	}
	
	return tx_cnt;
}

/**********************************************************************
 * @brief						USARTReceive
 * @param[in]	None
 * @return				None
 **********************************************************************/
uint32_t USARTReceive(USART_Type *USARTn, uint8_t *rxbuf, uint8_t buflen)
{
	uint8_t *buf, *rx_flag;
	uint32_t i, *head, *tail, tmp;
	uint32_t rx_cnt = 0;

	if(USARTn == USART10)
	{
		buf = USARTBuff10.rx;
		head = &USARTBuff10.buffer_head;
		tail = &USARTBuff10.buffer_tail;
		rx_flag = &USARTBuff10.rx_flag;
	}
	else if(USARTn == USART11)
	{
		buf = USARTBuff11.rx;
		head = &USARTBuff11.buffer_head;
		tail = &USARTBuff11.buffer_tail;
		rx_flag = &USARTBuff11.rx_flag;
	}

	while(*rx_flag != 1);
	*rx_flag = 0;

	if(*head > *tail)
	{
		tmp = *tail + (USART_BUFSIZE);
	}
	else
	{
		tmp = *tail;
	}

	for(i=0; (i<buflen && *head<tmp); i++, (*head)++)
	{
		rxbuf[i] = buf[*head];
		rx_cnt--;
	}
	
	return rx_cnt;
}

/*********************************************************************
 * @brief						USART interrupt handler sub-routine
 * @param[in]	USARTn	USART peripheral selected, should be:
 *											- USARTn	:	USART10~11 peripheral
 * @return				None
 **********************************************************************/
void USART_IRQHandler_IT(USART_Type *USARTn)
{
	volatile uint32_t int_status;

	int_status = (USARTn->ST & USART_ST_BITMASK);
	
	/* Receive Data Available or Character time-out */
	if((int_status & USART_ST_RXC) == USART_ST_RXC)
	{
		if(USARTn == USART10)
		{
			USARTBuff10.rx[USARTBuff10.buffer_tail++] = HAL_USART_ReceiveByte(USART10);
			USARTBuff10.rx_flag = 1;
		}
		else if(USARTn == USART11)
		{
			USARTBuff11.rx[USARTBuff11.buffer_tail++] = HAL_USART_ReceiveByte(USART11);
			USARTBuff11.rx_flag = 1;
		}
	}

	/* Transmit Holding Empty */
	if((int_status & USART_ST_TXC) == USART_ST_TXC)
	{
		USARTn->ST = USART_ST_TXC;
		
		if(USARTn == USART10)
		{
			USARTBuff10.tx_flag = 1;
		}
		else if(USARTn == USART11)
		{
			USARTBuff11.tx_flag = 1;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/* --------------------------------- End Of File ------------------------------ */
