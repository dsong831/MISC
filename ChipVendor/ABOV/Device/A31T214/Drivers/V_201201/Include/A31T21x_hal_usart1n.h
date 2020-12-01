/**********************************************************************//**
* @file				A31T21x_hal_usart1n.h
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


#ifndef A31T21X_HAL_USART1n_H
#define A31T21X_HAL_USART1n_H


/* Includes -------------------------------------------------------------- */
#include "A31T21x.h"
#include "A31T21x_hal_aa_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Private macros -------------------------------------------------------- */
#define USART_BLOCKING_TIMEOUT			(0xFFFFFFFFUL)

#define USART_ST_DRE										((uint8_t)(1<<7)) 		/*!<Transmit Data Register Empty Interrut Flag */
#define USART_ST_TXC											((uint8_t)(1<<6)) 		/*!<Transmit Complete Interrupt Flag */
#define USART_ST_RXC										((uint8_t)(1<<5)) 		/*!<Receive Complete Interrupt Flag */
#define USART_ST_WAKE									((uint8_t)(1<<4)) 		/*!<Wake-up Interrupt Flag */
#define USART_ST_DOR										((uint8_t)(1<<2)) 		/*!<Data Overrun occur */
#define USART_ST_FE												((uint8_t)(1<<1)) 		/*!<Frame Error occur */
#define USART_ST_PE												((uint8_t)(1<<0)) 		/*!<Parity Error occur */
#define USART_ST_BITMASK							((uint8_t)(0xFF)) 		/*!<USART Status Register Bit Mask */

// Macro defines for SPIConfigStruct
#define USART_CPOL_ActiveLow				(0)
#define USART_CPOL_ActiveHigh			(1)
#define USART_CPHA_ODD								(0)
#define USART_CPHA_EVEN							(1)
#define USART_MSBF_LSB								(0)
#define USART_MSBF_MSB								(1)
#define USART_MS_SLAVE								(0)
#define USART_MS_MASTER							(1)
#define USART_SS_DISABLE							(0)
#define USART_SS_ENABLE							(1)


/* Public Types --------------------------------------------------------------- */
typedef enum {
	USART_UART_MODE = 0,			/*!< UART mode */
	USART_USART_MODE,				/*!< USART mode */
	USART_SPI_MODE = 3					/*!< SPI mode */
} USART_OPMODE_Type;

typedef enum {
	USART_DATALEN_5 = 0,				/*!< USART 5 bit data mode */
	USART_DATALEN_6,							/*!< USART 6 bit data mode */
	USART_DATALEN_7,							/*!< USART 7 bit data mode */
	USART_DATALEN_8,							/*!< USART 8 bit data mode */
	USART_DATALEN_9 = 7				/*!< USART 9 bit data mode */
} USART_DATALEN_Type;

typedef enum {
	USART_STOPBIT_1 = 0,				/*!< USART 1 Stop Bits Select */
	USART_STOPBIT_2							/*!< USART 2 Stop Bits Select */
} USART_STOPBIT_Type;

typedef enum {
	USART_PARITY_NONE = 0,		/*!< No parity */
	USART_PARITY_EVEN = 2,		/*!< Even parity */
	USART_PARITY_ODD = 3			/*!< Odd parity */
} USART_PARITY_Type;

