/**********************************************************************//**
* @file				A31T21x_hal_uart.c
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
#include "A31T21x_hal_uart.h"


/* Private variable -------------------------------------------------------- */
uint8_t u8Dummy;

/* Private Types --------------------------------------------------------------- */
uRingBuffer_Type	tx0_RingBuffer;
uRingBuffer_Type	tx1_RingBuffer;
uRingBuffer_Type	rx0_RingBuffer;
uRingBuffer_Type	rx1_RingBuffer;


/* Public Functions -------------------------------------------------------- */
/**********************************************************************//**
* @brief						Read a single data from UART peripheral (Polling mode)
* @param[in]	UARTn	UART peripheral selected, should be:
*											- UART0 : UART0 peripheral
*											- UART1 : UART1 peripheral
* @return					Received data
 **********************************************************************/
uint8_t HAL_UART_ReadBuffer(UART_Type* UARTn)
{
	u8Dummy = UARTn->RBR;		// Dummy read

	while(!(UARTn->LSR & UART_LSR_RDR));
	return UARTn->RBR;
}

/**********************************************************************//**
 * @brief						Write a single data through UART peripheral (Polling mode)
 * @param[in]	UARTn		UART peripheral selected, should be:
 *											- UART0 : UART0 peripheral
 *											- UART1 : UART1 peripheral
 * @param[in]	tx_data		Data to transmit
 * @return				None
 **********************************************************************/
void HAL_UART_WriteBuffer(UART_Type* UARTn, uint8_t tx_data)
{
	while(!(UARTn->LSR & UART_LSR_THRE));	// Wait for current transmit complete
	UARTn->THR = tx_data;
}

/**********************************************************************//**
 * @brief						Initializes the UARTx peripheral according to the specified
 *											parameters in the UART_ConfigStruct.
 * @param[in]	UARTn			UART peripheral selected, should be:
 *											- UART0 : UART0 peripheral
 *											- UART1 : UART1 peripheral
 * @param[in]	UARTConfigStruct		Pointer to a UART_CFG_Type structure
 *											that contains the configuration information for the
 *											specified UART peripheral.
 *											- tBaudRate / tDataLength / tStopbit / tParity
 * @param[in]	UARTIntStruct		UART Interrupt selected, should be:
 *											- UART_INT_POLLING
 *											- UART_INT_RXE
 *											- UART_INT_THR
 *											- UART_INT_RXE_THR
 *											- UART_INT_RLS
 *											- UART_INT_RXE_RLS
 *											- UART_INT_THR_RLS
 *											- UART_INT_RXE_THR_RLS
 * @return					HAL_Status_Type
 **********************************************************************/
