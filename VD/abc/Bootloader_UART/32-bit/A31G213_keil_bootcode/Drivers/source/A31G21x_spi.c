/**
**********************(C) Copyright 2017 ABOV Semiconductor Co., Ltd *******************
* @ File : a31G213_spi.c
* 
* @ Author : Application Team, ABOV Semiconductor Co., Ltd 
*
* @ Version : V1.0
*
* @ Date : Sep, 2017
*
* @ Description 
*   ABOV Semiconductor is supplying this software for use with HART-m310
*   processor. This software contains the confidential and proprietary information
*   of ABOV Semiconductor Co., Ltd ("Confidential Information").
*
*
**************************************************************************************
* DISCLAIMER 
*
* 	THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS  
* 	WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE  
* 	TIME. AS A RESULT, ABOV SEMICONDUCTOR DISCLAIMS ALL LIABILITIES FROM ANY
* 	DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING  
* 	FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE  
* 	CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.  
*
**************************************************************************************
*/


#include <stdint.h>
//#include "A33G52x.h"
//#include "a33g52x_spi.h"
//#include "a33g52x_pcu.h"
//#include "a33g52x_nvic.h"
//#include "a33g52x_timer.h"
#include "a31g213_spi.h"
#include "a31g213_scu.h"

#define INTR_ENABLE 								(1)
#define INTR_DISABLE								(0)
//================================================================================
// SPI PORT (no differentiation between master and slave)
//
//			
//				first index			SPI0, SPI1 
//				second index			SS, SCK, MOSI, MISO
//				third index			base address, pin no, mux info 
//
//================================================================================
//uint32_t	g_SPI_PORT[2][4][3] = {


//	// SPI #0
//	{
//		{PC_BASE, PIN_10, PB10_MUX_SS0},				// SS0
//		{PC_BASE, PIN_11, PB11_MUX_SCK0},			// SCK0
//		{PC_BASE, PIN_12, PB12_MUX_MOSI0}, 			// MOSI0
//		{PC_BASE, PIN_13, PB13_MUX_MISO0}			// MISO0
//	}, 
//	
//	// SPI #1 
//	{
//		{PC_BASE, PIN_8, PD8_MUX_SS1}, 				// SS1 
//		{PC_BASE, PIN_9, PD9_MUX_SCK1}, 				// SCK1 
//		{PC_BASE, PIN_10, PD10_MUX_MOSI1}, 			// MOSI1 
//		{PC_BASE, PIN_11, PD11_MUX_MISO1} 			// MISO1 
//	}

//}; 



uint32_t rxbuftest[20];
uint32_t txbuftest[20];

/**
*********************************************************************************************************
* @ Name : SPI_ConfigureGPIO 
*
* @ Parameters
*		- spi : SPI0, SPI1 
*		- master_slave : SPI_MASTER, SPI_SLAVE
*
*
*********************************************************************************************************
*/
void SPI_ConfigureGPIO (SPI_Type * const spi, int master_slave)
{


////	uint32_t			spi_no; 

////	PCU_Type	*ss_port_addr, *sck_port_addr, *mosi_port_addr, *miso_port_addr; 
////	uint32_t		ss_pin_no, sck_pin_no, mosi_pin_no, miso_pin_no; 
////	uint32_t		ss_mux_info, sck_mux_info, mosi_mux_info, miso_mux_info; 


////	//--------------------------------------------------------------------------------
////	// get spi_no
////	//--------------------------------------------------------------------------------	
////	if (spi == SPI0)
////	{	
////		spi_no = 0; 
////	}
////	else if (spi == SPI1)
////	{
////		spi_no = 1; 
////	}



	//--------------------------------------------------------------------------------
	// setting info 
	//--------------------------------------------------------------------------------	
////	ss_port_addr = (PCU_Type *) g_SPI_PORT[spi_no][0][0]; 			// SS, port base address
////	ss_pin_no = g_SPI_PORT[spi_no][0][1]; 							// SS, pin no
////	ss_mux_info = g_SPI_PORT[spi_no][0][2]; 						// SS, mux info 

////	
////	sck_port_addr = (PCU_Type *) g_SPI_PORT[spi_no][1][0]; 		// SCK, port base address
////	sck_pin_no = g_SPI_PORT[spi_no][1][1]; 						// SCK, pin no
////	sck_mux_info = g_SPI_PORT[spi_no][1][2]; 						// SCK, mux info 	


////	mosi_port_addr = (PCU_Type *) g_SPI_PORT[spi_no][2][0]; 		// MOSI, port base address
////	mosi_pin_no = g_SPI_PORT[spi_no][2][1]; 						// MOSI, pin no
////	mosi_mux_info = g_SPI_PORT[spi_no][2][2]; 						//MOSI, mux info 		


////	miso_port_addr = (PCU_Type *) g_SPI_PORT[spi_no][3][0]; 		// MISO, port base address
////	miso_pin_no = g_SPI_PORT[spi_no][3][1]; 						// MISO, pin no
////	miso_mux_info = g_SPI_PORT[spi_no][3][2]; 						//MISO, mux info 			



	//--------------------------------------------------------------------------------
	// setting 
	//--------------------------------------------------------------------------------	
////	PCU_ConfigureFunction (ss_port_addr, ss_pin_no, ss_mux_info); 
////	PCU_ConfigureFunction (sck_port_addr, sck_pin_no, sck_mux_info); 
////	PCU_ConfigureFunction (mosi_port_addr, mosi_pin_no, mosi_mux_info); 
////	PCU_ConfigureFunction (miso_port_addr, miso_pin_no, miso_mux_info); 

////	if (master_slave == SPI_MASTER)
////	{
////		PCU_Set_Direction_Type(ss_port_addr, ss_pin_no, PnCR_OUTPUT_PUSH_PULL); 
////		PCU_Set_Direction_Type (sck_port_addr, sck_pin_no, PnCR_OUTPUT_PUSH_PULL); 
////		PCU_Set_Direction_Type (mosi_port_addr, mosi_pin_no, PnCR_OUTPUT_PUSH_PULL); 
////		PCU_Set_Direction_Type (miso_port_addr, miso_pin_no, PnCR_INPUT_LOGIC); 

////	}
////	else if (master_slave == SPI_SLAVE)
////	{
////		PCU_Set_Direction_Type (ss_port_addr, ss_pin_no, PnCR_INPUT_LOGIC); 
////		PCU_Set_Direction_Type (sck_port_addr, sck_pin_no, PnCR_INPUT_LOGIC); 
////		PCU_Set_Direction_Type (mosi_port_addr, mosi_pin_no, PnCR_INPUT_LOGIC); 
////		PCU_Set_Direction_Type (miso_port_addr, miso_pin_no, PnCR_OUTPUT_PUSH_PULL); 

////	}


////	PCU_ConfigurePullup (ss_port_addr, ss_pin_no, PnPCR_PULLUP_DISABLE); 
////	PCU_ConfigurePullup (sck_port_addr, sck_pin_no, PnPCR_PULLUP_DISABLE); 
////	PCU_ConfigurePullup (mosi_port_addr, mosi_pin_no, PnPCR_PULLUP_DISABLE); 
////	PCU_ConfigurePullup (miso_port_addr, miso_pin_no, PnPCR_PULLUP_DISABLE); 	
////		


}



