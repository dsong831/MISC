/**********************************************************************//**
* @file				A31T21x_hal_spi.c
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
#include "A31T21x_hal_spi.h"


/* Private variable -------------------------------------------------------- */

/* Private Types --------------------------------------------------------------- */
sRingBuffer_Type	spi_tx20_RingBuffer;
sRingBuffer_Type	spi_tx21_RingBuffer;
sRingBuffer_Type	spi_rx20_RingBuffer;
sRingBuffer_Type	spi_rx21_RingBuffer;


/* Public Functions ------------------------------------------------------- */
/**********************************************************************//**
 * @brief						Transmit a single data through SPIn peripheral
 * @param[in]	SPIn	SPI peripheral selected, should be:
 *											- SP	:SPI20~21 peripheral
 * @param[in]	tx_data		Data to transmit
 * @return				None
 **********************************************************************/
void HAL_SPI_WriteBuffer(SPI_Type* SPIn, uint32_t tx_data)
{
	while(!(SPIn->SR & SPI_SR_TRDY));	// Wait until transmit buffer is ready for use.
	SPIn->RDR_TDR = tx_data;
}

/**********************************************************************//**
 * @brief						Receive a single data from SPIx peripheral
 * @param[in]	SPIn	SPI peripheral selected, should be
 *											- SP	:SPI20~21 peripheral
 * @return				Received data
 **********************************************************************/
uint32_t HAL_SPI_ReadBuffer(SPI_Type* SPIn)
{
	while(!(SPIn->SR & SPI_SR_RRDY));	// Wait until receive buffer holds data.
	return ((uint32_t) (SPIn->RDR_TDR));
}

/*********************************************************************//**
 * @brief						Initializes the SPIn peripheral according to the specified
 *											parameters in the SPI_ConfigStruct.
 * @param[in]	SPIn SPI peripheral selected, should be:
 *											- SP	:SPI20~21 peripheral
 * @param[in]	SPIConfigStruct	Pointer to a SPI_CFG_Type structure that
 *											contains the configuration information for the specified
 *											SPI peripheral.
 *											- tBITSZ / tCPOL / tCPHA / tMSBF / tMS / tBaudrate
 * @param[in]	SPIIntConfig		SPI Interrupt selected, should be:
 *											- SPI_INT_POLLING
 *											- SPI_INT_RXIE
 *											- SPI_INT_TXIE
 *											- SPI_INT_RXIE_TXIE
 *											- SPI_INT_SSCIE
 *											- SPI_INT_RXIE_SSCIE
 *											- SPI_INT_TXIE_SSCIE
 *											- SPI_INT_RXIE_TXIE_SSCIE
 * @return				HAL_Satus_Type
 *********************************************************************/