HAL_Status_Type HAL_UART_Init(UART_Type *UARTn, UART_CFG_Type *UARTConfigStruct, UART_INTCFG_Type UARTIntConfig)
{
	uint8_t u8Reg;

	/* UART Block Reset */
	if(UARTn == UART0)
	{
		SCU->PER2 &= ~(1<<12);
		SCU->PCER2 &= ~(1<<12);
		SCU->PER2 |= (1<<12);
		SCU->PCER2 |= (1<<12);		
	}
	else if(UARTn == UART1)
	{
		SCU->PER2 &= ~(1<<13);
		SCU->PCER2 &= ~(1<<13);
		SCU->PER2 |= (1<<13);
		SCU->PCER2 |= (1<<13);
	}
	else
	{
		return HAL_ERROR;
	}	

	/* UART Register Initialize */
	UARTn->IER = 0;
	UARTn->LCR = 0;
	UARTn->DCR = 0;
	u8Dummy = UARTn->LSR;
	u8Dummy = UARTn->IIR;

	/* UART RingBuffer Initialize */
	HAL_UART_RingBuffer_Init(UARTn);
	
	/* Set BaudRate */
	HAL_UART_BaudrateSet(UARTn, (UARTConfigStruct->tBaudRate));
	
	/* Set DataLength */
	u8Reg = 0;
	switch(UARTConfigStruct->tDataLength)
	{
		case UART_DATALEN_5 :
			u8Reg |= UART_LCR_DLEN_5;
			break;
		case UART_DATALEN_6 :
			u8Reg |= UART_LCR_DLEN_6;
			break;
		case UART_DATALEN_7 :
			u8Reg |= UART_LCR_DLEN_7;
			break;
		case UART_DATALEN_8 :
			u8Reg |= UART_LCR_DLEN_8;
			break;
		default :
			u8Reg |= UART_LCR_DLEN_8;
			break;
	}

	/* Set StopBit */
	switch(UARTConfigStruct->tStopbit)
	{
		case UART_STOPBIT_2 :
			u8Reg |= UART_LCR_STOPBIT;
			break;
		case UART_STOPBIT_1 :
		default:
			u8Reg &= ~UART_LCR_STOPBIT;
			break;
	}

	/* Set Parity */
	if(UARTConfigStruct->tParity == UART_PARITY_NONE)
	{
		u8Reg &= ~(0x38);
	}
	else
	{
		u8Reg |= UART_LCR_PEN;
		switch(UARTConfigStruct->tParity)
		{
			case UART_PARITY_ODD :
				u8Reg &= ~UART_LCR_PARITY;
				break;

			case UART_PARITY_EVEN :
				u8Reg |= UART_LCR_PARITY;
				break;

			case UART_PARITY_SP_1 :
				u8Reg |= UART_LCR_STICKP;
				break;

			case UART_PARITY_SP_0 :
				u8Reg &= ~UART_LCR_STICKP;
				break;

			default :
				u8Reg &= ~(0x38);
				break;
		}
	}
	UARTn->LCR = (uint8_t)(u8Reg & UART_LCR_BITMASK);		// Set Line Control Register (DataLen/StopBit/Parity)

	/* UART Interrupt Setting */
	u8Reg = 0;
	switch(UARTIntConfig)
	{
		case UART_INT_POLLING:
			u8Reg = 0;
			break;
		case UART_INT_RXE:
			u8Reg = UART_IER_DRIE;
			break;
		case UART_INT_THRE:
			u8Reg = UART_IER_THREIE;
			break;
		case UART_INT_RXE_THRE:
			u8Reg = UART_IER_DRIE | UART_IER_THREIE;
			break;
		case UART_INT_RLS:
			u8Reg = UART_IER_RLSIE;
			break;
		case UART_INT_RXE_RLS:
			u8Reg = UART_IER_DRIE | UART_IER_RLSIE;
			break;
		case UART_INT_THRE_RLS:
			u8Reg = UART_IER_THREIE | UART_IER_RLSIE;
			break;
		case UART_INT_RXE_THRE_RLS:
			u8Reg = UART_IER_DRIE | UART_IER_THREIE | UART_IER_RLSIE;
			break;
		default :
			u8Reg = 0;
			break;
	}
	UARTn->IER = (uint8_t)(u8Reg & UART_IER_BITMASK);		// Set interrupt enable register

	return HAL_OK;
}

/*********************************************************************//**
 * @brief						Determines best dividers to get a target clock rate
 * @param[in]	UARTn	Pointer to selected UART peripheral, should be:
 *											- UART0	:UART0 peripheral
 *											- UART1	:UART1 peripheral
 * @param[in]	baudrate Desired UART baud rate.
 * @return				None
 **********************************************************************/