/**
*********************************************************************************************************
* @ Name : SPI_Init 
*
* @ Parameters
*		- spi : SPI0, SPI1 
*		- p_config
*			# master_slave		= SPI_MASTER, SPI_SLAVE 
*			# endian				= SPI_MSB_FIRST, SPI_LSB_FIRST
*			# CPHA				= SPI_CPHA_FRONT_HALF, SPI_CPHA_SECOND_HALF 
*			# CPOL				= SPI_CPOL_ACTIVE_LOW, SPI_CPOL_ACTIVE_HIGH
*
*			# bitsize				= SPI_BITSIZE_8_BITS, SPI_BITSIZE_9_BITS, SPI_BITSIZE_16_BITS, SPI_BITSIZE_17_BITS
*			# baudrate			= 0x0001 ~ 0xFF
*
*
*********************************************************************************************************
*/
void SPI_Init (SPI_Type * const spi, SPI_CONFIG * p_config)
{

	uint32_t				reg_val; 
	volatile int		i; 

	
	//----------------------------------------------------------------------------------------
	// config 
	//
	//				@ SP0CR = 0x4000_0804
	//				@ SP1CR = 0x4000_0824
	//
	//----------------------------------------------------------------------------------------
	// If the SPI is in master mode, 
	//
	//		SSMODE			= 1		(non-automatic)
	//		SSOUT			= 1 		(idle-high)
	//		SSMASK			= 0		
	//		SSMO			= 1		(gate opened)
	//		SSPOL			= 0 		(low-active)
	//
	//
	//
	// If the SPI is slave mode
	//
	//		SSMODE			= 0		
	//		SSOUT			= 0 		
	//		SSMASK			= 0		(receive data when SS=active) 
	//		SSMO			= 1		(gate opened)
	//		SSPOL			= 0 		(low-active)
	//
	//-----------------------------------------------------------------------------------------
	reg_val = SPnCR_SSMO; 		// SS gate-opened 

	// SSMODE = 0		(automatic : mater)
	// SSOUT = 0			(N/A : master) 
	// SSMASK = 0 


	reg_val |= (p_config->master_slave == SPI_MASTER) ? SPnCR_MS : 0; 
	reg_val |= (p_config->endian == SPI_MSB_FIRST) ? SPnCR_MSBF : 0; 
	reg_val |= (p_config->SS_polarity == SPI_SS_ACTIVE_HIGH) ? SPnCR_SSPOL : 0; 	// ACTIVE HIGH --> SSOUT_LOW,   ACTIVE LOW --> SSOUT_HIGH 
	reg_val |= (p_config->CPHA == 1) ? SPnCR_CPHA : 0; 
	reg_val |= (p_config->CPOL == 1) ? SPnCR_CPOL : 0; 
	reg_val |= SPnCR_BITSZ_VAL(p_config->bitsize); 


	spi->CR = reg_val;


	
		
	//----------------------------------------------------------------------------------------
	// baud rate 
	//
	//				@ SP0BR = 0x4000_080C
	//				@ SP1BR = 0x4000_082C
	//
	//----------------------------------------------------------------------------------------	
	spi->BR = SPnBR_VAL(p_config->baudrate);


	//----------------------------------------------------------------------------------------
	// timing 
	//
	//				@ SP0CST = 0x4000_0814
	//				@ SP1CST = 0x4000_0834
	//
	//----------------------------------------------------------------------------------------	
	reg_val = SPnLR_STL_VAL(5)|SPnLR_BTL_VAL(255)|SPnLR_SPL_VAL(5); 
	spi->LR = reg_val;




}


