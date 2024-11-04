#include "spic.h"

/* Flash configurations for different vendors */
static const flash_config_t flash_configs[] = {
    {
        .vendor_id = VENDOR_WINBOND,
        .write_cmd_4b = WINBOND_WRITE_4B,
        .write_cmd_3b = WINBOND_WRITE_3B,
        .quad_read_cmd = WINBOND_READ_QUAD,
        .quad_write_cmd = WINBOND_WRITE_QUAD,
        .quad_enable_bit = WINBOND_QUAD_BIT
    },
    {
        .vendor_id = VENDOR_MXIC,
        .write_cmd_4b = MXIC_WRITE_4B,
        .write_cmd_3b = MXIC_WRITE_3B,
        .quad_read_cmd = MXIC_READ_QUAD,
        .quad_write_cmd = MXIC_WRITE_QUAD,
        .quad_enable_bit = MXIC_QUAD_BIT
    },
    {
        .vendor_id = VENDOR_MICRON,
        .write_cmd_4b = MICRON_WRITE_4B,
        .write_cmd_3b = MICRON_WRITE_3B,
        .quad_read_cmd = 0,    // Micron might have different quad mode implementation
        .quad_write_cmd = 0,
        .quad_enable_bit = 0
    }
};

static flash_config_t current_flash;

/* Low level SPI functions - Implementation depends on your MCU */
static void spi_cs_low(void) {
    // Implementation needed
}

static void spi_cs_high(void) {
    // Implementation needed
}

static uint8_t spi_transfer(uint8_t data) {
    // Implementation needed
    return 0;
}

void spic_init(void) {
    // SPI initialization code
    uint32_t id = spic_read_id();
    
    // Find matching vendor configuration
    for (int i = 0; i < sizeof(flash_configs)/sizeof(flash_configs[0]); i++) {
        if ((id >> 16) == flash_configs[i].vendor_id) {
            current_flash = flash_configs[i];
            break;
        }
    }
}

uint32_t spic_read_id(void) {
    uint32_t id = 0;
    
    spi_cs_low();
    spi_transfer(CMD_READ_ID);
    id = (spi_transfer(0) << 16);
    id |= (spi_transfer(0) << 8);
    id |= spi_transfer(0);
    spi_cs_high();
    
    return id;
}

bool spic_is_busy(void) {
    uint8_t status;
    
    spi_cs_low();
    spi_transfer(CMD_READ_STATUS);
    status = spi_transfer(0);
    spi_cs_high();
    
    return (status & (1 << STATUS_BUSY_BIT)) ? true : false;
}

void spic_write_enable(void) {
    spi_cs_low();
    spi_transfer(CMD_WRITE_ENABLE);
    spi_cs_high();
}

bool spic_quad_read_enable(void) {
    uint8_t status;
    
    if (current_flash.quad_read_cmd == 0) {
        return false;  // Not supported by this vendor
    }
    
    spi_cs_low();
    spi_transfer(current_flash.quad_read_cmd);
    status = spi_transfer(0);
    spi_cs_high();
    
    // Set quad enable bit
    status |= (1 << current_flash.quad_enable_bit);
    
    spi_cs_low();
    spi_transfer(current_flash.quad_read_cmd);
    spi_transfer(status);
    spi_cs_high();
    
    return true;
}

bool spic_quad_write_enable(void) {
    uint8_t status;
    
    if (current_flash.quad_write_cmd == 0) {
        return false;  // Not supported by this vendor
    }
    
    spi_cs_low();
    spi_transfer(current_flash.quad_write_cmd);
    status = spi_transfer(0);
    spi_cs_high();
    
    // Set quad enable bit
    status |= (1 << current_flash.quad_enable_bit);
    
    spi_cs_low();
    spi_transfer(current_flash.quad_write_cmd);
    spi_transfer(status);
    spi_cs_high();
    
    return true;
}

static void send_address_3b(uint32_t addr) {
    spi_transfer((addr >> 16) & 0xFF);
    spi_transfer((addr >> 8) & 0xFF);
    spi_transfer(addr & 0xFF);
}

static void send_address_4b(uint32_t addr) {
    spi_transfer((addr >> 24) & 0xFF);
    spi_transfer((addr >> 16) & 0xFF);
    spi_transfer((addr >> 8) & 0xFF);
    spi_transfer(addr & 0xFF);
}

bool spic_read_data_3b(uint32_t addr, uint8_t *buffer, uint32_t len) {
    if (!buffer) return false;
    
    spi_cs_low();
    spi_transfer(CMD_READ_3B);
    send_address_3b(addr);
    
    while (len--) {
        *buffer++ = spi_transfer(0);
    }
    spi_cs_high();
    
    return true;
}

bool spic_read_data_4b(uint32_t addr, uint8_t *buffer, uint32_t len) {
    if (!buffer) return false;
    
    spi_cs_low();
    spi_transfer(CMD_READ_4B);
    send_address_4b(addr);
    
    while (len--) {
        *buffer++ = spi_transfer(0);
    }
    spi_cs_high();
    
    return true;
}

bool spic_write_data_3b(uint32_t addr, const uint8_t *buffer, uint32_t len) {
    if (!buffer) return false;
    
    spic_write_enable();
    
    spi_cs_low();
    spi_transfer(current_flash.write_cmd_3b);
    send_address_3b(addr);
    
    while (len--) {
        spi_transfer(*buffer++);
    }
    spi_cs_high();
    
    while(spic_is_busy());
    return true;
}

bool spic_write_data_4b(uint32_t addr, const uint8_t *buffer, uint32_t len) {
    if (!buffer) return false;
    
    spic_write_enable();
    
    spi_cs_low();
    spi_transfer(current_flash.write_cmd_4b);
    send_address_4b(addr);
    
    while (len--) {
        spi_transfer(*buffer++);
    }
    spi_cs_high();
    
    while(spic_is_busy());
    return true;
}

bool spic_erase(uint32_t addr, erase_size_t size) {
    uint8_t cmd;
    
    switch(size) {
        case ERASE_4K_3B:
            cmd = CMD_ERASE_4K_3B;
            break;
        case ERASE_4K_4B:
            cmd = CMD_ERASE_4K_4B;
            break;
        case ERASE_32K:
            cmd = CMD_ERASE_32K;
            break;
        default:
            return false;
    }
    
    spic_write_enable();
    
    spi_cs_low();
    spi_transfer(cmd);
    if (size == ERASE_4K_4B) {
        send_address_4b(addr);
    } else {
        send_address_3b(addr);
    }
    spi_cs_high();
    
    while(spic_is_busy());
    return true;
}