typedef struct {
	uint32_t tBaudRate;															/**< USART baud rate */
	USART_OPMODE_Type tOpMode;				/**< USART operating mode :
																															- USART_UART_MODE
																															- USART_USART_MODE
																															- USART_SPI_MODE */
	USART_DATALEN_Type tDataLength;		/**< Number of data bits, should be:
																															- USART_DATABIT_5: USART 5 bit data mode
																															- USART_DATABIT_6: USART 6 bit data mode
																															- USART_DATABIT_7: USART 7 bit data mode
																															- USART_DATABIT_8: USART 8 bit data mode
																															- USART_DATABIT_9: USART 9 bit data mode	*/	
  USART_STOPBIT_Type tStopbit;						/**< Number of stop bits, should be:
																															- USART_STOPBIT_1: USART 1 Stop Bits Select
																															- USART_STOPBIT_2: USART 2 Stop Bits Select	*/
  USART_PARITY_Type tParity;								/**< Parity selection, should be:
																															- USART_PARITY_NONE: No parity
																															- USART_PARITY_ODD: Odd parity
																															- USART_PARITY_EVEN: Even parity */
	uint32_t tCPOL;																			/** Clock polarity
																															- SPI_CPOL_ActiveLow
																															- SPI_CPOL_ActiveHigh */
	uint32_t tCPHA;																			/** Clock phase
																															- SPI_CPHA_ODD
																															- SPI_CPHA_EVEN */
	uint32_t tMSBF;																			/** MSB/LSB select 
																															- SPI_MSBF_LSB
																															- SPI_MSBF_MSB */
	uint32_t tMS;																					/** Master/Slave select 
																															- SPI_MS_SLAVE / USART_MS_SLAVE
																															- SPI_MS_MASTER / USART_MS_MASTER */
} USART_CFG_Type;

typedef enum {
	USART_INT_POLLING = 0,					/**< Polling mode */
	USART_INT_WAKEIE,									/**< Wkaeup Interrupt Only */
	USART_INT_RXCIE,										/**< Rx Interrupt Only */
	USART_INT_TXCIE,											/**< Tx Interrupt Only */
	USART_INT_RXCIE_TXCIE,					/**< Rx/Tx Interrupt */
	USART_INT_DRIE												/**< Data Register Empty Interrupt */
} USART_INTCFG_Type;


/* Public Functions ----------------------------------------------------------- */
HAL_Status_Type HAL_USART_Init(USART_Type *USARTn, USART_CFG_Type *USARTConfigStruct);
HAL_Status_Type HAL_USART_DeInit(USART_Type* USARTn);
HAL_Status_Type HAL_USART_ConfigInterrupt(USART_Type *USARTn, USART_INTCFG_Type USARTIntConfig, EN_DIS_Type int_en);
void HAL_USART_BaudrateSet(USART_Type *USARTn, USART_OPMODE_Type USARTOPModeConfig, uint32_t baudrate);
void HAL_USART_Command(USART_Type* USARTn, EN_DIS_Type usart_en);
void HAL_USART_TransmitByte(USART_Type* USARTn, uint8_t tx_data);
uint8_t HAL_USART_ReceiveByte(USART_Type* USARTn);
uint32_t HAL_USART_Transmit(USART_Type *USARTn, uint8_t *txbuf, uint32_t buflen);
uint32_t HAL_USART_Receive(USART_Type *USARTn, uint8_t *rxbuf, uint32_t buflen);
void HAL_USART_SetSSOutput(USART_Type* USARTn, EN_DIS_Type ss_en);


/////////////////////////////////////////////////////////////////////////////////////////////////////
/* For Using UART Interrupt ----------------------------------------------------------- */
#define USART_BUFSIZE 32

typedef struct {
	uint32_t buffer_head;											/*!< USART buffer head index */
	uint32_t buffer_tail;													/*!< USART buffer tail index */
	uint8_t	tx[USART_BUFSIZE];					/*!< USART Tx data buffer */
	uint8_t	rx[USART_BUFSIZE];				/*!< USART Rx data buffer */
	uint8_t	tx_flag;																/*!< USART Tx flag */
	uint8_t	rx_flag;																/*!< USART Rx flag */
	SET_RESET_Type TxIntStat;						/*!< USART Current Tx Interrupt enable state */
} USART_BUFFER_T;

uint32_t USARTSend(USART_Type *USARTn, uint8_t *txbuf, uint8_t buflen);
uint32_t USARTReceive(USART_Type *USARTn, uint8_t *rxbuf, uint8_t buflen);
void USART_IRQHandler_IT(USART_Type *USARTn);

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif


#endif		/* A31T21X_HAL_USART1n_H */

/* --------------------------------- End Of File ------------------------------ */
