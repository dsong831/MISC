#include "i2c.h"

#define I2C_BASE_ADDRESS 0x40010000  // Replace with actual base address
#define I2C ((I2C_TypeDef*)I2C_BASE_ADDRESS)

// ... (previous code remains the same)

// Global variables for slave operation
static uint8_t *slave_tx_buffer;
static uint8_t *slave_rx_buffer;
static uint32_t slave_tx_length;
static uint32_t slave_rx_length;
static uint32_t slave_tx_index;
static uint32_t slave_rx_index;

void I2C_SlaveInterruptHandler(I2C_TypeDef *I2Cx) {
    uint32_t status = I2Cx->I2CS_INTERRUPT_DONE_RO;

    // Start condition
    if (status & 0x1) {
        slave_tx_index = 0;
        slave_rx_index = 0;
        if (slave_tx_buffer && slave_tx_length > 0) {
            I2Cx->I2CS_SEND_RDATA = slave_tx_buffer[0];
        }
    }

    // Address update
    if (status & 0x4) {
        // Handle address update if needed
        // uint32_t received_addr = I2Cx->I2CS_RECV_ADDRESS;
    }

    // Write (master writing to slave)
    if (status & 0x8) {
        if (slave_rx_buffer && slave_rx_index < slave_rx_length) {
            slave_rx_buffer[slave_rx_index++] = I2Cx->I2CS_RECV_WDATA;
        }
    }

    // Read (master reading from slave)
    if (status & 0x10) {
        if (slave_tx_buffer && slave_tx_index < slave_tx_length) {
            I2Cx->I2CS_SEND_RDATA = slave_tx_buffer[slave_tx_index++];
        } else {
            I2Cx->I2CS_SEND_RDATA = 0xFF;  // Send dummy byte if no more data
        }
    }

    // Stop condition
    if (status & 0x2) {
        slave_tx_index = 0;
        slave_rx_index = 0;
    }

    // Device address update (typically rare)
    if (status & 0x20) {
        // Handle device address update if needed
        // uint32_t new_dev_addr = I2Cx->I2CS_RECV_DEV_ADDR;
    }

    // Clear all handled interrupts
    I2Cx->I2CS_INTERRUPT_CLEAR = status;
}

void I2C_SlaveTransmit(I2C_TypeDef *I2Cx, uint8_t *data, uint32_t length) {
    slave_tx_buffer = data;
    slave_tx_length = length;
    slave_tx_index = 0;
    
    if (length > 0) {
        I2Cx->I2CS_SEND_RDATA = data[0];
    }
}

void I2C_SlaveReceive(I2C_TypeDef *I2Cx, uint8_t *data, uint32_t length) {
    slave_rx_buffer = data;
    slave_rx_length = length;
    slave_rx_index = 0;
}

// ... (rest of the previous code remains the same)