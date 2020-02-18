/**********************************************************************
* @file		A31G213_dma.h
* @brief	Contains all macro definitions and function prototypes
* 			support for DMA firmware library on A34M418
* @version	1.0
* @date		
* @author ABOV Application2 team
*
* Copyright(C)  2017, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/



/* Includes ------------------------------------------------------------------- */
#include "a31g213.h"


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	uint32_t	transcnt;
	uint32_t	perisel;
	uint32_t	bussize;
	uint32_t	dirsel;
} DMA_CFG_Type;

extern enum
{
	CHANNEL_IDLE = 0,    /*0*/
	UART0_RX,			/*1*/
	UART0_TX,  		/*2*/
	UART1_RX, 		/*3*/
	UART1_TX,			/*4*/
	CRC_TX,				/*5*/
	NO_NA_01, 				/*6*/
	NO_NA_02, 				/*7*/
	NO_NA_03, 				/*8*/
	SPI20_RX, 		/*9*/
	SPI20_TX, 		/*10*/
	SPI21_RX, 		/*11*/
	SPI21_TX 		/*12*/

} DMA_PERISEL;



/* Public Macros -------------------------------------------------------------- */
/*********************************************************************//**
 * DMA Control	defines
 **********************************************************************/
#define DMA_CR_BYTE_TRANS			(0<<2)
#define DMA_CR_HWORD_TRANS			(1<<2)
#define DMA_CR_WORD_TRANS			(2<<2)

#define DMA_CR_DIR_MEM_TO_PERI		(0<<1)
#define DMA_CR_DIR_PERI_TO_MEM		(1<<1)


/*********************************************************************//**
 * DMA Control	defines
 **********************************************************************/
#define DMA_SR_EOT					(1<<7)



/* Public Functions ----------------------------------------------------------- */
void DMA_Init(void);
void DMA_Cmd(DMAC_Type* DMAx, DMA_CFG_Type *dma_cfg);
void DMA_Start(DMAC_Type* DMAx);
void DMA_Stop(DMAC_Type* DMAx);
uint32_t DMA_GetStatus(DMAC_Type* DMAx);
void DMA_SetPAR(DMAC_Type* DMAx, uint32_t peri_addr);
void DMA_SetMAR(DMAC_Type* DMAx, uint32_t mem_addr);




#ifdef __cplusplus
}
#endif




/* --------------------------------- End Of File ------------------------------ */