HAL_Status_Type HAL_SPI_Init(SPI_Type *SPIn, SPI_CFG_Type *SPIConfigStruct, SPI_INTCFG_Type SPIIntConfig)
{
	uint32_t u32Reg;

	/* SPI Block Reset */
	if(SPIn == SPI20)
	{
		SCU->PER2 &= ~(1<<14);
		SCU->PCER2 &= ~(1<14);
		SCU->PER2 |= (1<<14);
		SCU->PCER2 |= (1<<14);
	}
	else if(SPIn == SPI21)
	{
		SCU->PER2 &= ~(1<<15);
		SCU->PCER2 &= ~(1<<15);
		SCU->PER2 |= (1<<15);
		SCU->PCER2 |= (1<<15);
	}
	else
	{
		return HAL_ERROR;
	}

	/* SPI RingBuffer Initialize */
	HAL_SPI_RingBuffer_Init(SPIn);


	/* Disable SPI */
	SPIn->EN = 0x00;
	u32Reg = 0;


	/* Config SPI Function */
	// Bit Size Config
	switch(SPIConfigStruct->tBitsize)
	{
		case SPI_CR_BITSZ_8 :
			u32Reg |= SPI_CR_BITSZ_8;
			break;
		case SPI_CR_BITSZ_9 :
			u32Reg |= SPI_CR_BITSZ_9;
			break;
		case SPI_CR_BITSZ_16 :
			u32Reg |= SPI_CR_BITSZ_16;
			break;
		case SPI_CR_BITSZ_17 :
			u32Reg |= SPI_CR_BITSZ_17;
			break;
		default :
			u32Reg |= SPI_CR_BITSZ_8;
			break;
	}
	// CPOL Config
	if(SPIConfigStruct->tCPOL == SPI_CPOL_ActiveLow)
	{
		u32Reg |= SPI_CR_CPOL;
	}
	else
	{
		u32Reg &= ~(SPI_CR_CPOL);
	}
	// CPHA Config
	if(SPIConfigStruct->tCPHA == SPI_CPHA_EVEN)
	{
		u32Reg |= SPI_CR_CPHA;
	}
	else
	{
		u32Reg &= ~(SPI_CR_CPHA);
	}
	// MSBF Config
	if(SPIConfigStruct->tMSBF == SPI_MSBF_MSB)
	{
		u32Reg |= SPI_CR_MSBF;
	}
	else
	{
		u32Reg &= ~(SPI_CR_MSBF);
	}
	// MS Config
	if(SPIConfigStruct->tMS == SPI_MS_MASTER)
	{
		u32Reg |= SPI_CR_MS;
	}
	else
	{
		u32Reg &= ~(SPI_CR_MS);
	}

	SPIn->CR = (3<<19)		// TRX Buffer Clear
		| (0<<13)									// SSMOD		(0: SS auto mode / 1: SS manual mode)
		| (0<<12)									// SSOUT			(0: SS out low / 1: SS out high)
		| (0<<11)									// LBE					(0: Disable loop-back mode / 1: Enable loop-back mode)
		| (0<<10)									// SSMASK	(0: Disable SS Mask / 1: Enable SS Mask)
		| (1<<9)										// SSMO			(0: Disable SS signal / 1: Enable SS signal)
		| (0<<8)										// SSPOL			(0: SS Active Low / 1: SS Active High)
		| u32Reg
		;


	/* Config Baudrate */
	if(SPIConfigStruct->tMS == SPI_MS_MASTER)
	{
		SPIn->BR = ((SystemPeriClock / SPIConfigStruct->tBaudrate) - 1) & 0xFFFF;		// Baudrate = PCLK/(BR+1)
	}
	else
	{
		// if using slave, BR has fase clock set and LR has minimum set.
		SPIn->BR = 2;	// Baudrate = PCLK/(BR+1)
		SPIn->LR = 0x00010101UL;
	}


	/* Config Interrupt */
	u32Reg = 0;
	switch(SPIIntConfig)
	{
		case SPI_INT_POLLING :
			u32Reg = 0;
			break;
		case SPI_INT_RXIE :
			u32Reg = SPI_CR_RXIE;
			break;
		case SPI_INT_TXIE :
			// Enable TXIE at Transmit Function
			break;
		case SPI_INT_RXIE_TXIE :
			u32Reg = SPI_CR_RXIE;
			// Enable TXIE at Transmit Function
			break;
		case SPI_INT_SSCIE :
			u32Reg = SPI_CR_SSCIE;
			break;
		case SPI_INT_RXIE_SSCIE :
			u32Reg = SPI_CR_RXIE | SPI_CR_SSCIE;
			break;
		case SPI_INT_TXIE_SSCIE :
			u32Reg = SPI_CR_SSCIE;
			// Enable TXIE at Transmit Function
			break;
		case SPI_INT_RXIE_TXIE_SSCIE :
			u32Reg = SPI_CR_RXIE | SPI_CR_SSCIE;
			// Enable TXIE at Transmit Function
			break;
		default :
			u32Reg = 0;
			break;
	}
	SPIn->CR |= (u32Reg & 0x1C000);

	return HAL_OK;
}

/**********************************************************************//**
 * @brief						Enable or disable SPI peripheral's operation
 * @param[in]	SPIn	SPI peripheral, should be:
 *											- SP	:SPI20~21 peripheral
 * @param[in]	spi_en New State of SPIn peripheral's operation, should be:
 *											- ENABLE
 *											- DISABLE
 * @return				None
 **********************************************************************/
