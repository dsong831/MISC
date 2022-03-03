/**********************************************************************//**
* @file				A31T21x_hal_spi.h
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


#ifndef A31T21X_HAL_SPI_H
#define A31T21X_HAL_SPI_H


/* Includes -------------------------------------------------------------- */
#include "A31T21x.h"
#include "A31T21x_hal_aa_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Private macros -------------------------------------------------------- */
#define SPI_CR_BITSZ_8		((uint8_t)(0<<0))		/*!<8-bit data length */
#define SPI_CR_BITSZ_9		((uint8_t)(1<<0))		/*!<9-bit data length */
#define SPI_CR_BITSZ_16	((uint8_t)(2<<0))		/*!<16-bit data length */
#define SPI_CR_BITSZ_17	((uint8_t)(3<<0))		/*!<17-bit data length */

#define SPI_CR_CPOL				((uint32_t)(1<<2))		/*!<Select an active-low clock */
#define SPI_CR_CPHA				((uint32_t)(1<<3))		/*!<Samples data generated at even number edges */
#define SPI_CR_MSBF				((uint32_t)(1<<4))		/*!<Selects MSB-first transmission */
#define SPI_CR_MS						((uint32_t)(1<<5))		/*!<SPI Master mode */
#define SPI_CR_SSPOL			((uint32_t)(1<<8))		/*!<SS signal active high */
#define SPI_CR_SSMO			((uint32_t)(1<<9))		/*!<Enables the SS output signal */
#define SPI_CR_SSMASK		((uint32_t)(1<<10))	/*!<Mask the signal from pin SS */
#define SPI_CR_LBE					((uint32_t)(1<<11))	/*!<Uses loop-back mode */
#define SPI_CR_SSOUT			((uint32_t)(1<<12))	/*!<High SS output */
#define SPI_CR_SSMOD		((uint32_t)(1<<13))	/*!<SS signal generated, according to the SSOUT */
#define SPI_CR_RXIE					((uint32_t)(1<<14))	/*!<Enables the receive interrupt */
#define SPI_CR_TXIE					((uint32_t)(1<<15))	/*!<Enables the transmit interrupt */
#define SPI_CR_SSCIE				((uint32_t)(1<<16))	/*!<Enables interrupts for both nSS edges */
#define SPI_CR_RXDIE				((uint32_t)(1<<17))	/*!<Enables the DMA Rx complete interrupt */
#define SPI_CR_TXDIE				((uint32_t)(1<<18))	/*!<Enables the DMA Tx complete interrupt */
#define SPI_CR_RXBC				((uint32_t)(1<<19))	/*!<Clears the Rx buffer */
#define SPI_CR_TXBC				((uint32_t)(1<<20))	/*!<Clears the Tx buffer */

#define SPI_SR_RRDY				((uint32_t)(1<<0))		/*!<The receive buffer holds data */
#define SPI_SR_TRDY				((uint32_t)(1<<1))		/*!<The transmit buffer is ready for use */
#define SPI_SR_TXIDLE			((uint32_t)(1<<2))		/*!<The SPI is idle */
#define SPI_SR_UDRF				((uint32_t)(1<<3))		/*!<A transmit underrun error has been detected */
#define SPI_SR_OVRF				((uint32_t)(1<<4))		/*!<A receive overrun error has been detected */
#define SPI_SR_SSON				((uint32_t)(1<<5))		/*!<Enables the SS signal */
#define SPI_SR_SSDET			((uint32_t)(1<<6))		/*!<An SS edge has been detected */
#define SPI_SR_SBUSY			((uint32_t)(1<<7))		/*!<A transmission or reception is in progress */
#define SPI_SR_RXDMAF		((uint32_t)(1<<8))		/*!<A DMA receptoin has been completed. */
#define SPI_SR_TXDMAF		((uint32_t)(1<<9))		/*!<A DMA transmission has been completed. */

/* Macro defines for SPIConfigStruct */
#define SPI_CPOL_ActiveLow			(0)
#define SPI_CPOL_ActiveHigh		(1)
#define SPI_CPHA_ODD							(0)
#define SPI_CPHA_EVEN						(1)
#define SPI_MSBF_LSB							(0)
#define SPI_MSBF_MSB						(1)
#define SPI_MS_SLAVE							(0)
#define SPI_MS_MASTER					(1)
#define SPI_SSPOL_ActiveLow	(0)
#define SPI_SSPOL_ActiveHigh	(1)
#define SPI_SSMO_DISABLE			(0)
#define SPI_SSMO_ENABLE				(1)
#define SPI_SSMSK_DISABLE		(0)
#define SPI_SSMSK_ENABLE			(1)
#define SPI_LBE_DISABLE					(0)
#define SPI_LBE_ENABLE					(1)
#define SPI_SSOUT_LOW					(0)
#define SPI_SSOUT_HIGH					(1)
#define SPI_SSMOD_AUTO				(0)
#define SPI_SSMOD_MANUAL		(1)


