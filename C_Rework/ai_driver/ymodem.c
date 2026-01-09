#include "ymodem.h"
#include <string.h>

/* Ymodem protocol control characters */
#define SOH  0x01  /* Start of 128-byte block */
#define STX  0x02  /* Start of 1024-byte block */
#define EOT  0x04  /* End of transmission */
#define ACK  0x06  /* Acknowledge */
#define NAK  0x15  /* Negative acknowledge */
#define CAN  0x18  /* Cancel */
#define CRC  0x43  /* 'C' - CRC mode request */

/* Packet sizes */
#define PACKET_SIZE_128   128
#define PACKET_SIZE_1024  1024

/* Retry counts (platform independent) */
#define RETRY_INIT_HANDSHAKE  60   /* Initial 'C' handshake retry */
#define RETRY_PACKET_RX       3    /* Packet reception retry */
#define RETRY_ERROR_MAX       10   /* Maximum error count */

/* Timeout values (milliseconds) - used with uart_receive_byte */
#define TIMEOUT_RX_BYTE       100  /* Per-byte timeout */
#define TIMEOUT_RX_FIRST      500  /* First byte of packet timeout */

/* Internal functions */
static uint16_t crc16(const uint8_t *data, uint16_t length);
static ymodem_status_t receive_packet(uint8_t *buffer, uint32_t *packet_size, uint8_t *seq_num);
static ymodem_status_t send_packet(const uint8_t *data, uint32_t size, uint8_t seq_num);
static void send_control(uint8_t control);
static void abort_transfer(void);

/**
 * @brief Calculate CRC16-CCITT
 */