void HAL_SPI_Command(SPI_Type* SPIn, EN_DIS_Type spi_en)
{
	if(spi_en == ENABLE)
	{
		SPIn->EN = 0x01;	// Data must be written to the TDR before this bit is set enable
	}
	else
	{
		SPIn->EN = 0x00;
	}
}

/**********************************************************************//**
 * @brief						control SS Output in SPI peripheral
 * @param[in]	SPIn	SPI peripheral selected, should be:
 *											- SP	:SPI20~21 peripheral
 * @param[in]	ss_output	 State of Slave Output , should be:
 *											- SPI_SSOUT_LOW	: SS out value is Low
 *											- SPI_SSOUT_HIGH	: SS out value is High
 *											use this function after setting as HAL_SPI_ConfigSSOutputMode(SPIx,SS_MANUAL)
 * @return				None
 **********************************************************************/
void HAL_SPI_SetSSOutput(SPI_Type* SPIn, uint8_t ss_output)
{
	if(SPIn->CR & SPI_CR_SSMOD)	// Check SS manual mode
	{
		if(ss_output == SPI_SSOUT_HIGH)
		{
			SPIn->CR |= (SPI_CR_SSOUT);
		}
		else
		{
			SPIn->CR &= ~(SPI_CR_SSOUT);
		}
	}
	else
	{
		// You have to set SS manual mode before setting the SSOUT.
	}
}

/**********************************************************************//**
 * @brief						Transmit a single data through SPIn peripheral (Polling mode)
 * @param[in]	SPIn	SPI peripheral selected, should be:
 *											- SP	:SPI20~21 peripheral
 * @param[in]	tx_data		Data to transmit
 * @return				None
 **********************************************************************/
void HAL_SPI_TransmitData_POL(SPI_Type* SPIn, uint32_t tx_data)
{
	while(!(SPIn->SR & SPI_SR_TRDY));	// Wait until transmit buffer is ready for use.
	HAL_SPI_Command(SPIn, DISABLE);
	SPIn->RDR_TDR = tx_data;
	HAL_SPI_Command(SPIn, ENABLE);
}

/**********************************************************************//**
 * @brief						Receive a single data from SPIx peripheral (Polling mode)
 * @param[in]	SPIn	SPI peripheral selected, should be
 *											- SP	:SPI20~21 peripheral
 * @return				Received data
 **********************************************************************/