/**
*********************************************************************************************************
* @ Name : SPI_Enable
*
* @ Parameters
*		- spi : SPI0, SPI1 
*
*
*********************************************************************************************************
*/
void SPI_Enable (SPI_Type * const spi)
{

	//----------------------------------------------------------------------------------------
	// SPnEN
	//
	//				@ SP0EN	= 0x4000_0810
	//				@ SP1EN = 0x4000_0830
	//
	//----------------------------------------------------------------------------------------
	spi->EN = SPnEN_ENABLE;
	

}



/**
*********************************************************************************************************
* @ Name : SPI_Stop 
*
* @ Parameters
*		- spi : SPI0, SPI1 
*
*
*********************************************************************************************************
*/
void SPI_Stop (SPI_Type * const spi)
{

	//----------------------------------------------------------------------------------------
	// SPnEN
	//
	//				@ SP0EN	= 0x4000_0810
	//				@ SP1EN = 0x4000_0830
	//
	//----------------------------------------------------------------------------------------
	spi->EN = 0;


}



/**
*********************************************************************************************************
* @ Name : SPI_ConfigureInterrupt 
*
* @ Parameters
*		- spi : SPI0, SPI1 
*		- intr_mask : SPnCR_TXDIE, SPnCR_RXDIE, SPnCR_SSCIE, SPnCR_TXIE, SPnCR_RXIE
*		- enable : INTR_ENABLE, INTR_DISABLE 
*
*
*********************************************************************************************************
*/
void SPI_ConfigureInterrupt (SPI_Type * const spi, uint32_t intr_mask, uint32_t enable)
{
	uint32_t		reg_val; 

	//----------------------------------------------------------------------------------------
	// disable interrupt 
	//
	//				@ SP0CR = 0x4000_0804
	//				@ SP1CR = 0x4000_0824
	//
	//----------------------------------------------------------------------------------------
	reg_val = spi->CR;
	reg_val &= ~SPnCR_INTR_MASK;
	spi->CR = reg_val;


	//----------------------------------------------------------------------------------------
	// clear interrupt flag 
	//
	//				@ SP0SR = 0x4000_0808
	//				@ SP1SR = 0x4000_0828
	//
	//----------------------------------------------------------------------------------------
	reg_val = spi->SR;
	
	if (reg_val & SPnSR_TRDY)
	{
		spi->RDR_TDR = 0xFF;				// write dummy data 
	}

	if (reg_val & SPnSR_RRDY)
	{
		do {
			reg_val = spi->RDR_TDR;
			reg_val = spi->SR;

		} while (reg_val & SPnSR_RRDY); 
	}

	if (reg_val & SPnSR_SSDET)
	{
		reg_val = spi->SR;
		reg_val &= ~SPnSR_SSDET;
		spi->SR = reg_val;
	}



	//----------------------------------------------------------------------------------------
	// enable interrupt 
	//
	//				@ SP0SR = 0x4000_0808
	//				@ SP1SR = 0x4000_0828
	//
	//----------------------------------------------------------------------------------------	
	if (enable == INTR_ENABLE)
	{
		reg_val = spi->CR;
		reg_val |= (intr_mask & SPnCR_INTR_MASK);
		spi->CR = reg_val;
	}


}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int 		g_SPI0_index = 0; 
uint32_t	g_SPI0_status[40];

int 	g_SPI1_index; 
uint32_t	g_SPI1_status[40]; 


SPI_BUFFER		sSPI0_Master_Buffer; 
SPI_BUFFER		sSPI0_Slave_Buffer; 

SPI_BUFFER		sSPI1_Master_Buffer; 
SPI_BUFFER		sSPI1_Slave_Buffer; 




