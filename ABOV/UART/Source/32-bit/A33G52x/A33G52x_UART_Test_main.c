
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "A33G52x.h"
#include "a33g52x_gpio.h"
#include "a33g52x_pcu.h"
#include "a33g52x_uart.h"
#include "a33g52x_nvic.h"
#include "console.h"
#include "slib.h"

//#define	USE_UART_INTERRUPT


void PCU_Init(void);
void mainloop(void);
void Disp_MainMenu(void);
void delay (void);

void gpioBlinky(void);
void gpioInput(void);
void gpioExtInt(void);


uint32_t 	period;		// for Systick period 1ms
uint32_t		flag;
uint32_t		cnt = 0;



/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

UART_BUFFER	gt_Uart0Buffer;
UART_BUFFER	gt_Uart1Buffer;
UART_BUFFER	gt_Uart2Buffer;
UART_BUFFER	gt_Uart3Buffer;

/**
* @brief
* @param   
* @return
*/
UART_Type* HAL_UART_Get_Object(uint8_t uart_no)
{
	UART_Type* UARTx;

	/* Set Uart Channel */
	switch(uart_no)
	{
		case 0:
			UARTx = UART0;
			break;
		case 1:
			UARTx = UART1;
			break;
		case 2:
			UARTx = UART2;
			break;
		case 3:
		default:
			UARTx = UART3;
			break;
	}

	return (UARTx);
}

/**
* @brief
* @param   
* @return
*/
void HAL_Port_Init(void)
{
	/* UART1 Port Setting (PD13-TXD1 / PD12-RXD1) */
	PCU_ConfigureFunction (PCD, PIN_12, PD12_MUX_RXD1);
	PCU_ConfigureFunction (PCD, PIN_13, PD13_MUX_TXD1); 	
	PCU_Set_Direction_Type (PCD, PIN_12, PnCR_INPUT_LOGIC); 
	PCU_Set_Direction_Type (PCD, PIN_13, PnCR_OUTPUT_PUSH_PULL); 	
	PCU_ConfigurePullup_Pulldown (PCD, PIN_12, 0, PnPCR_PULLUPDOWN_DISABLE); 
	PCU_ConfigurePullup_Pulldown (PCD, PIN_13, 0, PnPCR_PULLUPDOWN_DISABLE);
}

