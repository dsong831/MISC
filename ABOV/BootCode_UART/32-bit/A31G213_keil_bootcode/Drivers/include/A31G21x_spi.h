/**********************************************************************
* @file		a31g213_spi.h
* @brief	      spi 10/11 function header file
* @version	1.0
* @date: 2016-11-23
* @author	ABOV M team
*
* Copyright(C) 2016, ABOV Semiconductor. All rights reserved.
**********************************************************************/



#include "a31g213.h"


//typedef struct {

//	REGISTER_T		SPnTDR_RDR;		// offset = 0x0000, R/W
//	REGISTER_T		SPnCR;				// offset = 0x0004, R/W
//	REGISTER_T		SPnSR;				// offset = 0x0008, R/W
//	REGISTER_T		SPnBR;				// offset = 0x000C, R/W

//	REGISTER_T		SPnEN; 				// offset = 0x0010, R/W
//	REGISTER_T 		SPnCST; 			// offset=0x0010, R/W


//} SPI_Types;




//==========================================================================
//
//	D E F I N A T I O N S
//
//==========================================================================
#define SPI_MASTER								(1)
#define SPI_SLAVE								(0)



//==========================================================================
//
//	S T R U C T U R E S 
//
//==========================================================================

//==========================================================================
//
//			SS_enable				= SSMO
//			SS_auto_manual			= SSMOD
//			SS_polarity				= SSPOL
//			SS_mask					= SSMASK
//
//			msb_lsb_first				= MSBF
//			SCK_ploarity				= CPOL
//			SCK_DATA_phase			= CPHA
//			bit_size					= BITSZ
//
//==========================================================================
#define SPI_SS_ENABLE							(1)
#define SPI_SS_DISABLE							(0)

#define SPI_SS_AUTOMATIC						(1)
#define SPI_SS_MANUAL							(0)

#define SPI_SS_ACTIVE_HIGH						(1)
#define SPI_SS_ACTIVE_LOW						(0)

#define SPI_SS_MASKING							(1)
#define SPI_SS_NOT_MASKING						(0)

#define SPI_MSB_FIRST							(1)
#define SPI_LSB_FIRST							(0)

#define SPI_BITSIZE_8_BITS						(0)
#define SPI_BITSIZE_9_BITS						(1)
#define SPI_BITSIZE_16_BITS						(2)
#define SPI_BITSIZE_17_BITS						(3)



typedef struct {

	uint8_t				master_slave; 
	uint8_t				endian;
	uint8_t				CPHA; 
	uint8_t				CPOL;

	uint8_t				SS_polarity; 
	uint8_t				bitsize; 		
	uint16_t				baudrate; 

} SPI_CONFIG; 




//==========================================================================
// 	SPnTDR_RDR
//		
//				@ SP0TDR, SP0RDR = 0x4000_0800
//				@ SP1TDR, SP1RDR = 0x4000_0820
//
//==========================================================================


//==========================================================================
// 	SPnCR
//	
//				@ SP0CR = 0x4000_0804
//				@ SP1CR = 0x4000_0824
//
//==========================================================================
#if 0
#define SPnCR_TXBC								(0x0001UL<<20)
#define SPnCR_RXBC								(0x0001UL<<19)


#define SPnCR_SSCIE								(0x0001UL<<15)
#define SPnCR_TXIE								(0x0001UL<<14)
#define SPnCR_RXIE								(0x0001UL<<13)
#define SPnCR_INTR_MASK						(0x0007UL<<13)

#define SPnCR_SSMODE							(0x0001UL<<12)
#define SPnCR_SSOUT								(0x0001UL<<11)

#define SPnCR_LBE								(0x0001UL<<10)
#define SPnCR_SSMASK							(0x0001UL<<9)
#define SPnCR_SSMO								(0x0001UL<<8)
#define SPnCR_SSPOL								(0x0001UL<<7)


#define SPnCR_MS								(0x0001UL<<5)
#define SPnCR_MSBF								(0x0001UL<<4)

