#include "i2c.h"

#define I2C_BASE_ADDRESS 0x40010000  // Replace with actual base address
#define I2C ((I2C_TypeDef*)I2C_BASE_ADDRESS)

void I2C_Init(I2C_TypeDef *I2Cx) {
    // Enable I2C master and slave
    I2Cx->I2CMS_EN = 0x101;  // Enable both master and slave

    // Configure default timing parameters
    I2Cx->I2CM_ONE_BIT_TIME = 0x0064;  // Example value, adjust as needed
    I2Cx->I2CM_BIT_P0 = 0x0019;
    I2Cx->I2CM_BIT_P1 = 0x0019;
    I2Cx->I2CM_BIT_P2 = 0x0019;
    I2Cx->I2CM_BIT_P3 = 0x0019;
    I2Cx->I2CM_LATCH_TIME = 0x0005;

    // Configure physical layer delays
    I2Cx->I2C_PHY_DELAY = 0x00010101;  // Example value, adjust as needed

    // Enable interrupts
    I2Cx->I2CM_INTERRUPT_EN = 0x7;  // Enable all master interrupts
    I2Cx->I2CS_INTERRUPT_EN = 0x3F;  // Enable all slave interrupts
}

void I2C_ConfigureMaster(I2C_TypeDef *I2Cx, I2C_Config *config) {
    I2Cx->I2CM_DEV_ID = config->device_id;
    I2Cx->I2CM_ADDRESS = config->address;
    I2Cx->I2CM_AUTO_LEN = config->data_len;

    uint32_t auto_cfg = 0;
    if (config->use_10bit_addr) {
        auto_cfg |= 1;  // Set 10-bit address mode
    }
    if (config->is_read) {
        auto_cfg |= (1 << 8);  // Set read operation
    }
    I2Cx->I2CM_AUTO_CFG = auto_cfg;
}

void I2C_ConfigureSlave(I2C_TypeDef *I2Cx, uint32_t slave_address) {
    // Configure slave address
    I2Cx->I2CS_DEVADDR_8bit = slave_address;
    
    // Enable 8-bit addressing mode
    I2Cx->I2CS_ADDR_MODE = 0x10000;
}

void I2C_MasterTransmit(I2C_TypeDef *I2Cx, uint8_t *data, uint32_t length) {
    // Write data to WDATA registers
    for (int i = 0; i < length && i < 16; i += 4) {
        uint32_t word = 0;
        for (int j = 0; j < 4 && (i + j) < length; j++) {
            word |= (data[i + j] << (8 * j));
        }
        (&I2Cx->I2CM_WDATA_0)[i / 4] = word;
    }

    // Start transmission
    I2Cx->I2CM_AUTO_START = 0x100;  // Set WREN bit
}

void I2C_MasterReceive(I2C_TypeDef *I2Cx, uint8_t *data, uint32_t length) {
    // Start reception
    I2Cx->I2CM_AUTO_START = 0x1;  // Set RDEN bit

    // Wait for completion (in practice, use interrupts)
    while (I2C_IsMasterBusy(I2Cx));

    // Read data from RDATA registers
    for (int i = 0; i < length && i < 16; i += 4) {
        uint32_t word = (&I2Cx->I2CM_RDATA_0)[i / 4];
        for (int j = 0; j < 4 && (i + j) < length; j++) {
            data[i + j] = (word >> (8 * j)) & 0xFF;
        }
    }
}

void I2C_SlaveTransmit(I2C_TypeDef *I2Cx, uint8_t *data, uint32_t length) {
    // In slave mode, we prepare the data to be sent when requested
    I2Cx->I2CS_SEND_RDATA = data[0];  // Prepare first byte
    // Rest of the data should be handled in the interrupt handler
}

void I2C_SlaveReceive(I2C_TypeDef *I2Cx, uint8_t *data, uint32_t length) {
    // In slave mode, data reception is handled in the interrupt handler
    // This function can be used to prepare the buffer
}

bool I2C_IsMasterBusy(I2C_TypeDef *I2Cx) {
    return (I2Cx->I2CM_INTERRUPT_DONE & 0x7) == 0;
}

bool I2C_IsSlaveSelected(I2C_TypeDef *I2Cx) {
    return (I2Cx->I2CS_INTERRUPT_DONE_RO & 0x1) != 0;
}

void I2C_ClearMasterInterrupt(I2C_TypeDef *I2Cx) {
    I2Cx->I2CM_INTERRUPT_CLR = 0x7;  // Clear all master interrupts
}

void I2C_ClearSlaveInterrupt(I2C_TypeDef *I2Cx) {
    I2Cx->I2CS_INTERRUPT_CLEAR = 0x3F;  // Clear all slave interrupts
}