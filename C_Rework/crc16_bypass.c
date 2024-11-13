#include <stdint.h>

uint16_t crc16_buypass(uint8_t *data, size_t len) {
    uint16_t crc = 0x0000;
    uint16_t polynomial = 0x8005;

    for (size_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ polynomial;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}

int main() {
    uint8_t data[] = {1, 2, 3, 4};
    uint16_t result = crc16_buypass(data, sizeof(data));
    printf("CRC-16/BUYPASS result: 0x%04X\n", result);
    return 0;
}