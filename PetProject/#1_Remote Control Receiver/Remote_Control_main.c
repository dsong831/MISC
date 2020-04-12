/*******************************************************************************
* Included File
*******************************************************************************/
#include "I2C_API_V1.00.h"
#include "IR_API_V1.00.h"
#include "DeviceName.h"
/*******************************************************************************
* Global Variable
*******************************************************************************/
unsigned char au8TxData[8];
unsigned char au8RxData[1];
/*******************************************************************************
* Interrupt Handler
*******************************************************************************/
TIMER1n_CAPTURECFG_Type TIMER1n_Config;
void TIMER12_IRQHandler(void)
{
	TIMER1nCapInt_ClrFg(TIMER12);
	IR_Rec_Handler();
}
/*******************************************************************************
* Public Function
*******************************************************************************/
void Timer12_Init(void)
{
	// Set GPIO Pin to timer function
	PCU_SetDirection(PA, 5, ALTERN_FUNC);
	PCU_ConfigureFunction(PA, 5, FUNC2);

	// Set Timer mode
	SCU_SetTimer1nClk(T1NCLK_PCLK);
	TIMER1n_Config.CkSel = TIMER1n_MCCR1PCLK;    
	TIMER1n_Config.Prescaler = 384;    														// 48Mhz / 384 = 125000hz ->8us (Timer Period)
	TIMER1n_Config.ClrMode=TIMER1n_FALLING_EGDE;   //	FALLING_EGDE,RISING_EGDE,BOTH_EGDE,

	TIMER1n_Init(TIMER12, TIMER1n_CAPTURE_MODE, &TIMER1n_Config);
	TIMER1n_IntConfig(TIMER12, TIMER1n_INTCFG_CIE, ENABLE);
	TIMER1nCapInt_ClrFg(TIMER12);

	NVIC_ClearPendingIRQ(TIMER12_IRQn);
  NVIC_SetPriority(TIMER12_IRQn, 3);
  NVIC_EnableIRQ(TIMER12_IRQn);
	
	TIMER1n_ClrCnt(TIMER12);
	TIMER1n_Cmd(TIMER12, ENABLE);		// Timer start & clear
	
	__enable_irq();
}

void SEG_Init(void)
{
	unsigned char i = 0;
	
	// I2C0 PD0:SCL0, PD1:SDA0	
	PCU_SetDirection(PD, 0, ALTERN_FUNC);
	PCU_ConfigureFunction(PD, 0, FUNC1);
	PCU_SetDirection(PD, 1, ALTERN_FUNC);
	PCU_ConfigureFunction(PD, 1, FUNC1);
	
	USI_I2C_Initial(I2C_CH0, I2C_SPEED, I2C_DEVICE_ADDRESS, I2C_ACK_ENABLE);
	
	// Driver set to output mode
	au8TxData[0] = 0x06;
	au8TxData[1] = 0x00;
	USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);

	// Driver set to SEG number
	au8TxData[0] = 0x02;
	au8TxData[1] = ~SEG_TO_NUM(0);
	USI_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, au8TxData, 2, au8RxData, 0);
}


int Remote_Control_main(void)
{
	__disable_irq();

	Timer12_Init();
	SEG_Init();

	__enable_irq();

	while(1);
	return (0);
}