/**
************************************************************************************************************
* @ Name: SPI_Init
*
* @ Parameter
*		- spi_no : 0, 1
*
*		- p_config
*			# master_slave		= SPI_MASTER, SPI_SLAVE 
*			# endian				= SPI_MSB_FIRST, SPI_LSB_FIRST
*			# CPHA				= SPI_CPHA_FRONT_HALF, SPI_CPHA_SECOND_HALF 
*			# CPOL				= SPI_CPOL_ACTIVE_LOW, SPI_CPOL_ACTIVE_HIGH
*
*			# bitsize				= SPI_BITSIZE_8_BITS, SPI_BITSIZE_9_BITS, SPI_BITSIZE_16_BITS, SPI_BITSIZE_17_BITS
*			# baudrate			= 0x0001 ~ 0xFF
*
*
* @ Return
*		0 = success
*		1 = fail 
*
*
************************************************************************************************************
*/
int _SPI_Init (int spi_no, SPI_CONFIG * p_config)
{

	SPI_Type				* spi;
	////NVIC_IntrConfig		nvic_config; 
	


	//------------------------------------------------------------------------------------------
	// check spi_no
	//------------------------------------------------------------------------------------------
	if ((spi_no < 0) || (spi_no > 1)) return (1); 


	//------------------------------------------------------------------------------------------
	// get object 
	//------------------------------------------------------------------------------------------
	spi = SPI_Get_Object(spi_no); 

	

	//------------------------------------------------------------------------------------------
	// stop SPI
	//------------------------------------------------------------------------------------------
	//SPI_Stop(spi); 
	
	

	//------------------------------------------------------------------------------------------
	// configure GPIO 
	//------------------------------------------------------------------------------------------
	SPI_ConfigureGPIO(spi, p_config->master_slave); 
	


	//------------------------------------------------------------------------------------------
	// buffer setting 
	//------------------------------------------------------------------------------------------	
	SPI_InitBuffer (spi_no, p_config->master_slave); 


	//------------------------------------------------------------------------------------------
	// mode setting 
	//------------------------------------------------------------------------------------------	
	SPI_Init (spi, p_config); 



	//------------------------------------------------------------------------------------------
	// interrupt (peripheral) 
	//------------------------------------------------------------------------------------------
	SPI_ConfigureInterrupt(spi, (SPnCR_SSCIE|SPnCR_TXIE|SPnCR_RXIE), INTR_ENABLE);


	//------------------------------------------------------------------------------------------
	// interrupt (NVIC)
	//------------------------------------------------------------------------------------------	
////	nvic_config.nIRQ_Number = (IRQ_SPI0+spi_no); 
////	nvic_config.Preemption_Priority= PRIO_SPI0_PREEMPTION; 
////	nvic_config.Subpriority= PRIO_SPI0_SUBPRIORITY; 
////	nvic_config.IntrEnable = INTR_ENABLE; 
////	NVIC_ConfigureInterrupt (NVIC, &nvic_config); 
//   if(spi_no){
//			NVIC_SetPriority(SPI1_IRQn, 3);
//			NVIC_EnableIRQ(SPI1_IRQn);
//	 }
//	 else {
//		NVIC_SetPriority(SPI0_IRQn, 3);
//	  NVIC_EnableIRQ(SPI0_IRQn);
//	 }
  


	//------------------------------------------------------------------------------------------
	// enable 
	//------------------------------------------------------------------------------------------
//	if (p_config->master_slave == SPI_SLAVE)
//	SPI_Enable (spi); 
									

	return (0); 
}




/**
************************************************************************************************************
* @ Name: SPI_Init2
*
* @ Parameter
*		- spi_no : 0, 1
*
*		- p_config
*			# master_slave		= SPI_MASTER, SPI_SLAVE 
*			# endian				= SPI_MSB_FIRST, SPI_LSB_FIRST
*			# CPHA				= SPI_CPHA_FRONT_HALF, SPI_CPHA_SECOND_HALF 
*			# CPOL				= SPI_CPOL_ACTIVE_LOW, SPI_CPOL_ACTIVE_HIGH
*
*			# bitsize				= SPI_BITSIZE_8_BITS, SPI_BITSIZE_9_BITS, SPI_BITSIZE_16_BITS, SPI_BITSIZE_17_BITS
*			# baudrate			= 0x0001 ~ 0xFF
*
*
* @ Return
*		0 = success
*		1 = fail 
*
************************************************************************************************************
*
*		This "Init" function is used for "timer interrupt-based communication." 
*
*
************************************************************************************************************
*/
int _SPI_Init2 (int spi_no, SPI_CONFIG * p_config)
{

	SPI_Type				* spi;
	////NVIC_IntrConfig		nvic_config; 
	


	//------------------------------------------------------------------------------------------
	// check spi_no
	//------------------------------------------------------------------------------------------
	if ((spi_no < 0) || (spi_no > 1)) return (1); 


	//------------------------------------------------------------------------------------------
	// get object 
	//------------------------------------------------------------------------------------------
	spi = SPI_Get_Object(spi_no); 

	

	//------------------------------------------------------------------------------------------
	// stop SPI
	//------------------------------------------------------------------------------------------
	//SPI_Stop(spi); 
	
	

	//------------------------------------------------------------------------------------------
	// configure GPIO 
	//------------------------------------------------------------------------------------------
	SPI_ConfigureGPIO(spi, p_config->master_slave); 



	


	//------------------------------------------------------------------------------------------
	// buffer setting 
	//------------------------------------------------------------------------------------------	
	SPI_InitBuffer (spi_no, p_config->master_slave); 


	//------------------------------------------------------------------------------------------
	// mode setting 
	//------------------------------------------------------------------------------------------	
	SPI_Init (spi, p_config); 



	//------------------------------------------------------------------------------------------
	// interrupt (peripheral) 
	//------------------------------------------------------------------------------------------
	if (p_config->master_slave == SPI_SLAVE)
	{
		SPI_ConfigureInterrupt(spi, (SPnCR_SSCIE|SPnCR_TXIE|SPnCR_RXIE), INTR_ENABLE);
	}



	//------------------------------------------------------------------------------------------
	// interrupt (NVIC)
	//------------------------------------------------------------------------------------------	
	if (p_config->master_slave == SPI_SLAVE)
	{
//		nvic_config.nIRQ_Number = (IRQ_SPI0+spi_no); 
//		nvic_config.Preemption_Priority= PRIO_SPI0_PREEMPTION; 
//		nvic_config.Subpriority= PRIO_SPI0_SUBPRIORITY; 
//		nvic_config.IntrEnable = INTR_ENABLE; 
//		NVIC_ConfigureInterrupt (NVIC, &nvic_config); 
		
        /* preemption = 1, sub-priority = 1 */
   if(spi_no){
			NVIC_SetPriority(SPI21_IRQn, 3);
			NVIC_EnableIRQ(SPI21_IRQn);
	 }
	 else {
		NVIC_SetPriority(SPI20_IRQn, 3);
	  NVIC_EnableIRQ(SPI20_IRQn);
	 }
  
		
	}


	//------------------------------------------------------------------------------------------
	// enable 
	//------------------------------------------------------------------------------------------
	if (p_config->master_slave == SPI_SLAVE) SPI_Enable (spi); 
									

	return (0); 
}