// For Using SPI Buffer
#define SPI_BUFFER_LENGTH		(20)
#define SPI_TX_IDLE									(0)
#define SPI_TX_BUSY								(1)
#define SPI_RX_IDLE									(0)
#define SPI_RX_BUSY								(1)


/* Public Types --------------------------------------------------------------- */
typedef struct {
	uint32_t tBitsize;				/** Databit number, should be 
																				- SPI_CR_BITSZ_8
																				- SPI_CR_BITSZ_9
																				- SPI_CR_BITSZ_16
																				- SPI_CR_BITSZ_17 */
	uint32_t tCPOL;					/** Clock polarity
																				- SPI_CPOL_ActiveLow
																				- SPI_CPOL_ActiveHigh */
	uint32_t tCPHA;					/** Clock phase
																				- SPI_CPHA_ODD
																				- SPI_CPHA_EVEN */
	uint32_t tMSBF;					/** MSB/LSB select 
																				- SPI_MSBF_LSB
																				- SPI_MSBF_MSB */
	uint32_t tMS;							/** Master/Slave select 
																				- SPI_MS_SLAVE
																				- SPI_MS_MASTER */
	uint32_t tBaudrate;		/** PCLK / (BaudRate+1)  BaudRate>=2 (0x0002~0xFFFF) */
} SPI_CFG_Type;


typedef enum {
	SPI_INT_POLLING = 0,					/**< Polling mode */
	SPI_INT_RXIE,											/**< Rx Interrupt Only */
	SPI_INT_TXIE,											/**< Tx Interrupt Only */
	SPI_INT_RXIE_TXIE,							/**< Rx/Tx Interrupt */
	SPI_INT_SSCIE,										/**< SS edge change Interrupt Only */
	SPI_INT_RXIE_SSCIE,						/**< SS edge/RX Interrupt */
	SPI_INT_TXIE_SSCIE,						/**< SS edge/TX Interrupt */
	SPI_INT_RXIE_TXIE_SSCIE,	/**< SS edge/RX/TX Interrupt */
	SPI_INT_MASTER,								/**< Master Interrupt Setting */
	SPI_INT_SLAVE										/**< Slave Interrupt Setting */
} SPI_INTCFG_Type;


// For Using SPI Buffer
typedef struct {
	uint8_t Buffer[SPI_BUFFER_LENGTH];
	uint8_t State;
	uint32_t DataLength;
} SPI_Buffer_Type;


/* Public Functions ----------------------------------------------------------- */
void HAL_SPI_TransmitData(SPI_Type* SPIn, uint32_t tx_data);
uint32_t HAL_SPI_ReceiveData(SPI_Type* SPIn);
HAL_Status_Type HAL_SPI_Init(SPI_Type *SPIn, SPI_CFG_Type *SPIConfigStruct);
HAL_Status_Type HAL_SPI_DeInit(SPI_Type* SPIn);
HAL_Status_Type HAL_SPI_ConfigInterrupt(SPI_Type* SPIn, SPI_INTCFG_Type SPIIntConfig, EN_DIS_Type int_en);
void HAL_SPI_Command(SPI_Type* SPIn, EN_DIS_Type spi_en);
void HAL_SPI_SetSSOutput(SPI_Type* SPIn, uint8_t ss_output);


void HAL_SPI_Buffer_Init(SPI_Type *SPIn);
void HAL_SPI_TransmitReceiveData_INT(SPI_Type *SPIn, uint32_t *p_txdata, uint8_t tx_length, uint32_t *p_rxdata, uint8_t rx_length);
void HAL_SPI_Handler_IT(SPI_Type *SPIn);

extern SPI_Buffer_Type	spi_tx20_Buffer;
extern SPI_Buffer_Type	spi_tx21_Buffer;
extern SPI_Buffer_Type	spi_rx20_Buffer;
extern SPI_Buffer_Type	spi_rx21_Buffer;


#ifdef __cplusplus
}
#endif

#endif		/* A31T21X_HAL_SPI_H */


/* --------------------------------- End Of File ------------------------------ */
