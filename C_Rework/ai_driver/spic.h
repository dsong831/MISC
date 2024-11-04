#ifndef __SPIC_H
#define __SPIC_H

#include <stdint.h>
#include <stdbool.h>

/* Flash Vendor IDs */
#define VENDOR_WINBOND     0xEF
#define VENDOR_MXIC        0xC2
#define VENDOR_MICRON      0x20
#define VENDOR_GIGADEVICE  0xC8

/* Common Flash Commands */
#define CMD_WRITE_ENABLE   0x06
#define CMD_READ_STATUS    0x05
#define CMD_READ_ID        0x9F

/* Flash Status Bits */
#define STATUS_BUSY_BIT    0x01
#define STATUS_WEL_BIT     0x02

/* Flash Parameters */
#define SECTOR_SIZE_4K     (4 * 1024)
#define BLOCK_SIZE_32K     (32 * 1024)
#define BLOCK_SIZE_64K     (64 * 1024)

typedef enum {
    ERASE_4K,
    ERASE_32K,
    ERASE_64K,
    ERASE_CHIP
} erase_size_t;

typedef struct {
    uint8_t vendor_id;
    uint8_t read_cmd_3b;
    uint8_t read_cmd_4b;
    uint8_t write_cmd_3b;
    uint8_t write_cmd_4b;
    uint8_t erase_4k_cmd;
    uint8_t erase_32k_cmd;
    uint8_t erase_64k_cmd;
    uint8_t chip_erase_cmd;
    uint8_t quad_enable_cmd;
    uint8_t quad_enable_status_bit;
} flash_config_t;

/* Function Prototypes */
void spic_init(void);
uint32_t spic_read_id(void);
bool spic_is_busy(void);
void spic_write_enable(void);
bool spic_quad_enable(void);

/* Read/Write Functions */
bool spic_read_data(uint32_t addr, uint8_t *buffer, uint32_t len);
bool spic_write_data(uint32_t addr, const uint8_t *buffer, uint32_t len);
bool spic_read_data_4b(uint32_t addr, uint8_t *buffer, uint32_t len);
bool spic_write_data_4b(uint32_t addr, const uint8_t *buffer, uint32_t len);

/* Erase Functions */
bool spic_erase(uint32_t addr, erase_size_t size);
bool spic_chip_erase(void);

#endif /* __SPIC_H */