/**
************************************************************************************************************
* @ Name: SPI_Get_Object
*
* @ Parameter
*		- spi_no : 0, 1
*
* @ Return
*		SPI object 
*
*
************************************************************************************************************
*/
SPI_Type * SPI_Get_Object (int spi_no)
{

	SPI_Type * p_obj; 


	switch (spi_no)
	{
	case 0:
		p_obj = SPI20; 
		break; 

	case 1: 
		p_obj = SPI21; 
		break; 

	default: 
		p_obj = (SPI_Type *) 0; 
		break; 
	}


	return (p_obj); 
}


/**
************************************************************************************************************
* @ Name: SPI_InitBuffer
*
* @ Parameter
*		- spi_no : 0, 1
*		- master_slave : SPI_MASTER, SPI_SLAVE
*
* @ Return
*		0 = success
*		1 = fail 
*
*
************************************************************************************************************
*/
int SPI_InitBuffer (int spi_no, int master_slave)
{

	SPI_BUFFER			*p_SPI_buffer; 
	int					i; 
	int					result; 


	//------------------------------------------------------------------------------------------
	// get base address
	//------------------------------------------------------------------------------------------
	p_SPI_buffer = SPI_Get_BufferAddr (spi_no, master_slave, &result); 


	if (result == SPI_CHANNEL_NOT_SUPPORTED)
	{
		return (1);
	}



	//------------------------------------------------------------------------------------------
	// init buffer variables 
	//------------------------------------------------------------------------------------------
	p_SPI_buffer->u16TxRxState = SPI_TXRX_STATE_IDLE;

	p_SPI_buffer->u16RxBuffer_HeadIndex = 0; 
	p_SPI_buffer->u16RxBuffer_TailIndex = 0;
	p_SPI_buffer->u16TxBuffer_HeadIndex = 0; 
	p_SPI_buffer->u16TxBuffer_TailIndex = 0; 

	for (i=0; i<SPI_MAX_RX_BUFFER; i++)
	{
		p_SPI_buffer->u32RxBuffer[i] = 0; 
	}


	for (i=0; i<SPI_MAX_TX_BUFFER; i++)
	{
		p_SPI_buffer->u32TxBuffer[i] = 0; 
	}	


	return (0); 
	
}




/**
************************************************************************************************************
* @ Name: SPI_Get_BufferAddr
*
* @ Parameter
*		- spi_no : 0, 1
*		- master_slave : SPI_MASTER, SPI_SLAVE
*		- *p_result: &result 
*
* @ Return
*		buffer address
*
*
************************************************************************************************************
*/
SPI_BUFFER* SPI_Get_BufferAddr (int spi_no, int master_slave, int *p_result)
{

	SPI_BUFFER		*p_SPI_buffer; 
	int				result = SPI_CHANNEL_SUPPORTED; 


	switch (spi_no)
	{
	case 0:
		if (master_slave == SPI_MASTER) p_SPI_buffer = &sSPI0_Master_Buffer; 
		else if (master_slave == SPI_SLAVE) p_SPI_buffer = &sSPI0_Slave_Buffer; 
		else result = SPI_CHANNEL_NOT_SUPPORTED; 

		break; 

	case 1: 
		if (master_slave == SPI_MASTER) p_SPI_buffer = &sSPI1_Master_Buffer; 
		else if (master_slave == SPI_SLAVE) p_SPI_buffer = &sSPI1_Slave_Buffer; 
		else result = SPI_CHANNEL_NOT_SUPPORTED; 

		break; 

	default:
		result = SPI_CHANNEL_NOT_SUPPORTED; 
		break; 
	}

	*p_result = result; 

	return (p_SPI_buffer); 

}







