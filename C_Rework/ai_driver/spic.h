#ifndef __SPIC_H
#define __SPIC_H

#include <stdint.h>
#include <stdbool.h>

/* Flash Vendor IDs */
#define VENDOR_WINBOND     0xEF
#define VENDOR_MXIC        0xC2
#define VENDOR_MICRON      0x20
#define VENDOR_GIGADEVICE  0xC8

/* Common Commands (All Vendors) */
#define CMD_READ_4B            0x6C    /* Read data with 4-byte address */
#define CMD_READ_3B            0x6B    /* Read data with 3-byte address */
#define CMD_READ_ID            0x9F    /* Check device ID */
#define CMD_READ_STATUS        0x05    /* Read status register */
#define CMD_WRITE_ENABLE      0x06    /* Write enable */
#define CMD_ERASE_4K_4B       0x21    /* 4KB erase with 4-byte address */
#define CMD_ERASE_4K_3B       0x20    /* 4KB erase with 3-byte address */
#define CMD_ERASE_32K         0x52    /* 32KB erase */

/* Winbond Specific Commands */
#define WINBOND_WRITE_4B      0x34    /* Write with 4-byte address */
#define WINBOND_WRITE_3B      0x32    /* Write with 3-byte address */
#define WINBOND_READ_QUAD_EN  0x35    /* Enable read quad command */
#define WINBOND_WRITE_QUAD_EN 0x31    /* Enable write quad command */
#define WINBOND_QUAD_BIT      0x01    /* Quad enable bit position */

/* MXIC Specific Commands */
#define MXIC_WRITE_4B         0x3E    /* Write with 4-byte address */
#define MXIC_WRITE_3B         0x38    /* Write with 3-byte address */
#define MXIC_QUAD_EN          0x01    /* Enable quad command */
#define MXIC_QUAD_BIT         0x06    /* Quad enable bit position */

/* Micron Specific Commands */
#define MICRON_WRITE_4B       0x34    /* Write with 4-byte address */
#define MICRON_WRITE_3B       0x32    /* Write with 3-byte address */

/* Status Register Bits */
#define STATUS_BUSY_BIT       0x00    /* Busy status bit position */

/* Flash Parameters */
#define SECTOR_SIZE_4K        (4 * 1024)
#define BLOCK_SIZE_32K        (32 * 1024)

typedef enum {
    ERASE_4K_3B,    /* 4KB erase with 3-byte address */
    ERASE_4K_4B,    /* 4KB erase with 4-byte address */
    ERASE_32K,      /* 32KB erase */
} erase_size_t;

typedef struct {
    uint8_t vendor_id;            /* Vendor identification */
    uint8_t write_cmd_4b;         /* Write command with 4-byte address */
    uint8_t write_cmd_3b;         /* Write command with 3-byte address */
    uint8_t quad_enable_cmd;      /* Quad mode enable command */
    uint8_t quad_enable_bit;      /* Quad mode status bit position */
} flash_config_t;

/* Function Prototypes */
void spic_init(void);
uint32_t spic_read_id(void);
bool spic_is_busy(void);
void spic_write_enable(void);
bool spic_quad_enable(void);

/* Read/Write Functions */
bool spic_read_data_3b(uint32_t addr, uint8_t *buffer, uint32_t len);
bool spic_read_data_4b(uint32_t addr, uint8_t *buffer, uint32_t len);
bool spic_write_data_3b(uint32_t addr, const uint8_t *buffer, uint32_t len);
bool spic_write_data_4b(uint32_t addr, const uint8_t *buffer, uint32_t len);

/* Erase Functions */
bool spic_erase(uint32_t addr, erase_size_t size);

/* Configuration Tables */
static const flash_config_t flash_configs[] = {
    {
        .vendor_id = VENDOR_WINBOND,
        .write_cmd_4b = WINBOND_WRITE_4B,
        .write_cmd_3b = WINBOND_WRITE_3B,
        .quad_enable_cmd = WINBOND_READ_QUAD_EN,
        .quad_enable_bit = WINBOND_QUAD_BIT
    },
    {
        .vendor_id = VENDOR_MXIC,
        .write_cmd_4b = MXIC_WRITE_4B,
        .write_cmd_3b = MXIC_WRITE_3B,
        .quad_enable_cmd = MXIC_QUAD_EN,
        .quad_enable_bit = MXIC_QUAD_BIT
    },
    {
        .vendor_id = VENDOR_MICRON,
        .write_cmd_4b = MICRON_WRITE_4B,
        .write_cmd_3b = MICRON_WRITE_3B,
        .quad_enable_cmd = 0,          /* Micron specific implementation required */
        .quad_enable_bit = 0
    }
};

#endif /* __SPIC_H */