void HAL_UART_BaudrateSet(UART_Type *UARTn, uint32_t baudrate)
{
	uint32_t numerator;
	uint32_t denominator; 
	uint32_t bdr, bfr;
	uint32_t fd;

	//------------------------------------------------------------------------------
	// numerator & denominator 
	// 
	// bdr = (UartBaseClock) / (16 * baudrate) 
	//------------------------------------------------------------------------------
	numerator = SystemPeriClock/2;
	denominator = 16 * baudrate; 

	bdr = numerator / denominator;

	//------------------------------------------------------------------------------
	//	fraction = numerator - bdr x denominator
	//
	//	bfr = INT (fraction * 256) = ((f x d ) << 8) / d 
	//------------------------------------------------------------------------------

	fd = numerator - (bdr * denominator); 
	bfr = (fd * 256) / denominator; 
	UARTn->BDR= (uint16_t)(bdr&0xffff);
	UARTn->BFR= (uint8_t)(bfr&0xff);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


/*********************************************************************//**
 * @brief						Initialize RingBuffer Parameters
 * @param[in]	UARTn	Pointer to selected UART peripheral, should be:
 *											- UART0	:UART0 peripheral
 *											- UART1	:UART1 peripheral
 * @return				None
 **********************************************************************/
void HAL_UART_RingBuffer_Init(UART_Type *UARTn)
{
	if(UARTn == UART0)
	{
		tx0_RingBuffer.HeadPtr = 0;
		tx0_RingBuffer.TailPtr = 0;
		tx0_RingBuffer.State = UART_TX_IDLE;
		
		rx0_RingBuffer.HeadPtr = 0;
		rx0_RingBuffer.TailPtr = 0;
		rx0_RingBuffer.State = UART_RX_IDLE;
	}
	else if(UARTn == UART1)
	{
		tx1_RingBuffer.HeadPtr = 0;
		tx1_RingBuffer.TailPtr = 0;
		tx1_RingBuffer.State = UART_TX_IDLE;

		rx1_RingBuffer.HeadPtr = 0;
		rx1_RingBuffer.TailPtr = 0;
		rx1_RingBuffer.State = UART_RX_IDLE;
	}
}

/*********************************************************************//**
 * @brief						UART Transmit Data (Interrupt mode)
 * @param[in]	UARTn	Pointer to selected UART peripheral, should be:
 *											- UART0	:UART0 peripheral
 *											- UART1	:UART1 peripheral
 * @param[in]	tx_data		UART Transmit Data
 * @return				None
 **********************************************************************/
void HAL_UART_TransmitData(UART_Type *UARTn, uint8_t tx_data)
{
	UARTn->IER &= ~(UART_IER_THREIE);		// Disable thre interrupt

	/* UART0 Unit */
	if(UARTn == UART0)
	{
		if(tx0_RingBuffer.HeadPtr == tx0_RingBuffer.TailPtr)
		{
			if(tx0_RingBuffer.State == UART_TX_IDLE)
			{
				HAL_UART_WriteBuffer(UARTn, tx_data);		// First data to transmit
				tx0_RingBuffer.State = UART_TX_BUSY;
			}
			else
			{
				tx0_RingBuffer.Buffer[tx0_RingBuffer.HeadPtr++] = tx_data;
				// Check buffer max length
				if(tx0_RingBuffer.HeadPtr > uRING_BUFFER_LENGTH)
				{
					tx0_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			tx0_RingBuffer.Buffer[tx0_RingBuffer.HeadPtr++] = tx_data;
			// Check buffer max length
			if(tx0_RingBuffer.HeadPtr > uRING_BUFFER_LENGTH)
			{
				tx0_RingBuffer.HeadPtr = 0;
			}
		}
	}
	/* UART1 Unit */
	else if(UARTn == UART1)
	{
		if(tx1_RingBuffer.HeadPtr == tx1_RingBuffer.TailPtr)
		{
			if(tx1_RingBuffer.State == UART_TX_IDLE)
			{
				HAL_UART_WriteBuffer(UARTn, tx_data);		// First data to transmit
				tx1_RingBuffer.State = UART_TX_BUSY;
			}
			else
			{
				tx1_RingBuffer.Buffer[tx1_RingBuffer.HeadPtr++] = tx_data;
				// Check buffer max length
				if(tx1_RingBuffer.HeadPtr > uRING_BUFFER_LENGTH)
				{
					tx1_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			tx1_RingBuffer.Buffer[tx1_RingBuffer.HeadPtr++] = tx_data;
			// Check buffer max length
			if(tx1_RingBuffer.HeadPtr > uRING_BUFFER_LENGTH)
			{
				tx1_RingBuffer.HeadPtr = 0;
			}
		}
	}

	UARTn->IER |= (UART_IER_THREIE);		// Enable thre interrupt
}

/*********************************************************************//**
 * @brief						UART Receive Data (Interrupt mode)
 * @param[in]	UARTn	Pointer to selected UART peripheral, should be:
 *											- UART0	:UART0 peripheral
 *											- UART1	:UART1 peripheral
 * @return				Received data
 **********************************************************************/
int8_t HAL_UART_ReceiveData(UART_Type *UARTn)
{
	/* UART0 Unit */
	if(UARTn == UART0)
	{
		if(rx0_RingBuffer.HeadPtr != rx0_RingBuffer.TailPtr)
		{
			// Check buffer max length
			if(rx0_RingBuffer.TailPtr > uRING_BUFFER_LENGTH)
			{
				rx0_RingBuffer.TailPtr = 0;
				return rx0_RingBuffer.Buffer[rx0_RingBuffer.TailPtr];
			}
			return rx0_RingBuffer.Buffer[rx0_RingBuffer.TailPtr++];
		}
		else
		{
				// No received data
				rx0_RingBuffer.State = UART_RX_IDLE;
				return -1;
		}
	}
	/* UART1 Unit */
	else if(UARTn == UART1)
	{
		if(rx1_RingBuffer.HeadPtr != rx1_RingBuffer.TailPtr)
		{
			// Check buffer max length
			if(rx1_RingBuffer.TailPtr > uRING_BUFFER_LENGTH)
			{
				rx1_RingBuffer.TailPtr = 0;
				return rx1_RingBuffer.Buffer[rx1_RingBuffer.TailPtr];
			}
			return rx1_RingBuffer.Buffer[rx1_RingBuffer.TailPtr++];
		}
		else
		{
				// No received data
				rx1_RingBuffer.State = UART_RX_IDLE;
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
 * @brief						UART Handler (Interrupt Handler)
 * @param[in]	UARTn	Pointer to selected UART peripheral, should be:
 *											- UART0	:UART0 peripheral
 *											- UART1	:UART1 peripheral
 * @return				None
 **********************************************************************/
void HAL_UART_Handler(UART_Type *UARTn)
{
	volatile uint32_t int_status;
	volatile uint32_t line_error;
	
	int_status = UARTn->IIR;

	/* Line Interrupt */
	if((int_status & UART_IIR_BITMASK) == UART_IIR_IID_RLS)
	{
		line_error = UARTn->LSR;							// Clear line Error Interrupt
	}
	
	/* Rx Interrupt */
	if((int_status & UART_IIR_BITMASK) == UART_IIR_IID_RDA)
	{
		HAL_UART_RX_Handler(UARTn);		// Rx Process
	}
	
	/* Tx Interrupt */
	if((int_status & UART_IIR_BITMASK) == UART_IIR_IID_THRE)
	{
		HAL_UART_TX_Handler(UARTn);		// Tx Process
	}
}

/*********************************************************************//**
 * @brief						UART Transmit Handler (Interrupt Handler)
 * @param[in]	UARTn	Pointer to selected UART peripheral, should be:
 *											- UART0	:UART0 peripheral
 *											- UART1	:UART1 peripheral
 * @return				None
 **********************************************************************/
void HAL_UART_TX_Handler(UART_Type *UARTn)
{
	/* UART0 Unit */
	if(UARTn == UART0)
	{
		if(tx0_RingBuffer.HeadPtr != tx0_RingBuffer.TailPtr)
		{
			if(UARTn->LSR & UART_LSR_THRE)
			{
				UARTn->THR = tx0_RingBuffer.Buffer[tx0_RingBuffer.TailPtr++];
				// Check buffer max length
				if(tx0_RingBuffer.TailPtr > uRING_BUFFER_LENGTH)
				{
					tx0_RingBuffer.TailPtr = 0;
				}
			}
		}
		else
		{
				tx0_RingBuffer.State = UART_TX_IDLE;
		}
	}
	/* UART1 Unit */
	else if(UARTn == UART1)
	{
		if(tx1_RingBuffer.HeadPtr != tx1_RingBuffer.TailPtr)
		{
			if(UARTn->LSR & UART_LSR_THRE)
			{
				UARTn->THR = tx1_RingBuffer.Buffer[tx1_RingBuffer.TailPtr++];
				// Check buffer max length
				if(tx1_RingBuffer.TailPtr > uRING_BUFFER_LENGTH)
				{
					tx1_RingBuffer.TailPtr = 0;
				}
			}
		}
		else
		{
				tx1_RingBuffer.State = UART_TX_IDLE;
		}
	}
}

/*********************************************************************//**
 * @brief						UART Receive Handler (Interrupt Handler)
 * @param[in]	UARTn	Pointer to selected UART peripheral, should be:
 *											- UART0	:UART0 peripheral
 *											- UART1	:UART1 peripheral
 * @return				None
 **********************************************************************/
void HAL_UART_RX_Handler(UART_Type *UARTn)
{
	/* UART0 Unit */
	if(UARTn == UART0)
	{
		if(rx0_RingBuffer.HeadPtr == rx0_RingBuffer.TailPtr)
		{
			if(rx0_RingBuffer.State == UART_RX_IDLE)
			{
				rx0_RingBuffer.Buffer[rx0_RingBuffer.HeadPtr++] = UARTn->RBR;		// First data to receive
				rx0_RingBuffer.State = UART_RX_BUSY;
				// Check buffer max length
				if(rx0_RingBuffer.HeadPtr > uRING_BUFFER_LENGTH)
				{
					rx0_RingBuffer.HeadPtr = 0;
				}
			}
			else
			{
				rx0_RingBuffer.Buffer[rx0_RingBuffer.HeadPtr++] = UARTn->RBR;
				// Check buffer max length
				if(rx0_RingBuffer.HeadPtr > uRING_BUFFER_LENGTH)
				{
					rx0_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			rx0_RingBuffer.Buffer[rx0_RingBuffer.HeadPtr++] = UARTn->RBR;
			// Check buffer max length
			if(rx0_RingBuffer.HeadPtr > uRING_BUFFER_LENGTH)
			{
				rx0_RingBuffer.HeadPtr = 0;
			}
		}
	}
	/* UART1 Unit */
	else if(UARTn == UART1)
	{
		if(rx1_RingBuffer.HeadPtr == rx1_RingBuffer.TailPtr)
		{
			if(rx1_RingBuffer.State == UART_RX_IDLE)
			{
				rx1_RingBuffer.Buffer[rx1_RingBuffer.HeadPtr++] = UARTn->RBR;		// First data to receive
				rx1_RingBuffer.State = UART_RX_BUSY;
				// Check buffer max length
				if(rx1_RingBuffer.HeadPtr > uRING_BUFFER_LENGTH)
				{
					rx1_RingBuffer.HeadPtr = 0;
				}
			}
			else
			{
				rx1_RingBuffer.Buffer[rx1_RingBuffer.HeadPtr++] = UARTn->RBR;
				// Check buffer max length
				if(rx1_RingBuffer.HeadPtr > uRING_BUFFER_LENGTH)
				{
					rx1_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			rx1_RingBuffer.Buffer[rx1_RingBuffer.HeadPtr++] = UARTn->RBR;
			// Check buffer max length
			if(rx1_RingBuffer.HeadPtr > uRING_BUFFER_LENGTH)
			{
				rx1_RingBuffer.HeadPtr = 0;
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
void aputc(UART_Type *UARTn, uint8_t data)
{
	HAL_UART_WriteBuffer(UARTn, data);
}

/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
void aputs(UART_Type *UARTn, uint8_t *p_data)
{
	while(1)
	{
		aputc(UARTn, *p_data);
		if(*p_data == 0){break;}
		else{p_data++;}
	}
}

/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
void aputhex(UART_Type *UARTn, uint32_t data)
{
	uint8_t i;
	uint8_t tx_data;
	uint32_t nibble;

	if(data > 0xFFFFFFF)
	{
		i = 32;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
			if(i==0){break;}
		}
	}
	else if(data > 0xFFFFFF)
	{
		i = 28;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
			if(i==0){break;}
		}
	}
	else if(data > 0xFFFFF)
	{
		i = 24;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
			if(i==0){break;}
		}
	}
	else if(data > 0xFFFF)
	{
		i = 20;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
			if(i==0){break;}
		}
	}
	else if(data > 0xFFF)
	{
		i = 16;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
			if(i==0){break;}
		}
	}
	else if(data > 0xFF)
	{
		i = 12;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
			if(i==0){break;}
		}
	}
	else if(data > 0xF)
	{
		i = 8;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
			if(i==0){break;}
		}
	}
	else
	{
		i = 4;
		while(1)
		{
			i-=4;
			nibble = (data>>i) & 0x0F;
			tx_data = (nibble>9) ? ('A' + nibble - 10) : ('0' + nibble);
			aputc(UARTn, tx_data);
			if(i==0){break;}
		}
	}
}

/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
uint8_t agetc(UART_Type *UARTn)
{
	return HAL_UART_ReadBuffer(UARTn);
}

/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
uint8_t agets(UART_Type *UARTn, uint8_t *p_data)
{
	uint8_t receive_count = 0;

	while(1)
	{
		*p_data = agetc(UARTn);
		if(*p_data == 0x0D)
		{
			break;
		}
		else
		{
			p_data++;
			receive_count++;
		}
	}

	return receive_count;
}

/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
uint8_t agethex(UART_Type *UARTn, uint8_t *p_data)
{
	uint8_t i;
	uint8_t receive_count;
	
	receive_count = agets(UARTn, p_data);
	
	for(i=0; i<receive_count; i++)
	{
		switch(p_data[i])
		{
			case 0x30:
				p_data[i] = 0x0;
				break;
			case 0x31:
				p_data[i] = 0x1;
				break;
			case 0x32:
				p_data[i] = 0x2;
				break;
			case 0x33:
				p_data[i] = 0x3;
				break;
			case 0x34:
				p_data[i] = 0x4;
				break;
			case 0x35:
				p_data[i] = 0x5;
				break;
			case 0x36:
				p_data[i] = 0x6;
				break;
			case 0x37:
				p_data[i] = 0x7;
				break;
			case 0x38:
				p_data[i] = 0x8;
				break;
			case 0x39:
				p_data[i] = 0x9;
				break;
			case 0x61:
				p_data[i] = 0xa;
				break;
			case 0x62:
				p_data[i] = 0xb;
				break;
			case 0x63:
				p_data[i] = 0xc;
				break;
			case 0x64:
				p_data[i] = 0xd;
				break;
			case 0x65:
				p_data[i] = 0xe;
				break;
			case 0x66:
				p_data[i] = 0xf;
				break;
			default :
				p_data[i] = 0;				
				break;
		}
	}
	
	return receive_count;
}

/*********************************************************************//**
 * @brief
 * @param[in]
 * @return
 **********************************************************************/
void RegisterWrite_Handler(UART_Type *UARTn)
{
	uint32_t addr, val;
	uint32_t *p_Register;
	uint8_t data[9];
	uint8_t i, data_cnt;

	aputs(UARTn,"\n\r#########################");
	aputs(UARTn,"\n\r## Register Write/Read ##");
	aputs(UARTn,"\n\r#########################\n\r");
	
	while(1)
	{
		addr = 0; val = 0;
		
		/* Input Target Address */
		aputs(UARTn,"\n\r");		
		aputs(UARTn,">>TargetAddr : 0x");		
		data_cnt = agethex(UARTn, data);
		for(i=0; i<data_cnt; i++)
		{
			addr = (addr<<4);
			addr |= data[i];
		}
		aputhex(UARTn, addr);
		aputs(UARTn," => OK\n\r");
		
		/* Input Value */
		aputs(UARTn,">>WriteValue : 0x");		
		data_cnt = agethex(UARTn, data);
		for(i=0; i<data_cnt; i++)
		{
			val = (val<<4);
			val |= data[i];
		}
		aputhex(UARTn, val);
		aputs(UARTn," => OK\n\r");		

		/* Register Write/Read */
		p_Register = (uint32_t *)addr;
		if(data_cnt >0)
		{
			*p_Register = val;
		}
		aputs(UARTn,"[0x");
		aputhex(UARTn,addr);
		aputs(UARTn,"] = 0x");		
		aputhex(UARTn, *p_Register);
		aputs(UARTn," [OK]\n\r");
	}
}


/* --------------------------------- End Of File ------------------------------ */