/**
************************************************************************************************************
* @ Name: SPI_Write
*
* @ Parameter
*		- spi_no : 0, 1
*		- data : data to transmit 
*
* @ Return 
*		none 
*
************************************************************************************************************
*/
int SPI_Write (int spi_no, uint32_t data)
{

	SPI_Type 		* spi; 

	
	//------------------------------------------------------------------------------------------
	// get SPI object 
	//------------------------------------------------------------------------------------------
	spi = SPI_Get_Object(spi_no); 
	
	if (spi == (SPI_Type *) 0)
	{
		return (SPI_TX_BUFFER_ERROR_WRONG_CHANNEL);
	}
	
	//spi->EN = 0;
	spi->RDR_TDR = data;

	spi->EN = 1;
	rxbuftest[0] = SPI21->RDR_TDR;
  txbuftest[0] = data;

	return (0); 


}




int SPI_Write_Data (int spi_no, int master_slave, uint32_t *p_write_buf, uint32_t data_count)
{

	SPI_BUFFER			*p_SPI_buffer; 
	SPI_Type				*spi; 
	int					i; 
	int					status;


	//------------------------------------------------------------------------------------------
	// get SPI object 
	//------------------------------------------------------------------------------------------
	spi = SPI_Get_Object(spi_no); 
	
	if (spi == (SPI_Type *) 0)
	{
		return (SPI_TX_BUFFER_ERROR_WRONG_CHANNEL);
	}
	


	//------------------------------------------------------------------------------------------
	// get base address
	//------------------------------------------------------------------------------------------
	p_SPI_buffer = SPI_Get_BufferAddr (spi_no, master_slave, &status); 

	if (status == SPI_CHANNEL_NOT_SUPPORTED)
	{
		return (SPI_TX_BUFFER_ERROR_WRONG_CHANNEL);
	}	



	//------------------------------------------------------------------------------------------
	// examine if SPI (TX) is idle
	//------------------------------------------------------------------------------------------
	for (i=0; i<10000; i++)
	{
		if (p_SPI_buffer->u16TxRxState == SPI_TXRX_STATE_IDLE) break; 
	}


	if (i == 10000)
	{
		return (SPI_TX_BUFFER_ERROR_WAIT_TIMEOUT); 
	}



	//------------------------------------------------------------------------------------------
	// copy data from write-buffer to SPI-buffer 
	//------------------------------------------------------------------------------------------
	for (i=0; i<data_count; i++)
	{

		p_SPI_buffer->u32TxBuffer[i] = *(p_write_buf + i); 
	}

	p_SPI_buffer->u16TxBuffer_HeadIndex = 0; 
	p_SPI_buffer->u16TxBuffer_TailIndex = data_count; 




	//------------------------------------------------------------------------------------------
	// update state & init SCLK count 
	//------------------------------------------------------------------------------------------
	p_SPI_buffer->u16TxRxState = SPI_TXRX_STATE_BUSY; 
	p_SPI_buffer->u16SCLK_count = 0; 



	//------------------------------------------------------------------------------------------
	// start transmission  
	//------------------------------------------------------------------------------------------
	////TIMER_Start(9); 


	return (SPI_TX_BUFFER_SUCCESS); 
	
}




