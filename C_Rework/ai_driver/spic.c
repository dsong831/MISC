#include "spic.h"

/* Flash configurations for different vendors */
static const flash_config_t flash_configs[] = {
    {
        .vendor_id = VENDOR_WINBOND,
        .read_cmd_3b = 0x03,
        .read_cmd_4b = 0x13,
        .write_cmd_3b = 0x02,
        .write_cmd_4b = 0x12,
        .erase_4k_cmd = 0x20,
        .erase_32k_cmd = 0x52,
        .erase_64k_cmd = 0xD8,
        .chip_erase_cmd = 0xC7,
        .quad_enable_cmd = 0x01,
        .quad_enable_status_bit = 0x02
    },
    // Add other vendor configurations similarly
};

static flash_config_t current_flash;
static bool is_4byte_addressing = false;

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
    
    return (status & STATUS_BUSY_BIT) ? true : false;
}

void spic_write_enable(void) {
    spi_cs_low();
    spi_transfer(CMD_WRITE_ENABLE);
    spi_cs_high();
}

bool spic_quad_enable(void) {
    spic_write_enable();
    
    spi_cs_low();
    spi_transfer(current_flash.quad_enable_cmd);
    spi_transfer(current_flash.quad_enable_status_bit);
    spi_cs_high();
    
    while(spic_is_busy());
    return true;
}

static void send_address(uint32_t addr) {
    if (is_4byte_addressing) {
        spi_transfer((addr >> 24) & 0xFF);
    }
    spi_transfer((addr >> 16) & 0xFF);
    spi_transfer((addr >> 8) & 0xFF);
    spi_transfer(addr & 0xFF);
}

bool spic_read_data(uint32_t addr, uint8_t *buffer, uint32_t len) {
    if (!buffer) return false;
    
    spi_cs_low();
    spi_transfer(current_flash.read_cmd_3b);
    send_address(addr);
    
    while (len--) {
        *buffer++ = spi_transfer(0);
    }
    spi_cs_high();
    
    return true;
}

bool spic_write_data(uint32_t addr, const uint8_t *buffer, uint32_t len) {
    if (!buffer) return false;
    
    spic_write_enable();
    
    spi_cs_low();
    spi_transfer(current_flash.write_cmd_3b);
    send_address(addr);
    
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
        case ERASE_4K:
            cmd = current_flash.erase_4k_cmd;
            break;
        case ERASE_32K:
            cmd = current_flash.erase_32k_cmd;
            break;
        case ERASE_64K:
            cmd = current_flash.erase_64k_cmd;
            break;
        default:
            return false;
    }
    
    spic_write_enable();
    
    spi_cs_low();
    spi_transfer(cmd);
    send_address(addr);
    spi_cs_high();
    
    while(spic_is_busy());
    return true;
}

bool spic_chip_erase(void) {
    spic_write_enable();
    
    spi_cs_low();
    spi_transfer(current_flash.chip_erase_cmd);
    spi_cs_high();
    
    while(spic_is_busy());
    return true;
}