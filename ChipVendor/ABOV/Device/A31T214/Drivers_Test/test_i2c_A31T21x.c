/**********************************************************************
* @file		test_i2c_A31T21x.c
* @brief	Contains all macro definitions and function prototypes
* 			support for A31T21x firmware library
* @version	1.0
* @date		
* @author	ABOV M team
*
* Copyright(C) 2020, ABOV Semiconductor
* All rights reserved.
*
**********************************************************************/
#include "main_conf.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void test_i2c_main(void);
static void I2C_SLAVE_TEST(void);
static void I2C_MASTER_TEST(void);
/* Private variables ---------------------------------------------------------*/
extern uint8_t receiveBuffer0[I2C_MAX_BUFFER_SIZE];
extern uint8_t transmitBuffer0[I2C_MAX_BUFFER_SIZE];
extern uint8_t receiveBuffer1[I2C_MAX_BUFFER_SIZE];
extern uint8_t transmitBuffer1[I2C_MAX_BUFFER_SIZE];


/**********************************************************************
 * @brief		Test Main program
 * @param[in]	None
 * @return	None
 **********************************************************************/
void test_i2c_main(void)
{

}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void I2C_SLAVE_TEST(void)
{
	// Test Pin setting PA1-SCL1 / PA0-SDA1
	HAL_GPIO_ModeConfig(PA, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 1, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PA, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 0, ALTERN_FUNC1);
	
#if 0
	// Test Pin setting PF6-SCL0 / PF7-SDA0
	HAL_GPIO_ModeConfig(PF, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PF, 6, ALTERN_FUNC2);
	HAL_GPIO_ModeConfig(PF, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PF, 7, ALTERN_FUNC2);
#else
	// Test Pin setting PD0-SCL0 / PD1-SDA0
	HAL_GPIO_ModeConfig(PD, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 0, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PD, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 1, ALTERN_FUNC1);
#endif

#if 1	// Port Debounce Setting
	SCU->MCCR4 |= (1<<0) | (1<<16);
//	HAL_GPIO_SetDebouncePin(PD, 0);
//	HAL_GPIO_SetDebouncePin(PD, 1);
	HAL_GPIO_SetDebouncePin(PF, 6);
	HAL_GPIO_SetDebouncePin(PF, 7);
#endif


	/* I2C Init */
	HAL_I2C_Init(I2C_CH0, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);

	__enable_irq();

	while(1)
	{
		HAL_I2C_SetSlaveData(I2C_CH0);

		if(receiveBuffer0[8] == 0x07)
		{
			//PC5_Toggle_RUN();
		}
		if(receiveBuffer1[8] == 0x07)
		{
			//PC5_Toggle_RUN();
		}
	}
}

/**********************************************************************
 * @brief		Test Function
 * @param[in]	None
 * @return 	None
 **********************************************************************/
static void I2C_MASTER_TEST(void)
{
	uint8_t i;
	uint8_t u8TxDat[8];	// I2C TX Buffer
	uint8_t u8RxDat[8];	// I2C RX Buffer

	// Test Pin setting PA1-SCL1 / PA0-SDA1
	HAL_GPIO_ModeConfig(PA, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 1, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PA, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PA, 0, ALTERN_FUNC1);

#if 0
	// Test Pin setting PF6-SCL0 / PF7-SDA0
	HAL_GPIO_ModeConfig(PF, 6, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PF, 6, ALTERN_FUNC2);
	HAL_GPIO_ModeConfig(PF, 7, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PF, 7, ALTERN_FUNC2);
#else
	// Test Pin setting PD0-SCL0 / PD1-SDA0
	HAL_GPIO_ModeConfig(PD, 0, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 0, ALTERN_FUNC1);
	HAL_GPIO_ModeConfig(PD, 1, ALTERN_FUNC);
	HAL_GPIO_AlternativeConfig(PD, 1, ALTERN_FUNC1);
#endif

#if 1	// Port Debounce Setting
	SCU->MCCR4 |= (1<<0) | (1<<16);
	HAL_GPIO_SetDebouncePin(PD, 0);
	HAL_GPIO_SetDebouncePin(PD, 1);
#endif
	

	/* I2C Init */
	HAL_I2C_Init(I2C_CH0, I2C_SPEED, I2C_SLAVE_OWN_ADDR, I2C_ACK_ENABLE);

	/* I2C Buffer Set */
	for(i=0;i<8;i++)
	{
		u8TxDat[i] = (i + 0x00);
		u8RxDat[i] = 0x00;
	}
	__enable_irq();
	
#if 1	// Master mode
	/* I2C Master Write 8-byte */
	HAL_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, u8TxDat, 8, u8RxDat, 0);

	/* I2C Master Read 8-byte */
	HAL_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, u8TxDat, 0, u8RxDat, 8);

	/* I2C Master Write 1-byte wordAddr then Read 8-byte */
	HAL_I2C_MasterTransferData(I2C_CH0, I2C_DEVICE_ADDRESS, u8TxDat, 1, u8RxDat, 8);
#else	// Slave mode
	while(1)
	{
		HAL_I2C_SetSlaveData(I2C_CH0);
	}
#endif
}