static uint16_t crc16(const uint8_t *data, uint16_t length)
{
    uint16_t crc = 0;
    
    while (length--) {
        crc ^= (uint16_t)(*data++) << 8;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

/**
 * @brief Send control character
 */
static void send_control(uint8_t control)
{
    uart_transmit_byte(control);
}

/**
 * @brief Abort transfer
 */
static void abort_transfer(void)
{
    send_control(CAN);
    send_control(CAN);
}

/**
 * @brief Receive one packet
 */
static ymodem_status_t receive_packet(uint8_t *buffer, uint32_t *packet_size, uint8_t *seq_num)
{
    uint8_t header, seq, seq_inv;
    uint16_t data_size, crc_recv, crc_calc;
    uint8_t crc_high, crc_low;
    
    /* Receive header with retry */
    int retry = 0;
    while (retry < RETRY_PACKET_RX) {
        if (uart_receive_byte(&header, TIMEOUT_RX_FIRST) == 0) {
            break;
        }
        retry++;
    }
    
    if (retry >= RETRY_PACKET_RX) {
        return YMODEM_TIMEOUT;
    }
    
    /* Handle special characters */
    if (header == EOT) {
        *packet_size = 0;
        return YMODEM_OK;
    }
    
    if (header == CAN) {
        uint8_t second;
        if (uart_receive_byte(&second, TIMEOUT_RX_BYTE) == 0 && second == CAN) {
            return YMODEM_CANCEL;
        }
        return YMODEM_INVALID_PACKET;
    }
    
    /* Determine packet size */
    if (header == SOH) {
        data_size = PACKET_SIZE_128;
    } else if (header == STX) {
        data_size = PACKET_SIZE_1024;
    } else {
        return YMODEM_INVALID_PACKET;
    }
    
    /* Receive and verify sequence number */
    if (uart_receive_byte(&seq, TIMEOUT_RX_BYTE) != 0 ||
        uart_receive_byte(&seq_inv, TIMEOUT_RX_BYTE) != 0) {
        return YMODEM_TIMEOUT;
    }
    
    if ((uint8_t)(~seq) != seq_inv) {
        return YMODEM_INVALID_PACKET;
    }
    
    /* Receive data */
    for (uint16_t i = 0; i < data_size; i++) {
        if (uart_receive_byte(&buffer[i], TIMEOUT_RX_BYTE) != 0) {
            return YMODEM_TIMEOUT;
        }
    }
    
    /* Receive and verify CRC */
    if (uart_receive_byte(&crc_high, TIMEOUT_RX_BYTE) != 0 ||
        uart_receive_byte(&crc_low, TIMEOUT_RX_BYTE) != 0) {
        return YMODEM_TIMEOUT;
    }
    
    crc_recv = ((uint16_t)crc_high << 8) | crc_low;
    crc_calc = crc16(buffer, data_size);
    
    if (crc_recv != crc_calc) {
        return YMODEM_INVALID_PACKET;
    }
    
    *seq_num = seq;
    *packet_size = data_size;
    return YMODEM_OK;
}

/**
 * @brief Ymodem receive implementation (callback-based)
 */
ymodem_status_t ymodem_receive(ymodem_packet_handler_t handler, ymodem_file_info_t *file_info)
{
    uint8_t packet_buf[PACKET_SIZE_1024];
    uint8_t expected_seq = 0;
    uint32_t data_offset = 0;
    int init_count = 0;
    int error_count = 0;
    ymodem_status_t status;
    
    if (!handler || !file_info) {
        return YMODEM_ERROR;
    }
    
    memset(file_info, 0, sizeof(ymodem_file_info_t));
    
    /* Receive first packet (header) - send 'C' periodically */
    while (init_count < RETRY_INIT_HANDSHAKE) {
        send_control(CRC);
        
        uint32_t packet_size;
        uint8_t seq_num;
        
        status = receive_packet(packet_buf, &packet_size, &seq_num);
        
        if (status == YMODEM_CANCEL) {
            return YMODEM_CANCEL;
        }
        
        /* Header packet must have sequence 0 */
        if (status == YMODEM_OK && seq_num == 0) {
            /* Empty filename = batch end */
            if (packet_buf[0] == 0) {
                send_control(ACK);
                return YMODEM_OK;
            }
            
            /* Extract filename */
            int name_len = 0;
            while (name_len < PACKET_SIZE_128 &&
                   name_len < (int)sizeof(file_info->filename) - 1 &&
                   packet_buf[name_len] != 0) {
                file_info->filename[name_len] = packet_buf[name_len];
                name_len++;
            }
            file_info->filename[name_len] = '\0';
            
            /* Extract file size */
            if (name_len < PACKET_SIZE_128 - 1 && packet_buf[name_len + 1] != 0) {
                char *p = (char *)&packet_buf[name_len + 1];
                while (*p == ' ') p++;
                
                file_info->file_size = 0;
                int digits = 0;
                while (digits < 20 && *p >= '0' && *p <= '9') {
                    file_info->file_size = file_info->file_size * 10 + (*p - '0');
                    p++;
                    digits++;
                }
            }
            
            send_control(ACK);
            send_control(CRC);
            expected_seq = 1;
            break;
        }
        
        init_count++;
    }
    
    if (init_count >= RETRY_INIT_HANDSHAKE) {
        abort_transfer();
        return YMODEM_TIMEOUT;
    }
    
    /* Receive data packets and process via callback */
    error_count = 0;
    
    while (error_count < RETRY_ERROR_MAX) {
        uint32_t packet_size;
        uint8_t seq_num;
        
        status = receive_packet(packet_buf, &packet_size, &seq_num);
        
        /* EOT received */
        if (status == YMODEM_OK && packet_size == 0) {
            send_control(ACK);
            return YMODEM_OK;
        }
        
        if (status == YMODEM_CANCEL) {
            return YMODEM_CANCEL;
        }
        
        if (status != YMODEM_OK) {
            send_control(NAK);
            error_count++;
            continue;
        }
        
        /* Verify sequence (with overflow consideration) */
        if (seq_num != expected_seq) {
            /* Duplicate packet (previous ACK was lost) */
            if (seq_num == (uint8_t)(expected_seq - 1)) {
                send_control(ACK);
            } else {
                send_control(NAK);
                error_count++;
            }
            continue;
        }
        
        /* Calculate valid data size */
        uint32_t valid_size = packet_size;
        
        if (file_info->file_size > 0) {
            if (data_offset >= file_info->file_size) {
                valid_size = 0;
            } else if (data_offset + packet_size > file_info->file_size) {
                valid_size = file_info->file_size - data_offset;
            }
        }
        
        /* Call handler - Flash write, etc. */
        if (valid_size > 0) {
            if (handler(packet_buf, valid_size, data_offset) != 0) {
                abort_transfer();
                return YMODEM_ABORT;
            }
            data_offset += valid_size;
        }
        
        send_control(ACK);
        expected_seq++;
        error_count = 0;  /* Reset error count on success */
    }
    
    abort_transfer();
    return YMODEM_ERROR;
}

/**
 * @brief Send one packet
 */
static ymodem_status_t send_packet(const uint8_t *data, uint32_t size, uint8_t seq_num)
{
    uint8_t packet[3 + PACKET_SIZE_1024 + 2];
    uint16_t packet_size = (size <= PACKET_SIZE_128) ? PACKET_SIZE_128 : PACKET_SIZE_1024;
    uint16_t crc;
    
    /* Header */
    packet[0] = (packet_size == PACKET_SIZE_128) ? SOH : STX;
    packet[1] = seq_num;
    packet[2] = (uint8_t)(~seq_num);
    
    /* Data and padding */
    memcpy(&packet[3], data, size);
    if (size < packet_size) {
        memset(&packet[3 + size], 0x1A, packet_size - size);
    }
    
    /* CRC */
    crc = crc16(&packet[3], packet_size);
    packet[3 + packet_size] = (uint8_t)(crc >> 8);
    packet[3 + packet_size + 1] = (uint8_t)(crc & 0xFF);
    
    /* Transmission attempts */
    for (int retry = 0; retry < RETRY_ERROR_MAX; retry++) {
        /* Send packet */
        int tx_ok = 1;
        for (uint32_t i = 0; i < 3 + packet_size + 2; i++) {
            if (uart_transmit_byte(packet[i]) != 0) {
                tx_ok = 0;
                break;
            }
        }
        
        if (!tx_ok) {
            return YMODEM_ERROR;
        }
        
        /* Wait for response */
        uint8_t response;
        int rx_retry = 0;
        while (rx_retry < RETRY_PACKET_RX) {
            if (uart_receive_byte(&response, TIMEOUT_RX_FIRST) == 0) {
                if (response == ACK) {
                    return YMODEM_OK;
                }
                if (response == CAN) {
                    return YMODEM_CANCEL;
                }
                break;
            }
            rx_retry++;
        }
    }
    
    return YMODEM_TIMEOUT;
}

/**
 * @brief Ymodem transmit implementation
 */
ymodem_status_t ymodem_transmit(const uint8_t *data, uint32_t size, const char *filename)
{
    uint8_t header_packet[PACKET_SIZE_128];
    uint8_t response;
    uint32_t offset = 0;
    uint8_t seq_num = 0;
    int init_count = 0;
    
    if (!data || size == 0) {
        return YMODEM_ERROR;
    }
    
    /* Wait for 'C' */
    while (init_count < RETRY_INIT_HANDSHAKE) {
        if (uart_receive_byte(&response, TIMEOUT_RX_FIRST) == 0) {
            if (response == CRC) {
                break;
            }
            if (response == CAN) {
                return YMODEM_CANCEL;
            }
        }
        init_count++;
    }
    
    if (init_count >= RETRY_INIT_HANDSHAKE) {
        return YMODEM_TIMEOUT;
    }
    
    /* Construct header packet */
    memset(header_packet, 0, sizeof(header_packet));
    
    if (filename) {
        int name_len = 0;
        while (filename[name_len] && name_len < 64) {
            header_packet[name_len] = filename[name_len];
            name_len++;
        }
        
        /* File size (ASCII) */
        char size_str[20];
        int len = 0;
        uint32_t tmp = size;
        
        do {
            size_str[len++] = '0' + (tmp % 10);
            tmp /= 10;
        } while (tmp > 0);
        
        /* Copy in reverse */
        for (int i = 0; i < len && name_len + 1 + i < 100; i++) {
            header_packet[name_len + 1 + i] = size_str[len - 1 - i];
        }
    }
    
    /* Send header */
    if (send_packet(header_packet, PACKET_SIZE_128, seq_num++) != YMODEM_OK) {
        abort_transfer();
        return YMODEM_ERROR;
    }
    
    /* Wait for 'C' again */
    init_count = 0;
    while (init_count < 20) {
        if (uart_receive_byte(&response, TIMEOUT_RX_FIRST) == 0 && response == CRC) {
            break;
        }
        init_count++;
    }
    
    if (init_count >= 20) {
        abort_transfer();
        return YMODEM_TIMEOUT;
    }
    
    /* Send data */
    while (offset < size) {
        uint32_t chunk = (size - offset > PACKET_SIZE_1024) ? 
                        PACKET_SIZE_1024 : (size - offset);
        
        if (send_packet(data + offset, chunk, seq_num++) != YMODEM_OK) {
            abort_transfer();
            return YMODEM_ERROR;
        }
        
        offset += chunk;
    }
    
    /* Send EOT and wait for ACK */
    send_control(EOT);
    
    init_count = 0;
    while (init_count < RETRY_PACKET_RX) {
        if (uart_receive_byte(&response, TIMEOUT_RX_FIRST) == 0 && response == ACK) {
            return YMODEM_OK;
        }
        init_count++;
    }
    
    return YMODEM_ERROR;
}