uint32_t HAL_SPI_ReceiveData_POL(SPI_Type* SPIn)
{
	while(!(SPIn->SR & SPI_SR_TRDY));	// Wait until transmit buffer is ready for use.
	SPIn->RDR_TDR = 0x00;										// Dummy data
	while(!(SPIn->SR & SPI_SR_RRDY));	// Wait until receive buffer holds data.
	return ((uint32_t) (SPIn->RDR_TDR));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


/*********************************************************************//**
 * @brief						Initialize RingBuffer Parameters
 * @param[in]	SPIn	Pointer to selected SPI peripheral, should be:
 *											- SPI20	:		SPI20 peripheral
 *											- SPI21	:		SPI21 peripheral
 * @return				None
 **********************************************************************/
void HAL_SPI_RingBuffer_Init(SPI_Type *SPIn)
{
	if(SPIn == SPI20)
	{
		spi_tx20_RingBuffer.HeadPtr = 0;
		spi_tx20_RingBuffer.TailPtr = 0;
		spi_tx20_RingBuffer.State = SPI_TX_IDLE;
		
		spi_rx20_RingBuffer.HeadPtr = 0;
		spi_rx20_RingBuffer.TailPtr = 0;
		spi_rx20_RingBuffer.State = SPI_RX_IDLE;
	}
	else if(SPIn == SPI21)
	{
		spi_tx21_RingBuffer.HeadPtr = 0;
		spi_tx21_RingBuffer.TailPtr = 0;
		spi_tx21_RingBuffer.State = SPI_TX_IDLE;

		spi_rx21_RingBuffer.HeadPtr = 0;
		spi_rx21_RingBuffer.TailPtr = 0;
		spi_rx21_RingBuffer.State = SPI_RX_IDLE;
	}
}

/*********************************************************************//**
 * @brief						SPI Transmit Data (Interrupt mode)
 * @param[in]	SPIn	Pointer to selected SPI peripheral, should be:
 *											- SPI20	:		SPI20 peripheral
 *											- SPI21	:		SPI21 peripheral
 * @param[in]	tx_data		SPI Transmit Data
 * @return				None
 **********************************************************************/
void HAL_SPI_TransmitData_INT(SPI_Type *SPIn, uint8_t tx_data)
{
	SPIn->CR &= ~(SPI_CR_TXIE);		// Disable tx interrupt

	/* SPI20 Unit */
	if(SPIn == SPI20)
	{
		if(spi_tx20_RingBuffer.HeadPtr == spi_tx20_RingBuffer.TailPtr)
		{
			if(spi_tx20_RingBuffer.State == SPI_TX_IDLE)
			{
				// First data to transmit
				while(!(SPIn->SR & SPI_SR_TRDY));	// Wait until transmit buffer is ready for use.
				SPIn->RDR_TDR = tx_data;
				HAL_SPI_Command(SPIn, ENABLE);

				spi_tx20_RingBuffer.State = SPI_TX_BUSY;
			}
			else
			{
				spi_tx20_RingBuffer.Buffer[spi_tx20_RingBuffer.HeadPtr++] = tx_data;
				// Check buffer max length
				if(spi_tx20_RingBuffer.HeadPtr > sRING_BUFFER_LENGTH)
				{
					spi_tx20_RingBuffer.HeadPtr = 0;
				}
//				SPIn->CR |= (SPI_CR_TXIE);			// Enable tx interrupt
			}
		}
		else
		{
			spi_tx20_RingBuffer.Buffer[spi_tx20_RingBuffer.HeadPtr++] = tx_data;
			// Check buffer max length
			if(spi_tx20_RingBuffer.HeadPtr > sRING_BUFFER_LENGTH)
			{
				spi_tx20_RingBuffer.HeadPtr = 0;
			}
//			SPIn->CR |= (SPI_CR_TXIE);			// Enable tx interrupt
		}
	}
	/* SPI21 Unit */
	else if(SPIn == SPI21)
	{
		if(spi_tx21_RingBuffer.HeadPtr == spi_tx21_RingBuffer.TailPtr)
		{
			if(spi_tx21_RingBuffer.State == SPI_TX_IDLE)
			{
				// First data to transmit
				while(!(SPIn->SR & SPI_SR_TRDY));	// Wait until transmit buffer is ready for use.
				SPIn->RDR_TDR = tx_data;
				HAL_SPI_Command(SPIn, ENABLE);

				spi_tx21_RingBuffer.State = SPI_TX_BUSY;
			}
			else
			{
				spi_tx21_RingBuffer.Buffer[spi_tx21_RingBuffer.HeadPtr++] = tx_data;
				// Check buffer max length
				if(spi_tx21_RingBuffer.HeadPtr > sRING_BUFFER_LENGTH)
				{
					spi_tx21_RingBuffer.HeadPtr = 0;
				}
				SPIn->CR |= (SPI_CR_TXIE);			// Enable tx interrupt
			}
		}
		else
		{
			spi_tx21_RingBuffer.Buffer[spi_tx21_RingBuffer.HeadPtr++] = tx_data;
			// Check buffer max length
			if(spi_tx21_RingBuffer.HeadPtr > sRING_BUFFER_LENGTH)
			{
				spi_tx21_RingBuffer.HeadPtr = 0;
			}
			SPIn->CR |= (SPI_CR_TXIE);			// Enable tx interrupt
		}
	}
}

/*********************************************************************//**
 * @brief						SPI Get Received Data (Interrupt mode)
 * @param[in]	SPIn	Pointer to selected SPI peripheral, should be:
 *											- SPI20	:		SPI20 peripheral
 *											- SPI21	:		SPI21 peripheral
 * @return				Received data
 **********************************************************************/
int8_t HAL_SPI_ReceivedData_INT(SPI_Type *SPIn)
{
	/* SPI20 Unit */
	if(SPIn == SPI20)
	{
		if(spi_rx20_RingBuffer.HeadPtr != spi_rx20_RingBuffer.TailPtr)
		{
			// Check buffer max length
			if(spi_rx20_RingBuffer.TailPtr > sRING_BUFFER_LENGTH)
			{
				spi_rx20_RingBuffer.TailPtr = 0;
				return spi_rx20_RingBuffer.Buffer[spi_rx20_RingBuffer.TailPtr];
			}
			return spi_rx20_RingBuffer.Buffer[spi_rx20_RingBuffer.TailPtr++];
		}
		else
		{
				// No received data
				spi_rx20_RingBuffer.State = SPI_RX_IDLE;
				return -1;
		}
	}
	/* SPI21 Unit */
	else if(SPIn == SPI21)
	{
		if(spi_rx21_RingBuffer.HeadPtr != spi_rx21_RingBuffer.TailPtr)
		{
			// Check buffer max length
			if(spi_rx21_RingBuffer.TailPtr > sRING_BUFFER_LENGTH)
			{
				spi_rx21_RingBuffer.TailPtr = 0;
				return spi_rx21_RingBuffer.Buffer[spi_rx21_RingBuffer.TailPtr];
			}
			return spi_rx21_RingBuffer.Buffer[spi_rx21_RingBuffer.TailPtr++];
		}
		else
		{
				// No received data
				spi_rx21_RingBuffer.State = SPI_RX_IDLE;
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
 * @brief						SPI Handler (Interrupt Handler)
 * @param[in]	SPIn	Pointer to selected SPI peripheral, should be:
 *											- SPI20	:SPI20 peripheral
 *											- SPI21	:SPI21 peripheral
 * @return				None
 **********************************************************************/
void HAL_SPI_Handler(SPI_Type *SPIn)
{
	volatile uint32_t int_status;
	
	int_status = SPIn->SR;

	/* Line Interrupt */
	if((int_status&SPI_SR_UDRF) || (int_status&SPI_SR_OVRF))
	{
		SPIn->CR |= (3<<19);		// Clear Tx/Rx Buffer
	}
	else
	{
		/* Tx Interrupt */
		if(int_status & SPI_SR_TRDY)
		{
			HAL_SPI_TX_Handler(SPIn);		// Tx Process
		}
		/* Rx Interrupt */
		if(int_status & SPI_SR_RRDY)
		{
			HAL_SPI_RX_Handler(SPIn);		// Rx Process
		}
	}
}

/*********************************************************************//**
 * @brief						SPI Transmit Handler (Interrupt Handler)
 * @param[in]	SPIn	Pointer to selected SPI peripheral, should be:
 *											- SPI20	:SPI20 peripheral
 *											- SPI21	:SPI21 peripheral
 * @return				None
 **********************************************************************/
void HAL_SPI_TX_Handler(SPI_Type *SPIn)
{
	/* SPI20 Unit */
	if(SPIn == SPI20)
	{
		if(spi_tx20_RingBuffer.HeadPtr != spi_tx20_RingBuffer.TailPtr)
		{
			SPIn->RDR_TDR = spi_tx20_RingBuffer.Buffer[spi_tx20_RingBuffer.TailPtr++];

			// Check buffer max length
			if(spi_tx20_RingBuffer.TailPtr > sRING_BUFFER_LENGTH)
			{
				spi_tx20_RingBuffer.TailPtr = 0;
			}
		}
		else
		{
				spi_tx20_RingBuffer.State = SPI_TX_IDLE;
				SPIn->CR &= ~(SPI_CR_TXIE);		// Disable tx interrupt
		}
	}
	/* SPI21 Unit */
	else if(SPIn == SPI21)
	{
		if(spi_tx21_RingBuffer.HeadPtr != spi_tx21_RingBuffer.TailPtr)
		{
			SPIn->RDR_TDR = spi_tx21_RingBuffer.Buffer[spi_tx21_RingBuffer.TailPtr++];

			// Check buffer max length
			if(spi_tx21_RingBuffer.TailPtr > sRING_BUFFER_LENGTH)
			{
				spi_tx21_RingBuffer.TailPtr = 0;
			}
		}
		else
		{
				spi_tx21_RingBuffer.State = SPI_TX_IDLE;
				SPIn->CR &= ~(SPI_CR_TXIE);		// Disable tx interrupt
		}
	}
}

/*********************************************************************//**
 * @brief						SPI Receive Handler (Interrupt Handler)
 * @param[in]	SPIn	Pointer to selected SPI peripheral, should be:
 *											- SPI20	:SPI20 peripheral
 *											- SPI21	:SPI21 peripheral
 * @return				None
 **********************************************************************/
void HAL_SPI_RX_Handler(SPI_Type *SPIn)
{
	/* SPI20 Unit */
	if(SPIn == SPI20)
	{
		if(spi_rx20_RingBuffer.HeadPtr == spi_rx20_RingBuffer.TailPtr)
		{
			if(spi_rx20_RingBuffer.State == SPI_RX_IDLE)
			{
				spi_rx20_RingBuffer.Buffer[spi_rx20_RingBuffer.HeadPtr++] = SPIn->RDR_TDR;		// First data to receive
				spi_rx20_RingBuffer.State = SPI_RX_BUSY;
				// Check buffer max length
				if(spi_rx20_RingBuffer.HeadPtr > sRING_BUFFER_LENGTH)
				{
					spi_rx20_RingBuffer.HeadPtr = 0;
				}
			}
			else
			{
				spi_rx20_RingBuffer.Buffer[spi_rx20_RingBuffer.HeadPtr++] = SPIn->RDR_TDR;
				// Check buffer max length
				if(spi_rx20_RingBuffer.HeadPtr > sRING_BUFFER_LENGTH)
				{
					spi_rx20_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			spi_rx20_RingBuffer.Buffer[spi_rx20_RingBuffer.HeadPtr++] = SPIn->RDR_TDR;
			// Check buffer max length
			if(spi_rx20_RingBuffer.HeadPtr > sRING_BUFFER_LENGTH)
			{
				spi_rx20_RingBuffer.HeadPtr = 0;
			}
		}
	}
	/* SPI21 Unit */
	else if(SPIn == SPI21)
	{
		if(spi_rx21_RingBuffer.HeadPtr == spi_rx21_RingBuffer.TailPtr)
		{
			if(spi_rx21_RingBuffer.State == SPI_RX_IDLE)
			{
				spi_rx21_RingBuffer.Buffer[spi_rx21_RingBuffer.HeadPtr++] = SPIn->RDR_TDR;		// First data to receive
				spi_rx21_RingBuffer.State = SPI_RX_BUSY;
				// Check buffer max length
				if(spi_rx21_RingBuffer.HeadPtr > sRING_BUFFER_LENGTH)
				{
					spi_rx21_RingBuffer.HeadPtr = 0;
				}
			}
			else
			{
				spi_rx21_RingBuffer.Buffer[spi_rx21_RingBuffer.HeadPtr++] = SPIn->RDR_TDR;
				// Check buffer max length
				if(spi_rx21_RingBuffer.HeadPtr > sRING_BUFFER_LENGTH)
				{
					spi_rx21_RingBuffer.HeadPtr = 0;
				}
			}
		}
		else
		{
			spi_rx21_RingBuffer.Buffer[spi_rx21_RingBuffer.HeadPtr++] = SPIn->RDR_TDR;
			// Check buffer max length
			if(spi_rx21_RingBuffer.HeadPtr > sRING_BUFFER_LENGTH)
			{
				spi_rx21_RingBuffer.HeadPtr = 0;
			}
		}
	}
}


/* --------------------------------- End Of File ------------------------------ */
