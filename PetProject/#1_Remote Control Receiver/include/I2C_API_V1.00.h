/*******************************************************************************
* Included File
*******************************************************************************/
#include <stdint.h>


/*******************************************************************************
* I2C Mode Selection
*******************************************************************************/
#if 1	// I2C Interrupt Mode
#define	I2C_INTERRUPT
#else	// I2C Polling Mode
#define	I2C_POLLING
#endif


/*******************************************************************************
* Public Macro
*******************************************************************************/
#define I2C_DEVICE_ADDRESS			0x40				// at master mode, slave device address
#define I2C_SLAVE_OWN_ADDR		0x40				// at slave mode, its own address
#define I2C_SPEED											10000			// i2c speed (10kHz)

#define I2C_MAX_BUFFER_SIZE		20					// i2c global buffer size (20-byte)
#define I2C_MAX_CHANNEL					2						// i2c count of channel (i2c0, i2c1)

#define I2C_CH0														0						// i2c0
#define I2C_CH1														1						// i2c1

#define I2C_ACK_DISABLE						0
#define I2C_ACK_ENABLE							1

#define I2C_WRITE_MODE						0
#define I2C_READ_MODE							1

#define I2C_IDLE													0
#define I2C_BUSY												1

#define I2C_TRUE												1
#define I2C_FALSE												0
/*******************************************************************************
* Public Typedef
*******************************************************************************/
/* I2C Control Register Flags */
enum  i2c_control_flags{
	fI2CnEN					= (1<<7),				// activate i2c block
	fTXDLYENBn		= (1<<6),				// data hold time register control
	fI2CnIEN					= (1<<5),				// i2c interrupt enable
	fI2CnIFLAG			= (1<<4),				// i2c interrupt flag
	fACKnEN					= (1<<3),				// control ack signal generation at ninth scl period
	fIMASTERn			= (1<<2),				// represents operation mode of i2c (master or slave)
	fSTOPCn				= (1<<1),				// stop condition generation when i2c is master
	fSTARTCn			= (1<<0),				// start condition generation when i2c is master
};
/* I2C Status Register Flags */
enum  i2c_status_flags{
	fGCALL						= (1<<7),				// general call is detected
	fTEND							= (1<<6),				// byte complete
	fSTOPD					= (1<<5),				// stop condition is detected
	fSSEL							= (1<<4),				// acts as a slave
	fMLOST					= (1<<3),				// lost bus mastership
	fBUSY						= (1<<2),				// i2c bus is busy
	fTMODE					= (1<<1),				// i2c is transmitter
	fRXACK						= (1<<0),				// ack is received at ninth scl period
};

/*******************************************************************************
* Exported Public Function
*******************************************************************************/
void USI_I2C_Initial(uint8_t ch, uint32_t speed, uint8_t addr, uint8_t ack);
void USI_I2C_Enable(uint8_t ch, uint8_t enable);
void USI_I2C_ConfigureGerneralCall(uint8_t ch, uint8_t enable);
void USI_I2C_ConfigureAcknowledge(uint8_t ch, uint8_t enable);
void USI_I2C_SoftwareReset(uint8_t ch, uint8_t enable);

void USI_I2C_MasterTransferData(uint8_t ch, uint8_t dev_addr, uint8_t *write_data, uint8_t write_len, uint8_t *read_data, uint8_t read_len);
void USI_I2C_ConfigureInterrupt(uint8_t ch, uint8_t enable);
uint8_t USI_I2C_GetInterruptStatus(uint8_t ch);
void USI_I2C_InterruptHandler(uint8_t ch);

void USI_I2C_SetSlaveData(uint8_t ch);
