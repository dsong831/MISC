/**********************************************************************//**
* @file				A31T21x_hal_dmac.h
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


#ifndef A31T21X_HAL_DMAC_H
#define A31T21X_HAL_DMAC_H


/* Includes -------------------------------------------------------------- */
#include "A31T21x.h"
#include "A31T21x_hal_aa_types.h"


/* Private macros -------------------------------------------------------- */
/* -----------------------------------  DMAC_CR  ----------------------------------- */
#define DMAC_CR_DIR_Pos										1																																					/*!< DMAC CR: DIR Position							*/
#define DMAC_CR_DIR_Msk										(0x01UL << DMAC_CR_DIR_Pos)													/*!< DMAC CR: DIR Mask									*/
#define DMAC_CR_SIZE_Pos									2																																					/*!< DMAC CR: SIZE Position						*/
#define DMAC_CR_SIZE_Msk									(0x03UL << DMAC_CR_SIZE_Pos)												/*!< DMAC CR: SIZE Mask								*/
#define DMAC_CR_PERISEL_Pos						8																																					/*!< DMAC CR: PERISEL Position			*/
#define DMAC_CR_PERISEL_Msk						(0x0FUL << DMAC_CR_PERISEL_Pos)									/*!< DMAC CR: PERISEL Mask					*/
#define DMAC_CR_TRANSCNT_Pos				16																																				/*!< DMAC CR: TRANSCNT Position	*/
#define DMAC_CR_TRANSCNT_Msk				(0x00000FFFUL << DMAC_CR_TRANSCNT_Pos)		/*!< DMAC CR: TRANSCNT Mask			*/
/* -----------------------------------  DMAC_SR  ----------------------------------- */
#define DMAC_SR_DMAEN_Pos							0																																					/*!< DMAC SR: DMAEN Position				*/
#define DMAC_SR_DMAEN_Msk							(0x01UL << DMAC_SR_DMAEN_Pos)										/*!< DMAC SR: DMAEN Mask						*/
#define DMAC_SR_EOT_Pos									7																																					/*!< DMAC SR: EOT Position						*/
#define DMAC_SR_EOT_Msk									(0x01UL << DMAC_SR_EOT_Pos)													/*!< DMAC SR: EOT Mask								*/
/* -----------------------------------  DMAC_PAR  ---------------------------------- */
#define DMAC_PAR_PAR_Pos								0																																					/*!< DMAC PAR: PAR Position					*/
#define DMAC_PAR_PAR_Msk								(0x0000FFFFUL << DMAC_PAR_PAR_Pos)							/*!< DMAC PAR: PAR Mask							*/
/* -----------------------------------  DMAC_MAR  ---------------------------------- */
#define DMAC_MAR_MAR_Pos								0																																					/*!< DMAC MAR: MAR Position					*/
#define DMAC_MAR_MAR_Msk								(0x0000FFFFUL << DMAC_MAR_MAR_Pos)						/*!< DMAC MAR: MAR Mask							*/

/* Public Macros -------------------------------------------------------------- */
/** PERISEL Selection */
#define DMAC_PERISEL_IDLE								(0UL)		/**< CHANNEL IDLE */
#define DMAC_PERISEL_UART0RX				(1UL)		/**< UART0 RX */
#define DMAC_PERISEL_UART0TX				(2UL)		/**< UART0 TX */
#define DMAC_PERISEL_UART1RX				(3UL)		/**< UART1 RX */
#define DMAC_PERISEL_UART1TX				(4UL)		/**< UART1 TX */
#define DMAC_PERISEL_USART10RX		(5UL)		/**< USART10 RX */
#define DMAC_PERISEL_USART10TX		(6UL)		/**< USART10 TX */
#define DMAC_PERISEL_USART11RX		(7UL)		/**< USART11 RX */
#define DMAC_PERISEL_USART11TX		(8UL)		/**< USART11 TX */
#define DMAC_PERISEL_SPI20RX				(9UL)		/**< SPI20 RX */
#define DMAC_PERISEL_SPI20TX					(10UL)	/**< SPI20 TX */
#define DMAC_PERISEL_SPI21RX				(11UL)	/**< SPI21 RX */
#define DMAC_PERISEL_SPI21TX					(12UL)	/**< SPI21 TX */
#define DMAC_PERISEL_CRC								(13UL)	/**< CRC */
/** Size in Source transfer width and Destination transfer width definitions */
#define DMAC_SIZE_BYTE							(0UL)		/**< Width = 1 byte */
#define DMAC_SIZE_HALFWORD		(1UL)		/**< Width = 2 bytes */
#define DMAC_SIZE_WORD						(2UL)		/**< Width = 4 bytes */
/** Width in Source transfer width and Destination transfer width definitions */
#define DMAC_DIR_TX										(0UL)		/**< TX */
#define DMAC_DIR_RX										(1UL)		/**< RX */


/* Public Types --------------------------------------------------------------- */
typedef struct {
	uint32_t TransferCnt;		/** Number of DMA transfer remained */
	uint32_t PeriSel;						/** Peripheral selection */
	uint32_t TransferSize;	/** Bus transfer size */
	uint32_t Dir;									/** Select transfer direction */
	uint32_t PeriAddr;					/** Peri address */
	uint32_t MemAddr;				/** Mem address */
} DMAC_CFG_Type;


/* Public Functions ----------------------------------------------------------- */
HAL_Status_Type HAL_DMAC_Init(DMA_Type *DMACx, DMAC_CFG_Type *DMAC_Config, EN_DIS_Type dmac_en);
HAL_Status_Type HAL_DMAC_Cmd(DMA_Type *DMACx, SET_RESET_Type NewState);
SET_RESET_Type HAL_DMAC_GetStatus(DMA_Type *DMACx);


#endif		/* A31T21X_HAL_DMAC_H */


/* --------------------------------- End Of File ------------------------------ */