/**
************************************************************************************************************
* @ Name: SPI_Read_Data
*
* @ Parameter
*		- spi_no : 0, 1
*		- master_slave : SPI_MASTER, SPI_SLAVE
*		- *p_read_buf : &data_buf[n]
*		- data_count: max. 20
*
* @ Return
*		SPI_TX_BUFFER_ERROR_WRONG_CHANNEL 
*		SPI_TX_BUFFER_ERROR_WAIT_TIMEOUT
*
*
************************************************************************************************************
*/
int SPI_Read_Data (int spi_no, int master_slave, uint32_t *p_read_buf, uint32_t data_count)
{
	SPI_Type		* spi;
	SPI_BUFFER		* p_SPI_buffer; 

	int				i;
	int				status; 
	int				read_count = 0; 


	//------------------------------------------------------------------------------------------
	// get SPI object 
	//------------------------------------------------------------------------------------------
	spi = SPI_Get_Object(spi_no); 

	if (spi == (SPI_Type *) 0)
	{
		return (SPI_RX_BUFFER_ERROR_WRONG_CHANNEL);
	}


	//------------------------------------------------------------------------------------------
	// get base address
	//------------------------------------------------------------------------------------------
	p_SPI_buffer = SPI_Get_BufferAddr (spi_no, master_slave, &status); 

	if (status == SPI_CHANNEL_NOT_SUPPORTED)
	{
		return (SPI_RX_BUFFER_ERROR_WRONG_CHANNEL);
	}	


	//------------------------------------------------------------------------------------------
	// get received data
	//------------------------------------------------------------------------------------------
	for (i=0; i<data_count; i++)
	{
		if (p_SPI_buffer->u16RxBuffer_HeadIndex != p_SPI_buffer->u16RxBuffer_TailIndex)
		{
		
			*(p_read_buf + i) = p_SPI_buffer->u32RxBuffer[p_SPI_buffer->u16RxBuffer_HeadIndex++]; 

			if (p_SPI_buffer->u16RxBuffer_HeadIndex >= SPI_MAX_RX_BUFFER) 
				p_SPI_buffer->u16RxBuffer_HeadIndex = 0; 

			read_count++; 
		}
		else
		{
			*(p_read_buf + i) = 0; 
		}
	}


	return (read_count); 


}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/**
************************************************************************************************************
* @ Name: SPI0_Master_ISR
*
* @ Parameter
*		none 
* @ Return
*		0 		in SPI transmission
*		1 		done 
*
* @ Function
*		interrupt handler 
*
************************************************************************************************************
*/
int SPI0_Master_ISR (void)
{

	SPI_Type 			*spi = SPI20; 

	volatile uint32_t		control, status;
	volatile uint32_t		data; 
	uint32_t				next_index; 
	SPI_BUFFER			* spi_buffer = &sSPI0_Master_Buffer; 



	//-----------------------------------------------------------------------------------
	// read SP0SR
	//-----------------------------------------------------------------------------------
	control = spi->CR;
	status = spi->SR;

	
	//-------------------------------------------------------------------------------------
	// save status++
	//-------------------------------------------------------------------------------------
	if (g_SPI0_index < 40) g_SPI0_status[g_SPI0_index++] = status; 

	

	//-----------------------------------------------------------------------------------
	// clear flags
	//-----------------------------------------------------------------------------------
	spi->SR = 0;



	//-----------------------------------------------------------------------------------
	// START 
	//
	//				SSDET = 1
	//				SSON = 1
	//				
	//-----------------------------------------------------------------------------------
	if ((status & (SPnSR_SSDET|SPnSR_SSON)) == (SPnSR_SSDET|SPnSR_SSON))
	{

	}


	//-----------------------------------------------------------------------------------
	// END 
	//
	//				SSDET = 1
	//				SSON = 0
	//				
	//-----------------------------------------------------------------------------------
	if ((status & (SPnSR_SSDET|SPnSR_SSON)) == SPnSR_SSDET)
	{

	}


	//-----------------------------------------------------------------------------------
	// receive data 
	//
	//			RRDY = 1 
	//
	//-----------------------------------------------------------------------------------
	if (status & SPnSR_RRDY)
	{
		data = spi->RDR_TDR;

		next_index = spi_buffer->u16RxBuffer_TailIndex + 1; 
		if (next_index >= SPI_MAX_RX_BUFFER) next_index = 0; 

		if (next_index != spi_buffer->u16RxBuffer_HeadIndex)
		{
			spi_buffer->u32RxBuffer[spi_buffer->u16RxBuffer_TailIndex] = data; 
			rxbuftest[spi_buffer->u16RxBuffer_TailIndex] = data; //test
			spi_buffer->u16RxBuffer_TailIndex = next_index; 

		}


		//------------------------------------------------------------------------------
		// turn off SPI 
		//------------------------------------------------------------------------------
		spi_buffer->u16SCLK_count++; 

		//####################################################
		//# Because of timing, this code is transferred to the SPnSR_TRDY routine
		//####################################################		
		/*
		if (spi_buffer->u16SCLK_count >=  spi_buffer->u16TxBuffer_TailIndex)
		{
			SPI0->EN = 0;
		}
		*/

	}

	
	
	//-----------------------------------------------------------------------------------
	// send data 
	//
	//				TRDY = 1
	//				
	//-----------------------------------------------------------------------------------
	if (status & SPnSR_TRDY)
	{
		if (spi_buffer->u16TxBuffer_HeadIndex < spi_buffer->u16TxBuffer_TailIndex)
		{
			data = spi_buffer->u32TxBuffer[spi_buffer->u16TxBuffer_HeadIndex++]; 
			txbuftest[spi_buffer->u16TxBuffer_HeadIndex] = data;    //test
			spi->RDR_TDR = data;
			

		}
		else 
		{
			spi->RDR_TDR = 0xAA;
		}

		//------------------------------------------------------------------------------
		// turn off SPI 
		//------------------------------------------------------------------------------
		if (spi_buffer->u16SCLK_count >=  spi_buffer->u16TxBuffer_TailIndex)
		{
			spi_buffer->u16TxRxState = SPI_TXRX_STATE_IDLE; 
			spi->EN = 0;
		}


	}
	


	//-------------------------------------------------------------------------------------
	// save status--
	//-------------------------------------------------------------------------------------
	status = spi->SR;
	if (g_SPI0_index < 40) g_SPI0_status[g_SPI0_index++] = (status | 0xF0000000UL); 

	
	return (0);
}






