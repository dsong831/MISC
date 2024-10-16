#ifndef PL230_I2C_H
#define PL230_I2C_H

#include <stdint.h>
#include <stdbool.h>

// I2C controller structure
typedef struct {
    volatile uint32_t I2CMS_EN;
    volatile uint32_t I2CM_MAN_CMD;
    volatile uint32_t I2CM_AUTO_CFG;
    volatile uint32_t I2CM_DEV_ID;
    volatile uint32_t I2CM_AUTO_START;
    volatile uint32_t I2CM_AUTO_LEN;
    volatile uint32_t I2CM_ADDRESS;
    volatile uint32_t I2CM_WDATA_0;
    volatile uint32_t I2CM_WDATA_1;
    volatile uint32_t I2CM_WDATA_2;
    volatile uint32_t I2CM_WDATA_3;
    volatile uint32_t I2CM_ONE_BIT_TIME;
    volatile uint32_t I2CM_BIT_P0;
    volatile uint32_t I2CM_BIT_P1;
    volatile uint32_t I2CM_BIT_P2;
    volatile uint32_t I2CM_BIT_P3;
    volatile uint32_t I2CM_LATCH_TIME;
    volatile uint32_t I2CM_RDATA_0;
    volatile uint32_t I2CM_RDATA_1;
    volatile uint32_t I2CM_RDATA_2;
    volatile uint32_t I2CM_RDATA_3;
    volatile uint32_t I2CM_MAN_RESP;
    volatile uint32_t I2CM_INTERRUPT_DONE;
    volatile uint32_t I2CM_INTERRUPT_EN;
    volatile uint32_t I2CM_INTERRUPT_CLR;
    volatile uint32_t I2CS_ADDR_MODE;
    volatile uint32_t I2CS_DEVADDR_16bit;
    volatile uint32_t I2CS_DEVADDR_eDP;
    volatile uint32_t I2CS_DEVADDR_8bit;
    volatile uint32_t I2CS_DEVADDR_11bit;
    volatile uint32_t I2C_PHY_DELAY;
    volatile uint32_t I2CS_SEND_RDATA;
    volatile uint32_t I2CS_RECV_DEV_ADDR;
    volatile uint32_t I2CS_RECV_ADDRESS;
    volatile uint32_t I2CS_RECV_WDATA;
    volatile uint32_t I2CS_ADDR_OFFSET;
    volatile uint32_t I2CS_INTERRUPT_DONE_RO;
    volatile uint32_t I2CS_INTERRUPT_EN;
    volatile uint32_t I2CS_INTERRUPT_CLEAR;
    volatile uint32_t I2CS_EDP_MAN_DEVADDR;
} I2C_TypeDef;

// I2C configuration structure
typedef struct {
    uint32_t device_id;
    uint32_t address;
    uint32_t data_len;
    bool is_read;
    bool use_10bit_addr;
} I2C_Config;

// Function prototypes
void I2C_Init(I2C_TypeDef *I2Cx);
void I2C_ConfigureMaster(I2C_TypeDef *I2Cx, I2C_Config *config);
void I2C_ConfigureSlave(I2C_TypeDef *I2Cx, uint32_t slave_address);
void I2C_MasterTransmit(I2C_TypeDef *I2Cx, uint8_t *data, uint32_t length);
void I2C_MasterReceive(I2C_TypeDef *I2Cx, uint8_t *data, uint32_t length);
void I2C_SlaveTransmit(I2C_TypeDef *I2Cx, uint8_t *data, uint32_t length);
void I2C_SlaveReceive(I2C_TypeDef *I2Cx, uint8_t *data, uint32_t length);
bool I2C_IsMasterBusy(I2C_TypeDef *I2Cx);
bool I2C_IsSlaveSelected(I2C_TypeDef *I2Cx);
void I2C_ClearMasterInterrupt(I2C_TypeDef *I2Cx);
void I2C_ClearSlaveInterrupt(I2C_TypeDef *I2Cx);

#endif // PL230_I2C_H