#define SPnCR_CPHA								(0x0001UL<<3)
#define SPnCR_CPOL								(0x0001UL<<2)

#define SPnCR_BITSZ_8_BITS						(0x0000UL<<0)
#define SPnCR_BITSZ_9_BITS						(0x0001UL<<0)
#define SPnCR_BITSZ_16_BITS					(0x0002UL<<0)
#define SPnCR_BITSZ_17_BITS					(0x0003UL<<0)
#define SPnCR_BITSZ_VAL(n)						(((n)&0x0003UL)<<0)
#define SPnCR_BITSZ_MASK						(0x0003UL<<0)

#endif 

#define SPnCR_TXBC								(0x0001UL<<20)
#define SPnCR_RXBC								(0x0001UL<<19)

#define SPnCR_DTXIE								(0x0001UL<<18)   //ADD
#define SPnCR_DRXIE								(0x0001UL<<17)   //ADD


#define SPnCR_SSCIE								(0x0001UL<<16)
#define SPnCR_TXIE								(0x0001UL<<15)
#define SPnCR_RXIE								(0x0001UL<<14)
#define SPnCR_INTR_MASK						(0x0007UL<<14)

#define SPnCR_SSMODE							(0x0001UL<<13)
#define SPnCR_SSOUT								(0x0001UL<<12)

#define SPnCR_LBE								(0x0001UL<<11)
#define SPnCR_SSMASK							(0x0001UL<<10)
#define SPnCR_SSMO								(0x0001UL<<9)
#define SPnCR_SSPOL								(0x0001UL<<8)


#define SPnCR_MS								(0x0001UL<<5)
#define SPnCR_MSBF								(0x0001UL<<4)

#define SPnCR_CPHA								(0x0001UL<<3)
#define SPnCR_CPOL								(0x0001UL<<2)

#define SPnCR_BITSZ_8_BITS						(0x0000UL<<0)
#define SPnCR_BITSZ_9_BITS						(0x0001UL<<0)
#define SPnCR_BITSZ_16_BITS					(0x0002UL<<0)
#define SPnCR_BITSZ_17_BITS					(0x0003UL<<0)
#define SPnCR_BITSZ_VAL(n)						(((n)&0x0003UL)<<0)
#define SPnCR_BITSZ_MASK						(0x0003UL<<0)


//==========================================================================
// 	SPnSR
//	
//				@ SP0SR = 0x4000_0808
//				@ SP1SR = 0x4000_0828
//
//==========================================================================
#define SPnSR_TXDMAF								(0x0001UL<<8)   //ADD
#define SPnSR_RXDMAF								(0x0001UL<<9)   //ADD
#define SPnSR_SBUSY							  	(0x0001UL<<7)     //ADD

#define SPnSR_SSDET								(0x0001UL<<6)
#define SPnSR_SSON								(0x0001UL<<5)
#define SPnSR_OVRF								(0x0001UL<<4)

#define SPnSR_UDRF								(0x0001UL<<3)
//#define SPnSR_SBUSY								(0x0001UL<<2)
#define SPnSR_TXIDLE								(0x0001UL<<2)





#define SPnSR_TRDY								(0x0001UL<<1)
#define SPnSR_RRDY								(0x0001UL<<0)




//==========================================================================
// 	SPnBR
//	
//				@ SP0BR = 0x4000_080C
//				@ SP1BR = 0x4000_082C
//
//==========================================================================
#define SPnBR_VAL(n)							(((n)&0x00FFUL)<<0)




//==========================================================================
// 	SPnEN
//	
//				@ SP0EN = 0x4000_0810
//				@ SP1EN = 0x4000_0830
//
//==========================================================================
#define SPnEN_ENABLE							(0x0001UL<<0)



//==========================================================================
// 	SPnLR
//	
//				@ SP0LR = 0x4000_0814
//				@ SP1LR = 0x4000_0834
//
//==========================================================================
#define SPnLR_SPL_VAL(n)						(((n)&0x001FUL)<<16)   //10
#define SPnLR_SPL_MASK 							(0x001FUL<<16)   //10