/**
************************************************************************************************************
* @ Name: SPI1_Slave_ISR
*
* @ Parameter
*		none 
*
* @ Function
*		interrupt handler 
*
************************************************************************************************************
*/
void SPI1_Slave_ISR (void)
{

	SPI_Type			* spi = SPI21; 
	SPI_BUFFER		* spi_buffer = &sSPI1_Slave_Buffer; 
	
	uint32_t			status; 
	uint32_t			send_data, rcv_data; 
	uint32_t			next_index; 
	uint32_t			reg_val;
	
	//------------------------------------------------------------------------------------------
	// get status
	//
	//				@ SP1SR = 0x4000_0828
	//------------------------------------------------------------------------------------------
	status = spi->SR;

	// MONITOR 
	if (g_SPI1_index < 40) g_SPI1_status[g_SPI1_index++] = status; 



	//------------------------------------------------------------------------------------------
	// SSDET
	//
	//------------------------------------------------------------------------------------------
	if (status & SPnSR_SSDET)
	{
		status &= ~SPnSR_SSDET; 
		spi->SR = status;


		if (status & SPnSR_SSON)
		{

			//------------------------------------------------------------------------
			//	WHEN SSDET INTERRUPT ARISES
			//
			//			SPnEN = 0
			//			SPnCR : buffer clear
			//			SPnTDR : load the first data
			//			SPnEN = 1 
			//------------------------------------------------------------------------

			// SPnEN = 0 
			spi->EN = 0;

			// SPnCR : buffer clear 
			reg_val = SPI21->CR;
			reg_val |= SPnCR_TXBC;
			SPI21->CR = reg_val;
			

			// SPnTDR : load the first data 
			if (spi_buffer->u16TxBuffer_HeadIndex < spi_buffer->u16TxBuffer_TailIndex)
			{
				send_data = spi_buffer->u32TxBuffer[spi_buffer->u16TxBuffer_HeadIndex++]; 
			}
			else
			{
				send_data = 0; 
			}

			spi->RDR_TDR = send_data;
			
			spi_buffer->u16TxRxState = SPI_TXRX_STATE_BUSY;

			// SPnEN = 1 
			spi->EN = 1;
			
		}
		else
		{
			//------------------------------------------------------------------------
			// SPnSR_SSDET && !SPnSR_SSON
			//
			//				SS : active --> idle 
			//------------------------------------------------------------------------		
			spi_buffer->u16TxRxState = SPI_TXRX_STATE_IDLE;
		}
	}



	//------------------------------------------------------------------------------------------
	// RRDY
	//
	//------------------------------------------------------------------------------------------
	if (status & SPnSR_RRDY)
	{
		
		rcv_data = spi->RDR_TDR;
		

		//------------------------------------------------------------------------------------------
		// search location to deposit data 
		//------------------------------------------------------------------------------------------
		next_index = spi_buffer->u16RxBuffer_TailIndex + 1; 

		if (next_index >= SPI_MAX_RX_BUFFER)
		{
			next_index = 0; 
		}


		//------------------------------------------------------------------------------------------
		// deposit data if space is available 
		//------------------------------------------------------------------------------------------
		if (next_index != spi_buffer->u16RxBuffer_HeadIndex)
		{
			spi_buffer->u32RxBuffer[spi_buffer->u16RxBuffer_TailIndex] = rcv_data; 
			spi_buffer->u16RxBuffer_TailIndex = next_index; 
		}
	}


	//------------------------------------------------------------------------------------------
	// TRDY
	//
	//------------------------------------------------------------------------------------------
	if (status & SPnSR_TRDY)
	{

		if (spi_buffer->u16TxBuffer_HeadIndex < spi_buffer->u16TxBuffer_TailIndex)
		{
			send_data = spi_buffer->u32TxBuffer[spi_buffer->u16TxBuffer_HeadIndex++]; 
		}
		else
		{
			send_data = 0; 
		}

		spi->RDR_TDR = send_data;
		
	}



	//------------------------------------------------------------------------------------------
	// get status again 
	//
	//				@ SP1SR = 0x4000_0828
	//------------------------------------------------------------------------------------------
	status = spi->SR;	


	//------------------------------------------------------------------------------------------
	// TRDY2 
	//
	//------------------------------------------------------------------------------------------
	#if 0
	if (status & SPnSR_TRDY)
	{

		if (spi_buffer->u16TxBuffer_HeadIndex < spi_buffer->u16TxBuffer_TailIndex)
		{
			send_data = spi_buffer->u32TxBuffer[spi_buffer->u16TxBuffer_HeadIndex++]; 
		}
		else
		{
			send_data = 0; 
		}

		spi->RDR_TDR = send_data;

		//------------------------------------------------------------------------------------------
		// get status again 
		//
		//				@ SP1SR = 0x4000_0828
		//------------------------------------------------------------------------------------------
		status = spi->SR;
		
	}
	#endif 



	// MONITOR
	if (g_SPI1_index < 40) g_SPI1_status[g_SPI1_index++] = (status | (0xFFUL<<24)); 	


}


