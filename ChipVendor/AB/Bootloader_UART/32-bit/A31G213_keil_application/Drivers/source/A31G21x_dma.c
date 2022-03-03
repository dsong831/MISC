/**********************************************************************
* @file		a31g213_dma.c
* @brief	Contains all functions support for rng firmware library
* 			on a31g213
* @version	1.0
* @date		
* @author	ABOV Application2 team
*
* Copyright(C) 2017, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/

/* Includes ------------------------------------------------------------------- */
//#include "A34M418_scu.h"
//#include "A34M418_dma.h"
//#include "A34M418_libcfg.h"
#include "a31g213_dma.h"
#include "a31g213_scu.h"

#define _DMAC
#ifdef _DMAC


/* Public Functions ----------------------------------------------------------- */
/*********************************************************************
 * @brief		Initializes the DMA peripheral 
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void DMA_Init(void)
{
	//SYST_ACCESS_EN();
	
	SCU->PER1 &= ~(1UL<<4);
	SCU->PCER1 &= ~(1UL<<4);
	
	SCU->PER1 |= (1UL<<4);
	SCU->PCER1 |= (1UL<<4);
	
	//SYST_ACCESS_DIS();
}

/**********************************************************************
 * @brief		DMA Config Function
 * @param[in]	Struct Type DMA
 * @return		None
 **********************************************************************/
void DMA_Cmd(DMAC_Type* DMAx, DMA_CFG_Type *dma_cfg)
{
	DMAx->CR = 0
	| ((dma_cfg->transcnt&0xFFF)<<16)
	| ((dma_cfg->perisel&0x1F)<<8)
	| ((dma_cfg->bussize&(0x3<<2)))
	| ((dma_cfg->dirsel&(1<<1)))
	;

	
}


/**********************************************************************
 * @brief		DMA Start
 * @param[in]	DMA Type : MDAx
 * @return		None
 **********************************************************************/
void DMA_Start(DMAC_Type* DMAx)
{
	DMAx->SR |= (1<<0);
}


/**********************************************************************
 * @brief		DMA Stop
 * @param[in]	DMA Type : MDAx
 * @return		None
 **********************************************************************/
void DMA_Stop(DMAC_Type* DMAx)
{
	DMAx->SR &= ~(1<<0);
}


/**********************************************************************
 * @brief		DMA Get Status
 * @param[in]	DMA Type : MDAx
 * @return		DMAx Status
 **********************************************************************/
uint32_t DMA_GetStatus(DMAC_Type* DMAx)
{
	return (DMAx->SR);
}


/**********************************************************************
 * @brief		DMA Set Peripheral Address Register
 * @param[in]	DMA Type : MDAx, Peri Address
 * @return		None
 **********************************************************************/
void DMA_SetPAR(DMAC_Type* DMAx, uint32_t peri_addr)
{
	DMAx->PAR = peri_addr;
}


/**********************************************************************
 * @brief		DMA Set Memory Address Register
 * @param[in]	DMA Type : MDAx, Mem Address
 * @return		None
 **********************************************************************/
void DMA_SetMAR(DMAC_Type* DMAx, uint32_t mem_addr)
{
	DMAx->MAR = mem_addr;
}




#endif