#define SPnLR_BTL_VAL(n)						(((n)&0x001FUL)<<8) //5
#define SPnLR_BTL_MASK 							(0x001FUL<<8)  //5


#define SPnLR_STL_VAL(n)						(((n)&0x001FUL)<<0)
#define SPnLR_STL_MASK 							(0x001FUL<<0)






//============================================================================
// 	
//	D E F I N A T I O N S 
//
//============================================================================
#define SPI_MAX_RX_BUFFER					(20)
#define SPI_MAX_TX_BUFFER					(20)

#define SPI_CHANNEL_SUPPORTED				(1)
#define SPI_CHANNEL_NOT_SUPPORTED			(0)



//------------------------------------------------------------------------------------
// definitions for TX/RX state  
//------------------------------------------------------------------------------------
#define SPI_TXRX_STATE_IDLE					(0)
#define SPI_TXRX_STATE_BUSY					(1)




//------------------------------------------------------------------------------------
// definitions for read/write function 
//------------------------------------------------------------------------------------
#define SPI_TX_BUFFER_SUCCESS				(0)
#define SPI_TX_BUFFER_ERROR_WRONG_CHANNEL	(1)
#define SPI_TX_BUFFER_ERROR_WAIT_TIMEOUT	(2)

#define SPI_RX_BUFFER_SUCCESS				(0)
#define SPI_RX_BUFFER_ERROR_WRONG_CHANNEL	(1)
#define SPI_RX_BUFFER_ERROR_EMPTY			(2)





#define SPI_TXDATA_END_MARK					(0xFFFFFFFF)


//============================================================================
// 	
//	S T R U C T U R E S
//
//============================================================================
typedef struct {
	uint16_t		u16TxRxState; 
	uint16_t		u16SCLK_count; 
	
	uint16_t		u16RxBuffer_HeadIndex;
	uint16_t 		u16RxBuffer_TailIndex;
	
	uint16_t		u16TxBuffer_HeadIndex;
	uint16_t 		u16TxBuffer_TailIndex;
	
	uint32_t		u32RxBuffer[SPI_MAX_RX_BUFFER];
	uint32_t		u32TxBuffer[SPI_MAX_TX_BUFFER]; 

	
} SPI_BUFFER; 





//==========================================================================
// 
//		F U N C T I O N    D E C L A R A T I O N S 
//
//==========================================================================
void SPI_ConfigureGPIO (SPI_Type * const spi, int master_slave); 
void SPI_Init (SPI_Type * const spi, SPI_CONFIG * p_config); 
void SPI_Enable (SPI_Type * const spi); 
void SPI_Stop (SPI_Type * const spi); 
void SPI_ConfigureInterrupt (SPI_Type * const spi, uint32_t intr_mask, uint32_t enable); 




////////////////////////////////////////////////////////////////////////////////////////////
int _SPI_Init (int spi_no, SPI_CONFIG * p_config); 
int _SPI_Init2 (int spi_no, SPI_CONFIG * p_config); 
SPI_Type* SPI_Get_Object (int spi_no); 

int SPI_InitBuffer (int spi_no, int master_slave); 
SPI_BUFFER* SPI_Get_BufferAddr (int spi_no, int master_slave, int *p_result); 

int SPI_Write (int spi_no, uint32_t data); 
int SPI_Write_Data (int spi_no, int master_slave, uint32_t *p_write_buf, uint32_t data_count); 
int SPI_Read_Data (int spi_no, int master_slave, uint32_t *p_read_buf, uint32_t data_count); 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int SPI0_Master_ISR (void); 
int SPI0_Master_Transmit_Receive_on_Timer_ISR (void); 
void SPI0_Slave_ISR (void); 

int SPI1_Master_ISR (void); 
void SPI1_Slave_ISR (void); 