/**
* @brief
* @param   
* @return
*/
UART_BUFFER* HAL_Uart_GetBufferBaseAddr(uint8_t uart_no)
{
	UART_BUFFER* tp_UartBuffer;
	
	/* Get Buffer BaseAddress */
	switch(uart_no)
	{
		case 0:
			tp_UartBuffer = &(gt_Uart0Buffer);
			break;
		case 1:
			tp_UartBuffer = &(gt_Uart1Buffer);
			break;
		case 2:
			tp_UartBuffer = &(gt_Uart2Buffer);
			break;
		case 3:
		default:
			tp_UartBuffer = &(gt_Uart3Buffer);
			break;
	}

	return (tp_UartBuffer);
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_BufferInit(uint8_t uart_no)
{
	UART_BUFFER* tp_UartBuffer;
	uint8_t i;

	/* Get Buffer BaseAddress */
	tp_UartBuffer = HAL_Uart_GetBufferBaseAddr(uart_no);

	/* Set Buffer Parameter */
	tp_UartBuffer->RxState = UART_RX_STATE_IDLE;
	tp_UartBuffer->TxState = UART_TX_STATE_IDLE;
	tp_UartBuffer->RxBuffer_HeadIndex = 0;
	tp_UartBuffer->TxBuffer_HeadIndex = 0;
	tp_UartBuffer->RxBuffer_TailIndex = 0;
	tp_UartBuffer->TxBuffer_TailIndex = 0;

	for(i=0; i<UART_MAX_RX_BUFFER; i++)
	{
		tp_UartBuffer->RxBuffer[i] = 0;
	}
	for(i=0; i<UART_MAX_TX_BUFFER; i++)
	{
		tp_UartBuffer->TxBuffer[i] = 0;
	}
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_IntConfig(uint8_t uart_no, uint32_t intr_mask, uint32_t int_enable)
{
	NVIC_IntrConfig nvic_config;
	UART_Type* UARTx;

	uint32_t intr_status;
	uint32_t reg_val;

	/* Get Uart Object */
	UARTx = HAL_UART_Get_Object(uart_no);

	/* Disable interrupt */
	reg_val = UARTx->IER_DLM;
	reg_val = (reg_val & ~UnIER_INTR_MASK);
	UARTx->IER_DLM = reg_val;

	/* Clear interrupt flags */
	intr_status = UARTx->IIR;
	if (intr_status & UnIIR_IID_RCV_LINE_STATUS)
	{
		reg_val = UARTx->LSR;
	}
	if (intr_status & UnIIR_IID_RBR_READY)
	{
		reg_val = UARTx->RBR_THR_DLL;
	}

	/* Enable interrupt flags */
	if(int_enable == 1)
	{
		reg_val = UARTx->IER_DLM;
		reg_val = reg_val | (intr_mask & ~(UnIER_TEMTIE|UnIER_THREIE));
		UARTx->IER_DLM = reg_val;
	}
	else
	{
		UARTx->IER_DLM = 0;
	}

	/* NVIC Setup */
	nvic_config.nIRQ_Number = (IRQ_UART0+uart_no); 
	nvic_config.Preemption_Priority= PRIO_UART0_PREEMPTION; 
	nvic_config.Subpriority= PRIO_UART0_SUBPRIORITY; 
	nvic_config.IntrEnable = INTR_ENABLE; 
	NVIC_ConfigureInterrupt (NVIC, &nvic_config); 
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_SetBaudrate(UART_Type* UARTx, uint32_t baudrate, uint32_t ref_clk)
{
	uint32_t reg_val;
	uint32_t bdr, dlm, dll;
#ifdef USE_BFR
	uint32_t d_product_f;
	uint32_t bfr;
#endif

	/* Cal. DLM:DLL */
	bdr = ref_clk / (16*2*baudrate);

	dlm = (bdr>>8) & 0x00FF;
	dll = (bdr & 0x00FF);

	/* Enable DLAB */
	reg_val = UARTx->LCR;
	UARTx->LCR = (reg_val | UnLCR_DLAB);

	/* Set Baudrate */
	UARTx->IER_DLM = dlm;
	UARTx->RBR_THR_DLL = dll;
#ifdef USE_BFR
	d_product_f = ref_clk - (bdr * (16*2*baudrate));
	bfr = (d_product_f << 8) / (16*2*baudrate);				// FCNT = point_value * 256
	UARTx->BFR = bfr;
#endif 

	/* Disable DLAB */
	reg_val = UARTx->LCR;
	UARTx->LCR = (reg_val  & ~UnLCR_DLAB);
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_Init(uint8_t uart_no, uint32_t Baudrate, uint8_t DataBits, uint8_t Parity, uint8_t StopBits, uint8_t int_enable)
{
	UART_Type* UARTx;

	uint32_t uart_intr;
	uint32_t uart_mode;
	uint8_t uart_sampling;
	uint8_t uart_invert;

	/* Get Uart Object */
	UARTx = HAL_UART_Get_Object(uart_no);

	/* Set Uart Parameter */
	console_port = uart_no;																																// Set global var.
	uart_intr = (UART_INTR_LINE|UART_INTR_RX|UART_INTR_TX);		// Enable line/rx/tx interrupt
	uart_sampling = 0;																																				// 0: 1 time sampling , 1: 3 time sampling
	uart_invert = 0;																																							// 0: no invert , 1: invert

	/* Set Uart Buffer */
	HAL_Uart_BufferInit(uart_no);

	/* Set Uart Interrupt */
	HAL_Uart_IntConfig(uart_no, uart_intr, int_enable);

	/* Set Uart Baudrate */
	HAL_Uart_SetBaudrate(UARTx, Baudrate, SystemPeriClock);

	/* Set Uart DataBits */
	switch(DataBits)
	{
		case 5:
			uart_mode = UnLCR_DLEN_5_BITS;
			break;
		case 6:
			uart_mode = UnLCR_DLEN_6_BITS;
			break;
		case 7:
			uart_mode = UnLCR_DLEN_7_BITS;
			break;
		case 8:
		default:
			uart_mode = UnLCR_DLEN_8_BITS;
			break;
	}
	/* Set Uart Parity */
	if(Parity == UART_ODD_PARITY)
	{
		uart_mode |= UnLCR_PEN; 
	}
	else if(Parity == UART_EVEN_PARITY)
	{
		uart_mode |= (UnLCR_PEN|UnLCR_PARITY); 
	}
	/* Set Uart StopBits */
	if(StopBits == 1)
	{
		uart_mode |= UnLCR_STOPBIT_1_STOPBIT;
	}
	else
	{
		uart_mode |= UnLCR_STOPBIT_2_STOPBITS;
	}
	/* Set Uart Sampling Time */
	if(uart_sampling)
	{
		UARTx->DTR |= (UnDTR_SMS | UnDTR_DMS);	// Sampling 3 time run
	}
	else
	{
		UARTx->DTR = 0;																								// Sampling 1 time run
	}
	/* Set Uart Invert */
	if(uart_invert)
	{
		UARTx->DTR |= (UnDTR_RXINV|UnDTR_TXINV);
	}

	UARTx->LCR = uart_mode;		// Set Line Control Register
	UARTx->FCR = 0;									// Disable FIFO
	UARTx->SCR = 0;									// Disable Scratch
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_Enable_Tx_Interrupt(UART_Type *UARTx, uint8_t status)
{
	uint32_t reg_val;

	if(status == 1)
	{
		reg_val = UARTx->IER_DLM;
		reg_val |= UnIER_THREIE;
		UARTx->IER_DLM = reg_val;
	}
	else
	{
		reg_val = UARTx->IER_DLM;
		reg_val &= ~(UnIER_THREIE);
		UARTx->IER_DLM = reg_val;
	}
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_Tx_Handler(uint8_t uart_no)
{
	UART_Type* UARTx;
	UART_BUFFER *tp_UartBuffer;
	volatile uint32_t line_status;
	uint8_t send_data;

	/* Get Uart Object */
	UARTx = HAL_UART_Get_Object(uart_no);	

	/* Get Buffer BaseAddress */
	tp_UartBuffer = HAL_Uart_GetBufferBaseAddr(uart_no);

	/* Read Line Status Register */
	line_status = UARTx->LSR;

	/* Line Error Check */
	if((line_status & UnLSR_BI) == UnLSR_BI || (line_status & UnLSR_PE) == UnLSR_PE || (line_status & UnLSR_OE) == UnLSR_OE)
	{
		line_status = UARTx->LSR;
	}
	/* Normal process */
	else
	{
		if((line_status & UnLSR_THRE) == UnLSR_THRE)
		{
			/* There are transmission data */
			if(tp_UartBuffer->TxBuffer_HeadIndex < tp_UartBuffer->TxBuffer_TailIndex)
			{
				send_data = tp_UartBuffer->TxBuffer[tp_UartBuffer->TxBuffer_HeadIndex++];
				UARTx->RBR_THR_DLL = send_data;
			}
			/* Transmission done */
			else
			{
#ifdef	USE_UART_INTERRUPT
				// Disable tx interrupt
				HAL_Uart_Enable_Tx_Interrupt(UARTx, 0);
#endif
				// Initial parameters
				tp_UartBuffer->TxBuffer_HeadIndex = 0;
				tp_UartBuffer->TxBuffer_TailIndex = 0;
				tp_UartBuffer->TxState = UART_TX_STATE_IDLE;
			}
		}
	}
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_Rx_Handler(uint8_t uart_no)
{
	
}

/**
* @brief
* @param   
* @return
*/
void HAL_Uart_Handler(uint8_t uart_no)
{
	UART_Type* UARTx;
	volatile uint32_t intr_status;
	volatile uint32_t line_status;

	/* Get Uart Object */
	UARTx = HAL_UART_Get_Object(uart_no);
	
	/* Check Interrupt ID */
	intr_status = UARTx->IIR;

	/* Line Interrupt */
	if((intr_status & UnIIR_INTR_BASIC_MASK) == UnIIR_IID_RCV_LINE_STATUS)
	{
		line_status = UARTx->LSR;					// Line Interrupt clear
	}
	
	/* Rx Interrupt */
	if((intr_status & UnIIR_INTR_BASIC_MASK) == UnIIR_IID_RBR_READY)
	{
		HAL_Uart_Rx_Handler(uart_no);		// Rx Process
		UART1_ReceiveData_ISR(); // origin function
	}

	/* Tx Interrupt */
	if((intr_status & UnIIR_INTR_BASIC_MASK) == UnIIR_IID_THR_EMPTY)
	{
		HAL_Uart_Tx_Handler(uart_no);		// Tx Process
	}
}

/**
* @brief
* @param   
* @return
*/
uint8_t HAL_Uart_WriteBuffer(uint8_t uart_no, uint8_t *p_data, uint32_t data_count)
{
	UART_Type* UARTx;
	UART_BUFFER *tp_UartBuffer;
	uint32_t i;

	/* Get Uart Object */
	UARTx = HAL_UART_Get_Object(uart_no);
	
	/* Get Buffer BaseAddress */
	tp_UartBuffer = HAL_Uart_GetBufferBaseAddr(uart_no);
	
	/* Wait until past last sequence */
	while(tp_UartBuffer->TxState != UART_TX_STATE_IDLE);
	for(i=0; i<0x100000; i++)
	{
		if(tp_UartBuffer->TxBuffer_HeadIndex == tp_UartBuffer->TxBuffer_TailIndex) {break;}
		if(i == 0xFFFFF) {return (UART_TX_BUFFER_ERROR_WAIT_TIMEOUT);}
	}

	/* Load data to TxBuffer */
	for(i=0; i<data_count; i++)
	{
		tp_UartBuffer->TxBuffer[i] = *(p_data + i);
	}
	tp_UartBuffer->TxBuffer_HeadIndex = 0;
	tp_UartBuffer->TxBuffer_TailIndex = data_count;	

	/* Update Tx Status */
	tp_UartBuffer->TxState = UART_TX_STATE_TRANSMIT;

#ifdef	USE_UART_INTERRUPT
	HAL_Uart_Enable_Tx_Interrupt(UARTx, 1);
#else
	while(1)
	{
		HAL_Uart_Tx_Handler(uart_no);
		if(tp_UartBuffer->TxState == UART_TX_STATE_IDLE)
		{
			break;
		}
	}
#endif
	
	return (UART_TX_BUFFER_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void zputs(uint8_t *str)
{
	uint32_t		i; 

	for (i=0; ; i++)
	{
		if (*(str+i) == 0) break; 
	}

	HAL_Uart_WriteBuffer(console_port, str, i); 
}

int main(void)
{		
	PCU_Init();
	
	SystemInit();														// RINGOSC (1MHz)
	
 	SystemCoreClockUpdate();		// Clock Update to PLL Frequency

	HAL_Port_Init();
	HAL_Uart_Init(1,9600, 8, 0, 1, 1);

	mainloop();							// User Code

	return 0;
}

void mainloop(void)
{
	char 	ch;
	int	disp_menu = 1;
	
	for (;;)
	{
		
		if (disp_menu)
		{
			Disp_MainMenu(); 
			disp_menu = 0; 
		}

		zputs("[G527]#"); 

		InData[0] = 0; 

		for (;;)
		{
			ch = getstring(); 
			if (ch == ASCII_CARRIAGE_RETURN) break; 
		}

		zputs("\r\n\r\n"); 

		if (InFlag)
		{
			if (!strncmp(InData, "1", 1))
			{
				zputs(">>> GPIO Blinky\r\n");
				gpioBlinky();				
			}
			else if (!strncmp(InData, "2", 1))
			{
				zputs(">>> GPIO Input\r\n");
				gpioInput();				
			}	
			else if (!strncmp(InData, "3", 1))
			{
				zputs(">>> GPIO External Input\r\n");
				gpioExtInt();				
			}
			
			InFlag = InCount = 0; 
		}
	}

}

void Disp_MainMenu(void)
{
#if 0
	cputs("\r\n========================================\r\n");
	cputs("GPIO Demo\r\n");  
	cputs("\t - MCU : A33G52x\r\n");
	cputs("\t - Core: ARM Cortex-M3 \n\r");
	cputs("\t - Communicate via: UART0 - 38400 bps \n\r");
	cputs("1. GPIO Blinky\r\n");
	cputs("2. GPIO Input\r\n");
	cputs("3. GPIO External Intput\r\n");
	cputs("========================================\r\n"); 	
#else
	zputs("\r\n====0");
	zputs("GPIO Demo\r\n");  
	zputs("\t - MCU : A33G52x\r\n");
	zputs("\t - Core: ARM Cortex-M3 \n\r");
	zputs("\t - Communicate via: UART0 - 38400 bps \n\r");
	zputs("1. GPIO Blinky\r\n");
	zputs("2. GPIO Input\r\n");
	zputs("3. GPIO External Intput\r\n");
	zputs("========================================\r\n"); 
#endif
}

void delay (void) 
{
	uint32_t i;
	
	for (i = 0; i < 0x100000; i++) ;
}

void gpioBlinky(void)
{
	uint32_t i;	

	// Set LED (PD0, PD1)
	PCU_ConfigureFunction (PCD, PIN_0, PD0_MUX_PD0); 
	PCU_Set_Direction_Type(PCD, PIN_0, PnCR_OUTPUT_PUSH_PULL); 
	PCU_ConfigurePullup_Pulldown (PCD, PIN_0, 0, PnPCR_PULLUPDOWN_DISABLE);
//	PCU_ConfigurePullup_Pulldown (PCD, PIN_0, PnPCR_PULLUPDOWN_UP, PnPCR_PULLUPDOWN_ENABLE);			// PD0 Pull Up Enable
//	PCU_ConfigurePullup_Pulldown (PCD, PIN_0, PnPCR_PULLUPDOWN_DOWN, PnPCR_PULLUPDOWN_ENABLE);		// PD0 Pull-Down Enable
	
	PCU_ConfigureFunction (PCD, PIN_1, PD1_MUX_PD1); 
	PCU_Set_Direction_Type(PCD, PIN_1, PnCR_OUTPUT_PUSH_PULL); 
	PCU_ConfigurePullup_Pulldown (PCD, PIN_1, 0, PnPCR_PULLUPDOWN_DISABLE); 		
//	PCU_ConfigurePullup_Pulldown (PCD, PIN_1, PnPCR_PULLUPDOWN_UP, PnPCR_PULLUPDOWN_ENABLE);			// PD1 Pull Up Enable
//	PCU_ConfigurePullup_Pulldown (PCD, PIN_1, PnPCR_PULLUPDOWN_DOWN, PnPCR_PULLUPDOWN_ENABLE);		// PD1 Pull-Down Enable

	PCU_ConfigureFunction (PCD, PIN_2, PD2_MUX_PD2); 
	PCU_Set_Direction_Type(PCD, PIN_2, PnCR_OUTPUT_PUSH_PULL); 
	PCU_ConfigurePullup_Pulldown (PCD, PIN_2, 0, PnPCR_PULLUPDOWN_DISABLE); 		
	PCU_ConfigureFunction (PCD, PIN_3, PD3_MUX_PD3); 
	PCU_Set_Direction_Type(PCD, PIN_3, PnCR_OUTPUT_PUSH_PULL); 
	PCU_ConfigurePullup_Pulldown (PCD, PIN_3, 0, PnPCR_PULLUPDOWN_DISABLE); 		
	PCU_ConfigureFunction (PCD, PIN_4, PD4_MUX_PD4); 
	PCU_Set_Direction_Type(PCD, PIN_4, PnCR_OUTPUT_PUSH_PULL); 
	PCU_ConfigurePullup_Pulldown (PCD, PIN_4, 0, PnPCR_PULLUPDOWN_DISABLE); 		
	PCU_ConfigureFunction (PCD, PIN_5, PD5_MUX_PD5); 
	PCU_Set_Direction_Type(PCD, PIN_5, PnCR_OUTPUT_PUSH_PULL); 
	PCU_ConfigurePullup_Pulldown (PCD, PIN_5, 0, PnPCR_PULLUPDOWN_DISABLE); 		
	PCU_ConfigureFunction (PCD, PIN_6, PD6_MUX_PD6); 
	PCU_Set_Direction_Type(PCD, PIN_6, PnCR_OUTPUT_PUSH_PULL); 
	PCU_ConfigurePullup_Pulldown (PCD, PIN_6, 0, PnPCR_PULLUPDOWN_DISABLE); 		
	PCU_ConfigureFunction (PCD, PIN_7, PD7_MUX_PD7); 
	PCU_Set_Direction_Type(PCD, PIN_7, PnCR_OUTPUT_PUSH_PULL); 
	PCU_ConfigurePullup_Pulldown (PCD, PIN_7, 0, PnPCR_PULLUPDOWN_DISABLE); 		


	
	cprintf("SystemCoreClock : %d\r\n", SystemCoreClock);
	
	// Set Systick1
	
//	SysTick_Config(SystemCoreClock/1000000);  //1us interrupt 
	SysTick_Config(SystemCoreClock/1000);      //1ms interrupt 
	
	SysTick_Run (SysTick, INTR_ENABLE);
	__enable_irq();
		
/*
	if(SysTick->LOAD == (SystemCoreClock/1000000)-1)
	{
		while(1);
	}
	else 
	if(SysTick->LOAD == (SystemCoreClock/1000)-1) */
	{
//		while(1)
//		{
//			cprintf("OK\r\n");
//		period = 500;
//		while(period);
//		GPIO_OutputLow(PD,0);
//		period = 500;
//		while(period);
//		GPIO_OutputHigh(PD,0);
//		if (tmp&0x1)
//			GPIO_OutputHigh(PD,1);
//		else 
//			GPIO_OutputLow(PD,1);
//		if (++tmp == 8) 
//			tmp = 0;	
//		}			
		while(1){
			period = 200;
			while(period);
		
		for(i=0;i<6;i++){
			period = 50;
			while(period);
			GPIO_OutputLow(PD, i);	//_BIT(i));
		}
		
		for(i=0;i<6;i++){
			period = 50;
			while(period);
			GPIO_OutputHigh(PD,i);	//_BIT(i));
		}
	}

	}	

}

void gpioInput(void)
{
	// Set LED (PD0)
	PCU_ConfigureFunction (PCD, PIN_0, PD0_MUX_PD0); 
	PCU_Set_Direction_Type(PCD, PIN_0, PnCR_OUTPUT_PUSH_PULL); 
	PCU_ConfigurePullup_Pulldown (PCD, PIN_0, 0, PnPCR_PULLUPDOWN_DISABLE);
//	PCU_ConfigurePullup_Pulldown (PCD, PIN_0, PnPCR_PULLUPDOWN_UP, PnPCR_PULLUPDOWN_ENABLE);			// PD0 Pull Up Enable
//	PCU_ConfigurePullup_Pulldown (PCD, PIN_0, PnPCR_PULLUPDOWN_DOWN, PnPCR_PULLUPDOWN_ENABLE);		// PD0 Pull-Down Enable	
	
	// Set Switch (PB1)
	PCU_ConfigureFunction (PCB, PIN_1, PB1_MUX_PB1); 
	PCU_Set_Direction_Type(PCB, PIN_1, PnCR_INPUT_LOGIC); 
	PCU_ConfigurePullup_Pulldown (PCB, PIN_1, PnPCR_PULLUPDOWN_UP, PnPCR_PULLUPDOWN_ENABLE);			// PB1 Pull Up Enable
	
	while(1){
		if (GPIO_ReadValue(PB) & (1 << 1))
			GPIO_OutputHigh(PD, 0);
		else 
			GPIO_OutputLow(PD, 0);
	}	
}

extern uint32_t				g_PB_PnISR; 
void gpioExtInt(void)
{
	NVIC_IntrConfig 				nvic_config;
	
	// Set LED (PD0)
	PCU_ConfigureFunction (PCD, PIN_0, PD0_MUX_PD0); 
	PCU_Set_Direction_Type(PCD, PIN_0, PnCR_OUTPUT_PUSH_PULL); 
	PCU_ConfigurePullup_Pulldown (PCD, PIN_0, 0, PnPCR_PULLUPDOWN_DISABLE);
	//	PCU_ConfigurePullup_Pulldown (PCD, PIN_0, PnPCR_PULLUPDOWN_UP, PnPCR_PULLUPDOWN_ENABLE);			// PD0 Pull Up Enable
	//	PCU_ConfigurePullup_Pulldown (PCD, PIN_0, PnPCR_PULLUPDOWN_DOWN, PnPCR_PULLUPDOWN_ENABLE);		// PD0 Pull-Down Enable	
	
	// Set Switch (PB1)
	PCU_ConfigureFunction (PCB, PIN_1, PB1_MUX_PB1); 
	PCU_Set_Direction_Type(PCB, PIN_1, PnCR_INPUT_LOGIC); 
	PCU_ConfigurePullup_Pulldown (PCB, PIN_1, PnPCR_PULLUPDOWN_UP, PnPCR_PULLUPDOWN_ENABLE);		// PB1 Pull-Down Enable
	

	// Set External Interrupt of GPIO (PB1)
//	PCU_ConfigureInterrupt(PCB, PIN_1, PCU_LOW_LEVEL_INTR, INTR_ENABLE); 				// Low Level Interrupt
//	PCU_ConfigureInterrupt(PCB, PIN_1, PCU_HIGH_LEVEL_INTR, INTR_ENABLE); 				// High Level Interrupt
//	PCU_ConfigureInterrupt(PCB, PIN_1, PCU_FALLING_EDGE_INTR, INTR_ENABLE); 			// Falling Edge Interrupt
//	PCU_ConfigureInterrupt(PCB, PIN_1, PCU_RISING_EDGE_INTR, INTR_ENABLE); 				// Rising Edge Interrupt
	PCU_ConfigureInterrupt(PCB, PIN_1, PCU_BOTH_FALLING_RISING_EDGE_INTR, INTR_ENABLE); 	// Falling & Rising Interrupt
	

	// Set GPIOB Interrupt
	nvic_config.nIRQ_Number = IRQ_GPIOB; 
	nvic_config.Preemption_Priority= PRIO_GPIOB_PREEMPTION; 
	nvic_config.Subpriority= PRIO_GPIOB_SUBPRIORITY; 
	nvic_config.IntrEnable = INTR_ENABLE; 
	NVIC_ConfigureInterrupt (NVIC, &nvic_config);
	__enable_irq();
	

	while(1){
		cprintf("PBISR = 0x%08X\r\n", g_PB_PnISR, PB->IDR);
		if(g_PB_PnISR == 0x04)
			cprintf("Falling\r\n");
		else if(g_PB_PnISR == 0x08)
			cprintf("Rising\r\n");		
		else if(g_PB_PnISR == 0x0C)
			cprintf("Rising or Falling\r\n");		
		g_PB_PnISR = 0;
		GPIO_OutputLow(PD, 0);
		delay();
	}
	
}

void PCU_Init(void)
{
	//Peripheral Enable Register 1  0:Disable, 1:Enable	
	PMU->PER=PMU->PER
		| (1<<13)   // GPIOF
		| (1<<12)   // GPIOE
		| (1<<11)   // GPIOD
		| (1<<10)   // GPIOC
		| (1<<9)    // GPIOB
		| (1<<8)    // GPIOA
		; 		
//Peripheral Clock Enable Register 1 0:Disable, 1:Enable	
	PMU->PCCR=PMU->PCCR
		| (1<<8)    // GPIO
		; 	
	
	//--------------------------------------------------------------
	//	PORT INIT
	//		PA, PB, PC, PD, PE, PF
	//--------------------------------------------------------------
	// PORT - A
	PCA->MR = 0
		| (0x0UL<<30)              // 0:PA15		1:T9C			2:				3:AN15
		| (0x0UL<<28)              // 0:PA14		1:T8C			2:				3:AN14
		| (0x0UL<<26)              // 0:PA13		1:T7C			2:				3:
		| (0x0UL<<24)              // 0:PA12		1:T6C			2:				3:
		| (0x0UL<<22)              // 0:PA11		1:T5C			2:				3:
		| (0x0UL<<20)              // 0:PA10		1:T4C			2:				3:
		| (0x0UL<<18)              // 0:PA9			1:T3C			2:				3:
		| (0x0UL<<16)              // 0:PA8			1:T2C			2:				3:
		| (0x0UL<<14)              // 0:PA7			1:T1C			2:				3:AN7
		| (0x0UL<<12)              // 0:PA6			1:T0C			2:				3:AN6
		| (0x0UL<<10)              // 0:PA5			1:				2:				3:AN5
		| (0x0UL<<8)               // 0:PA4			1:				2:				3:AN4
		| (0x0UL<<6)               // 0:PA3			1:				2:				3:AN3
		| (0x0UL<<4)               // 0:PA2			1:				2:				3:AN2
		| (0x0UL<<2)               // 0:PA1			1:				2:				3:AN1
		| (0x0UL<<0)               // 0:PA0			1:				2:				3:AN0
		;
	
	PCA->CR = 0              // 0 : Output(Push-pull) Mode,	1 : Output(Open-drain) Mode,	2 : Input,	3: Analog
		| (0x3UL<<30)              // P15
		| (0x3UL<<28)              // P14
		| (0x3UL<<26)              // P13
		| (0x3UL<<24)              // P12
		| (0x3UL<<22)              // P11
		| (0x3UL<<20)              // P10
		| (0x3UL<<18)              // P9
		| (0x3UL<<16)              // P8
		| (0x3UL<<14)              // P7
		| (0x3UL<<12)              // P6
		| (0x3UL<<10)              // P5
		| (0x3UL<<8)              // P4
		| (0x3UL<<6)              // P3
		| (0x3UL<<4)              // P2
		| (0x3UL<<2)              // P1
		| (0x3UL<<0)              // P0
		;
	
	PCA->PCR = 0
		| (0x0UL<<31)             // P15		0: Pull up,			1: Pull down
		| (0x0UL<<30)             // P14		0: Pull up,			1: Pull down
		| (0x0UL<<29)             // P13		0: Pull up,			1: Pull down
		| (0x0UL<<28)             // P12		0: Pull up,			1: Pull down
		| (0x0UL<<27)             // P11		0: Pull up,			1: Pull down
		| (0x0UL<<26)             // P10		0: Pull up,			1: Pull down
		| (0x0UL<<25)             // P9			0: Pull up,			1: Pull down
		| (0x0UL<<24)             // P8			0: Pull up,			1: Pull down
		| (0x0UL<<23)             // P7			0: Pull up,			1: Pull down
		| (0x0UL<<22)             // P6			0: Pull up,			1: Pull down
		| (0x0UL<<21)             // P5			0: Pull up,			1: Pull down
		| (0x0UL<<20)             // P4			0: Pull up,			1: Pull down
		| (0x0UL<<19)             // P3			0: Pull up,			1: Pull down
		| (0x0UL<<18)             // P2			0: Pull up,			1: Pull down
		| (0x0UL<<17)             // P1			0: Pull up,			1: Pull down
		| (0x0UL<<16)             // P0			0: Pull up,			1: Pull down
		| (0x0UL<<15)             // P15		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<14)             // P14		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<13)             // P13		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<12)             // P12		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<11)             // P11		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<10)             // P10		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<9)              // P9			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<8)              // P8			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<7)              // P7			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<6)              // P6			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<5)              // P5			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<4)              // P4			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<3)              // P3			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<2)              // P2			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<1)              // P1			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<0)              // P0			0: Disable PU/PD 	1: Enable PU/PD
		;
	
//////////////////////////////////////////////////////////////////////////////////////////////////////	
	// PORT - B
	PCB->MR = 0
		| (0x0UL<<30)              // 0:PB15		1:SDA0			2:				3:
		| (0x0UL<<28)              // 0:PB14		1:SCL0			2:				3:
		| (0x0UL<<26)              // 0:PB13		1:MISO0			2:				3:
		| (0x0UL<<24)              // 0:PB12		1:MOSI0			2:				3:
		| (0x0UL<<22)              // 0:PB11		1:SCK0			2:				3:
		| (0x0UL<<20)              // 0:PB10		1:SS0			2:				3:
		| (0x0UL<<18)              // 0:PB9			1:T9O			2:				3:
		| (0x0UL<<16)              // 0:PB8			1:T8O			2:				3:
		| (0x0UL<<14)              // 0:PB7			1:T7O			2:				3:
		| (0x0UL<<12)              // 0:PB6			1:T6O			2:				3:
		| (0x0UL<<10)              // 0:PB5			1:T5O			2:				3:
		| (0x0UL<<8)               // 0:PB4			1:T4O			2:				3:
		| (0x0UL<<6)               // 0:PB3			1:T3O			2:				3:
		| (0x0UL<<4)               // 0:PB2			1:T2O			2:				3:
		| (0x0UL<<2)               // 0:PB1			1:T1O			2:				3:
		| (0x0UL<<0)               // 0:PB0			1:T0O			2:				3:
		;
	
	PCB->CR = 0              // 0 : Output(Push-pull) Mode,	1 : Output(Open-drain) Mode,	2 : Input,	3: Analog
		| (0x3UL<<30)              // P15
		| (0x3UL<<28)              // P14
		| (0x3UL<<26)              // P13
		| (0x3UL<<24)              // P12
		| (0x3UL<<22)              // P11
		| (0x3UL<<20)              // P10
		| (0x3UL<<18)              // P9
		| (0x3UL<<16)              // P8
		| (0x3UL<<14)              // P7
		| (0x3UL<<12)              // P6
		| (0x3UL<<10)              // P5
		| (0x3UL<<8)              // P4
		| (0x3UL<<6)              // P3
		| (0x3UL<<4)              // P2
		| (0x3UL<<2)              // P1
		| (0x3UL<<0)              // P0
		;
	
	PCB->PCR = 0
		| (0x0UL<<31)             // P15		0: Pull up,			1: Pull down
		| (0x0UL<<30)             // P14		0: Pull up,			1: Pull down
		| (0x0UL<<29)             // P13		0: Pull up,			1: Pull down
		| (0x0UL<<28)             // P12		0: Pull up,			1: Pull down
		| (0x0UL<<27)             // P11		0: Pull up,			1: Pull down
		| (0x0UL<<26)             // P10		0: Pull up,			1: Pull down
		| (0x0UL<<25)             // P9			0: Pull up,			1: Pull down
		| (0x0UL<<24)             // P8			0: Pull up,			1: Pull down
		| (0x0UL<<23)             // P7			0: Pull up,			1: Pull down
		| (0x0UL<<22)             // P6			0: Pull up,			1: Pull down
		| (0x0UL<<21)             // P5			0: Pull up,			1: Pull down
		| (0x0UL<<20)             // P4			0: Pull up,			1: Pull down
		| (0x0UL<<19)             // P3			0: Pull up,			1: Pull down
		| (0x0UL<<18)             // P2			0: Pull up,			1: Pull down
		| (0x0UL<<17)             // P1			0: Pull up,			1: Pull down
		| (0x0UL<<16)             // P0			0: Pull up,			1: Pull down
		| (0x0UL<<15)             // P15		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<14)             // P14		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<13)             // P13		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<12)             // P12		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<11)             // P11		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<10)             // P10		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<9)              // P9			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<8)              // P8			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<7)              // P7			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<6)              // P6			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<5)              // P5			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<4)              // P4			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<3)              // P3			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<2)              // P2			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<1)              // P1			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<0)              // P0			0: Disable PU/PD 	1: Enable PU/PD
		;
	
//////////////////////////////////////////////////////////////////////////////////////////////////////	
	// PORT - C
	PCC->MR = 0
		| (0x0UL<<30)              // 0:PC15		1:XTALI			2:CLKIN			3:
		| (0x0UL<<28)              // 0:PC14		1:XTALO			2:				3:
		| (0x0UL<<26)              // 0:PC13		1:CLKO			2:				3:
		| (0x0UL<<24)              // 0:PC12		1:STBYO			2:				3:
		| (0x0UL<<22)              // 0:PC11		1:TXD2			2:				3:
		| (0x0UL<<20)              // 0:PC10		1:RXD2			2:				3:
		| (0x0UL<<18)              // 0:PC9			1:TXD0			2:				3:
		| (0x0UL<<16)              // 0:PC8			1:RXD0			2:				3:
		| (0x0UL<<14)              // 0:PC7			1:				2:				3:
		| (0x1UL<<12)              // 0:PC6			1:nRESET		2:				3:
		| (0x0UL<<10)              // 0:PC5			1:				2:				3:
		| (0x1UL<<8)               // 0:PC4			1:TDO			2:				3:
		| (0x1UL<<6)               // 0:PC3			1:TCK			2:				3:
		| (0x1UL<<4)               // 0:PC2			1:TMD			2:				3:
		| (0x1UL<<2)               // 0:PC1			1:TDI			2:				3:
		| (0x1UL<<0)               // 0:PC0			1:nTRST			2:				3:
		;
	
	PCC->CR = 0              // 0 : Output(Push-pull) Mode,	1 : Output(Open-drain) Mode,	2 : Input,	3: Analog
		| (0x3UL<<30)              // P15
		| (0x3UL<<28)              // P14
		| (0x3UL<<26)              // P13
		| (0x3UL<<24)              // P12
		| (0x3UL<<22)              // P11
		| (0x3UL<<20)              // P10
		| (0x3UL<<18)              // P9
		| (0x3UL<<16)              // P8
		| (0x3UL<<14)              // P7
		| (0x2UL<<12)              // P6
		| (0x3UL<<10)              // P5
		| (0x0UL<<8)              // P4
		| (0x2UL<<6)              // P3
		| (0x2UL<<4)              // P2
		| (0x2UL<<2)              // P1
		| (0x2UL<<0)              // P0
		;
	
	PCC->PCR = 0
		| (0x0UL<<31)             // P15		0: Pull up,			1: Pull down
		| (0x0UL<<30)             // P14		0: Pull up,			1: Pull down
		| (0x0UL<<29)             // P13		0: Pull up,			1: Pull down
		| (0x0UL<<28)             // P12		0: Pull up,			1: Pull down
		| (0x0UL<<27)             // P11		0: Pull up,			1: Pull down
		| (0x0UL<<26)             // P10		0: Pull up,			1: Pull down
		| (0x0UL<<25)             // P9			0: Pull up,			1: Pull down
		| (0x0UL<<24)             // P8			0: Pull up,			1: Pull down
		| (0x0UL<<23)             // P7			0: Pull up,			1: Pull down
		| (0x0UL<<22)             // P6			0: Pull up,			1: Pull down
		| (0x0UL<<21)             // P5			0: Pull up,			1: Pull down
		| (0x0UL<<20)             // P4			0: Pull up,			1: Pull down
		| (0x0UL<<19)             // P3			0: Pull up,			1: Pull down
		| (0x0UL<<18)             // P2			0: Pull up,			1: Pull down
		| (0x0UL<<17)             // P1			0: Pull up,			1: Pull down
		| (0x0UL<<16)             // P0			0: Pull up,			1: Pull down
		| (0x0UL<<15)             // P15		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<14)             // P14		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<13)             // P13		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<12)             // P12		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<11)             // P11		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<10)             // P10		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<9)              // P9			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<8)              // P8			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<7)              // P7			0: Disable PU/PD 	1: Enable PU/PD
		| (0x1UL<<6)              // P6			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<5)              // P5			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<4)              // P4			0: Disable PU/PD 	1: Enable PU/PD
		| (0x1UL<<3)              // P3			0: Disable PU/PD 	1: Enable PU/PD
		| (0x1UL<<2)              // P2			0: Disable PU/PD 	1: Enable PU/PD
		| (0x1UL<<1)              // P1			0: Disable PU/PD 	1: Enable PU/PD
		| (0x1UL<<0)              // P0			0: Disable PU/PD 	1: Enable PU/PD
		;
	
//////////////////////////////////////////////////////////////////////////////////////////////////////	
	// PORT - D
	PCD->MR = 0
		| (0x0UL<<30)              // 0:PD15		1:SDA1			2:				3:
		| (0x0UL<<28)              // 0:PD14		1:SCL1			2:				3:
		| (0x0UL<<26)              // 0:PD13		1:TXD1			2:				3:
		| (0x0UL<<24)              // 0:PD12		1:RXD1			2:				3:
		| (0x0UL<<22)              // 0:PD11		1:MISO1			2:				3:
		| (0x0UL<<20)              // 0:PD10		1:MOSI1			2:				3:
		| (0x0UL<<18)              // 0:PD9			1:SCK1			2:				3:
		| (0x0UL<<16)              // 0:PD8			1:SS1			2:				3:
		| (0x0UL<<14)              // 0:PD7			1:MPWMA7		2:				3:
		| (0x0UL<<12)              // 0:PD6			1:MPWMA6		2:				3:
		| (0x0UL<<10)              // 0:PD5			1:MPWMA5		2:				3:
		| (0x0UL<<8)               // 0:PD4			1:MPWMA4		2:				3:
		| (0x0UL<<6)               // 0:PD3			1:MPWMA3		2:				3:
		| (0x0UL<<4)               // 0:PD2			1:MPWMA2		2:				3:
		| (0x0UL<<2)               // 0:PD1			1:MPWMA1		2:				3:
		| (0x0UL<<0)               // 0:PD0			1:MPWMA0		2:				3:
		;
	
	PCD->CR = 0              // 0 : Output(Push-pull) Mode,	1 : Output(Open-drain) Mode,	2 : Input,	3: Analog
		| (0x3UL<<30)              // P15
		| (0x3UL<<28)              // P14
		| (0x3UL<<26)              // P13
		| (0x3UL<<24)              // P12
		| (0x3UL<<22)              // P11
		| (0x3UL<<20)              // P10
		| (0x3UL<<18)              // P9
		| (0x3UL<<16)              // P8
		| (0x3UL<<14)              // P7
		| (0x3UL<<12)              // P6
		| (0x3UL<<10)              // P5
		| (0x3UL<<8)              // P4
		| (0x3UL<<6)              // P3
		| (0x3UL<<4)              // P2
		| (0x3UL<<2)              // P1
		| (0x3UL<<0)              // P0
		;
	
	PCD->PCR = 0
		| (0x0UL<<31)             // P15		0: Pull up,			1: Pull down
		| (0x0UL<<30)             // P14		0: Pull up,			1: Pull down
		| (0x0UL<<29)             // P13		0: Pull up,			1: Pull down
		| (0x0UL<<28)             // P12		0: Pull up,			1: Pull down
		| (0x0UL<<27)             // P11		0: Pull up,			1: Pull down
		| (0x0UL<<26)             // P10		0: Pull up,			1: Pull down
		| (0x0UL<<25)             // P9			0: Pull up,			1: Pull down
		| (0x0UL<<24)             // P8			0: Pull up,			1: Pull down
		| (0x0UL<<23)             // P7			0: Pull up,			1: Pull down
		| (0x0UL<<22)             // P6			0: Pull up,			1: Pull down
		| (0x0UL<<21)             // P5			0: Pull up,			1: Pull down
		| (0x0UL<<20)             // P4			0: Pull up,			1: Pull down
		| (0x0UL<<19)             // P3			0: Pull up,			1: Pull down
		| (0x0UL<<18)             // P2			0: Pull up,			1: Pull down
		| (0x0UL<<17)             // P1			0: Pull up,			1: Pull down
		| (0x0UL<<16)             // P0			0: Pull up,			1: Pull down
		| (0x0UL<<15)             // P15		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<14)             // P14		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<13)             // P13		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<12)             // P12		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<11)             // P11		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<10)             // P10		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<9)              // P9			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<8)              // P8			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<7)              // P7			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<6)              // P6			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<5)              // P5			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<4)              // P4			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<3)              // P3			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<2)              // P2			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<1)              // P1			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<0)              // P0			0: Disable PU/PD 	1: Enable PU/PD
		;
	
//////////////////////////////////////////////////////////////////////////////////////////////////////	
	// PORT - E
	PCE->MR = 0
		| (0x0UL<<30)              // 0:PE15		1:TraceCLK		2:				3:
		| (0x0UL<<28)              // 0:PE14		1:TraceD0		2:				3:
		| (0x0UL<<26)              // 0:PE13		1:TraceD1		2:				3:
		| (0x0UL<<24)              // 0:PE12		1:TraceD2		2:				3:
		| (0x0UL<<22)              // 0:PE11		1:TraceD3		2:				3:
		| (0x1UL<<18)              // 0:PE9			1:SXOUT			2:				3:
		| (0x1UL<<16)              // 0:PE8			1:SXIN			2:				3:
		| (0x0UL<<14)              // 0:PE7			1:MPWMB7		2:TXD3			3:
		| (0x0UL<<12)              // 0:PE6			1:MPWMB6		2:RXD3			3:
		| (0x0UL<<10)              // 0:PE5			1:MPWMB5		2:				3:
		| (0x0UL<<8)               // 0:PE4			1:MPWMB4		2:				3:
		| (0x0UL<<6)               // 0:PE3			1:MPWMB3		2:				3:
		| (0x0UL<<4)               // 0:PE2			1:MPWMB2		2:				3:
		| (0x0UL<<2)               // 0:PE1			1:MPWMB1		2:				3:
		| (0x0UL<<0)               // 0:PE0			1:MPWMB0		2:				3:
		;
	
	PCE->CR = 0              // 0 : Output(Push-pull) Mode,	1 : Output(Open-drain) Mode,	2 : Input,	3: Analog
		| (0x3UL<<30)              // P15
		| (0x3UL<<28)              // P14
		| (0x3UL<<26)              // P13
		| (0x3UL<<24)              // P12
		| (0x3UL<<22)              // P11
		| (0x3UL<<18)              // P9
		| (0x3UL<<16)              // P8
		| (0x3UL<<14)              // P7
		| (0x3UL<<12)              // P6
		| (0x3UL<<10)              // P5
		| (0x3UL<<8)              // P4
		| (0x3UL<<6)              // P3
		| (0x3UL<<4)              // P2
		| (0x3UL<<2)              // P1
		| (0x3UL<<0)              // P0
		;
	
	PCE->PCR = 0
		| (0x0UL<<31)             // P15		0: Pull up,			1: Pull down
		| (0x0UL<<30)             // P14		0: Pull up,			1: Pull down
		| (0x0UL<<29)             // P13		0: Pull up,			1: Pull down
		| (0x0UL<<28)             // P12		0: Pull up,			1: Pull down
		| (0x0UL<<27)             // P11		0: Pull up,			1: Pull down
		| (0x0UL<<25)             // P9			0: Pull up,			1: Pull down
		| (0x0UL<<24)             // P8			0: Pull up,			1: Pull down
		| (0x0UL<<23)             // P7			0: Pull up,			1: Pull down
		| (0x0UL<<22)             // P6			0: Pull up,			1: Pull down
		| (0x0UL<<21)             // P5			0: Pull up,			1: Pull down
		| (0x0UL<<20)             // P4			0: Pull up,			1: Pull down
		| (0x0UL<<19)             // P3			0: Pull up,			1: Pull down
		| (0x0UL<<18)             // P2			0: Pull up,			1: Pull down
		| (0x0UL<<17)             // P1			0: Pull up,			1: Pull down
		| (0x0UL<<16)             // P0			0: Pull up,			1: Pull down
		| (0x0UL<<15)             // P15		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<14)             // P14		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<13)             // P13		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<12)             // P12		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<11)             // P11		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<9)              // P9			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<8)              // P8			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<7)              // P7			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<6)              // P6			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<5)              // P5			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<4)              // P4			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<3)              // P3			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<2)              // P2			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<1)              // P1			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<0)              // P0			0: Disable PU/PD 	1: Enable PU/PD
		;
	
//////////////////////////////////////////////////////////////////////////////////////////////////////	
	// PORT - F
	PCF->MR = 0
		| (0x0UL<<22)              // 0:PF11		1:				2:FRTO_FMF		3:
		| (0x0UL<<20)              // 0:PF10		1:				2:				3:
		| (0x0UL<<18)              // 0:PF9		1:				2:				3:
		| (0x0UL<<16)              // 0:PF8		1:				2:				3:
		| (0x0UL<<14)              // 0:PF7		1:				2:				3:
		| (0x0UL<<10)              // 0:PF5		1:				2:				3:AN13
		| (0x0UL<<8)               // 0:PF4		1:				2:				3:AN12
		| (0x0UL<<6)               // 0:PF3		1:				2:				3:AN11
		| (0x0UL<<4)               // 0:PF2		1:				2:				3:AN10
		| (0x0UL<<2)               // 0:PF1		1:				2:				3:AN9
		| (0x0UL<<0)               // 0:PF0		1:				2:				3:AN8
		;
	
	PCF->CR = 0              // 0 : Output(Push-pull) Mode,	1 : Output(Open-drain) Mode,	2 : Input,	3: Analog
		| (0x3UL<<22)              // P11
		| (0x3UL<<20)              // P10
		| (0x3UL<<18)              // P9
		| (0x3UL<<16)              // P8
		| (0x3UL<<14)              // P7
		| (0x3UL<<10)              // P5
		| (0x3UL<<8)              // P4
		| (0x3UL<<6)              // P3
		| (0x3UL<<4)              // P2
		| (0x3UL<<2)              // P1
		| (0x3UL<<0)              // P0
		;
	
	PCF->PCR = 0
		| (0x0UL<<27)             // P11		0: Pull up,			1: Pull down
		| (0x0UL<<26)             // P10		0: Pull up,			1: Pull down
		| (0x0UL<<25)             // P9			0: Pull up,			1: Pull down
		| (0x0UL<<24)             // P8			0: Pull up,			1: Pull down
		| (0x0UL<<23)             // P7			0: Pull up,			1: Pull down
		| (0x0UL<<21)             // P5			0: Pull up,			1: Pull down
		| (0x0UL<<20)             // P4			0: Pull up,			1: Pull down
		| (0x0UL<<19)             // P3			0: Pull up,			1: Pull down
		| (0x0UL<<18)             // P2			0: Pull up,			1: Pull down
		| (0x0UL<<17)             // P1			0: Pull up,			1: Pull down
		| (0x0UL<<16)             // P0			0: Pull up,			1: Pull down
		| (0x0UL<<11)             // P11		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<10)             // P10		0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<9)              // P9			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<8)              // P8			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<7)              // P7			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<5)              // P5			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<4)              // P4			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<3)              // P3			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<2)              // P2			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<1)              // P1			0: Disable PU/PD 	1: Enable PU/PD
		| (0x0UL<<0)              // P0			0: Disable PU/PD 	1: Enable PU/PD
		;
}

void SysTick_Handler (void) 					// SysTick Interrupt Handler @ 1000Hz
{
	if (SysTick->LOAD == (SystemCoreClock/1000000)-1)		// SysTick period is 1us
	{
		if(flag == 0)
		{
			GPIO_OutputLow(PD,0);
			flag = 1;
		}
		else
		{
			GPIO_OutputHigh(PD,0);
			flag = 0;
		}
		cnt++;
	}
	else if(SysTick->LOAD == (SystemCoreClock/1000)-1)		// SysTick period is 1ms
	{
		if(period)
			period--;
	}
}

void UART1_Handler (void)
{
//	HAL_Uart_Handler(1);
	UART1_Transmit_Receive_ISR(); 
}
