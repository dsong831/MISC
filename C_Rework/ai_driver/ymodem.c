#include "ymodem.h"
#include <string.h>

/* Ymodem 프로토콜 제어 문자 */
#define SOH  0x01  /* 128바이트 블록 시작 */
#define STX  0x02  /* 1024바이트 블록 시작 */
#define EOT  0x04  /* 전송 종료 */
#define ACK  0x06  /* 긍정 응답 */
#define NAK  0x15  /* 부정 응답 */
#define CAN  0x18  /* 전송 취소 */
#define CRC  0x43  /* 'C' - CRC 모드 요청 */

/* 패킷 크기 */
#define PACKET_SIZE_128   128
#define PACKET_SIZE_1024  1024

/* 타임아웃 설정 (밀리초) - Teraterm용으로 길게 설정 */
#define TIMEOUT_INITIAL   30000  /* 초기 대기 30초 */
#define TIMEOUT_PACKET    5000   /* 패킷 대기 5초 */
#define TIMEOUT_BYTE      1000   /* 바이트 대기 1초 */

/* 재시도 횟수 */
#define MAX_ERRORS        10
#define MAX_INIT_RETRY    60     /* 1분간 시도 */

/* 내부 함수 선언 */
static uint16_t crc16(const uint8_t *data, uint16_t length);
static ymodem_status_t receive_packet(uint8_t *buffer, uint32_t *packet_size, uint8_t *seq_num);
static ymodem_status_t send_packet(const uint8_t *data, uint32_t size, uint8_t seq_num);
static void send_control(uint8_t control);
static void abort_transfer(void);

/**
 * @brief CRC16-CCITT 계산
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
 * @brief 제어 문자 송신
 */
static void send_control(uint8_t control)
{
    uart_transmit_byte(control);
}

/**
 * @brief 전송 중단
 */
static void abort_transfer(void)
{
    send_control(CAN);
    send_control(CAN);
}

/**
 * @brief 패킷 수신
 */
static ymodem_status_t receive_packet(uint8_t *buffer, uint32_t *packet_size, uint8_t *seq_num)
{
    uint8_t header, seq, seq_inv;
    uint16_t data_size, crc_recv, crc_calc;
    uint8_t crc_high, crc_low;
    
    /* 헤더 수신 */
    if (uart_receive_byte(&header, TIMEOUT_PACKET) != 0) {
        return YMODEM_TIMEOUT;
    }
    
    /* 특수 문자 처리 */
    if (header == EOT) {
        *packet_size = 0;
        return YMODEM_OK;
    }
    
    if (header == CAN) {
        uint8_t second;
        if (uart_receive_byte(&second, 500) == 0 && second == CAN) {
            return YMODEM_CANCEL;
        }
        return YMODEM_INVALID_PACKET;
    }
    
    /* 패킷 크기 결정 */
    if (header == SOH) {
        data_size = PACKET_SIZE_128;
    } else if (header == STX) {
        data_size = PACKET_SIZE_1024;
    } else {
        return YMODEM_INVALID_PACKET;
    }
    
    /* 시퀀스 번호 수신 및 검증 */
    if (uart_receive_byte(&seq, TIMEOUT_BYTE) != 0 ||
        uart_receive_byte(&seq_inv, TIMEOUT_BYTE) != 0) {
        return YMODEM_TIMEOUT;
    }
    
    if ((uint8_t)(~seq) != seq_inv) {
        return YMODEM_INVALID_PACKET;
    }
    
    /* 데이터 수신 */
    for (uint16_t i = 0; i < data_size; i++) {
        if (uart_receive_byte(&buffer[i], TIMEOUT_BYTE) != 0) {
            return YMODEM_TIMEOUT;
        }
    }
    
    /* CRC 수신 및 검증 */
    if (uart_receive_byte(&crc_high, TIMEOUT_BYTE) != 0 ||
        uart_receive_byte(&crc_low, TIMEOUT_BYTE) != 0) {
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
 * @brief Ymodem 수신 구현 (콜백 방식)
 */
ymodem_status_t ymodem_receive(ymodem_packet_handler_t handler, ymodem_file_info_t *file_info)
{
    uint8_t packet_buf[PACKET_SIZE_1024];
    uint8_t expected_seq = 0;
    uint32_t total_received = 0;
    int error_count = 0;
    int packet_errors = 0;
    ymodem_status_t status;
    
    if (!handler || !file_info) {
        return YMODEM_ERROR;
    }
    
    memset(file_info, 0, sizeof(ymodem_file_info_t));
    
    /* 첫 번째 패킷(헤더) 수신 - 능동적으로 'C' 반복 전송 */
    error_count = 0;
    while (error_count < MAX_INIT_RETRY) {
        send_control(CRC);
        
        uint32_t packet_size;
        uint8_t seq_num;
        
        status = receive_packet(packet_buf, &packet_size, &seq_num);
        
        if (status == YMODEM_CANCEL) {
            return YMODEM_CANCEL;
        }
        
        if (status == YMODEM_OK && seq_num == 0) {
            /* 빈 파일명 = 배치 종료 */
            if (packet_buf[0] == 0) {
                send_control(ACK);
                return YMODEM_OK;
            }
            
            /* 파일명 추출 */
            int name_len = 0;
            while (name_len < PACKET_SIZE_128 &&
                   name_len < (int)sizeof(file_info->filename) - 1 &&
                   packet_buf[name_len] != 0) {
                file_info->filename[name_len] = packet_buf[name_len];
                name_len++;
            }
            file_info->filename[name_len] = '\0';
            
            /* 파일 크기 추출 */
            if (name_len < PACKET_SIZE_128 - 1) {
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
        
        error_count++;
    }
    
    if (error_count >= MAX_INIT_RETRY) {
        abort_transfer();
        return YMODEM_TIMEOUT;
    }
    
    /* 데이터 패킷 수신 및 콜백 처리 */
    packet_errors = 0;
    while (packet_errors < MAX_ERRORS) {
        uint32_t packet_size;
        uint8_t seq_num;
        
        status = receive_packet(packet_buf, &packet_size, &seq_num);
        
        /* EOT 수신 */
        if (status == YMODEM_OK && packet_size == 0) {
            send_control(ACK);
            return YMODEM_OK;
        }
        
        if (status == YMODEM_CANCEL) {
            return YMODEM_CANCEL;
        }
        
        if (status != YMODEM_OK) {
            send_control(NAK);
            packet_errors++;
            continue;
        }
        
        /* 시퀀스 검증 */
        if (seq_num != expected_seq) {
            /* 중복 패킷 */
            if (seq_num == (uint8_t)(expected_seq - 1)) {
                send_control(ACK);
            } else {
                send_control(NAK);
                packet_errors++;
            }
            continue;
        }
        
        /* 유효한 데이터 크기 계산 */
        uint32_t valid_size = packet_size;
        if (file_info->file_size > 0) {
            if (total_received >= file_info->file_size) {
                valid_size = 0;
            } else if (total_received + packet_size > file_info->file_size) {
                valid_size = file_info->file_size - total_received;
            }
        }
        
        /* 콜백 호출 - Flash 쓰기 등 처리 */
        if (valid_size > 0) {
            if (handler(packet_buf, valid_size, total_received) != 0) {
                abort_transfer();
                return YMODEM_ABORT;
            }
            total_received += valid_size;
        }
        
        send_control(ACK);
        expected_seq++;
        packet_errors = 0;
    }
    
    abort_transfer();
    return YMODEM_ERROR;
}

/**
 * @brief 패킷 송신
 */
static ymodem_status_t send_packet(const uint8_t *data, uint32_t size, uint8_t seq_num)
{
    uint8_t packet[3 + PACKET_SIZE_1024 + 2];  /* header + data + crc */
    uint16_t packet_size = (size <= PACKET_SIZE_128) ? PACKET_SIZE_128 : PACKET_SIZE_1024;
    uint16_t crc;
    
    /* 헤더 */
    packet[0] = (packet_size == PACKET_SIZE_128) ? SOH : STX;
    packet[1] = seq_num;
    packet[2] = (uint8_t)(~seq_num);
    
    /* 데이터 및 패딩 */
    memcpy(&packet[3], data, size);
    if (size < packet_size) {
        memset(&packet[3 + size], 0x1A, packet_size - size);
    }
    
    /* CRC */
    crc = crc16(&packet[3], packet_size);
    packet[3 + packet_size] = (uint8_t)(crc >> 8);
    packet[3 + packet_size + 1] = (uint8_t)(crc & 0xFF);
    
    /* 전송 시도 */
    for (int retry = 0; retry < MAX_ERRORS; retry++) {
        /* 패킷 송신 */
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
        
        /* 응답 대기 */
        uint8_t response;
        if (uart_receive_byte(&response, TIMEOUT_PACKET) == 0) {
            if (response == ACK) {
                return YMODEM_OK;
            }
            if (response == CAN) {
                return YMODEM_CANCEL;
            }
        }
    }
    
    return YMODEM_TIMEOUT;
}

/**
 * @brief Ymodem 송신 구현
 */
ymodem_status_t ymodem_transmit(const uint8_t *data, uint32_t size, const char *filename)
{
    uint8_t header_packet[PACKET_SIZE_128];
    uint8_t response;
    uint32_t offset = 0;
    uint8_t seq_num = 0;
    int init_retry = 0;
    
    if (!data || size == 0) {
        return YMODEM_ERROR;
    }
    
    /* 'C' 대기 - Teraterm이 준비될 때까지 */
    while (init_retry < MAX_INIT_RETRY) {
        if (uart_receive_byte(&response, 500) == 0) {
            if (response == CRC) {
                break;
            }
            if (response == CAN) {
                return YMODEM_CANCEL;
            }
        }
        init_retry++;
    }
    
    if (init_retry >= MAX_INIT_RETRY) {
        return YMODEM_TIMEOUT;
    }
    
    /* 헤더 패킷 구성 */
    memset(header_packet, 0, sizeof(header_packet));
    
    if (filename) {
        int name_len = 0;
        while (filename[name_len] && name_len < 64) {
            header_packet[name_len] = filename[name_len];
            name_len++;
        }
        
        /* 파일 크기 (ASCII) */
        char size_str[20];
        int len = 0;
        uint32_t tmp = size;
        
        do {
            size_str[len++] = '0' + (tmp % 10);
            tmp /= 10;
        } while (tmp > 0);
        
        /* 역순 복사 */
        for (int i = 0; i < len && name_len + 1 + i < 100; i++) {
            header_packet[name_len + 1 + i] = size_str[len - 1 - i];
        }
    }
    
    /* 헤더 송신 */
    if (send_packet(header_packet, PACKET_SIZE_128, seq_num++) != YMODEM_OK) {
        abort_transfer();
        return YMODEM_ERROR;
    }
    
    /* 'C' 재대기 */
    init_retry = 0;
    while (init_retry < 20) {
        if (uart_receive_byte(&response, 500) == 0 && response == CRC) {
            break;
        }
        init_retry++;
    }
    
    if (init_retry >= 20) {
        abort_transfer();
        return YMODEM_TIMEOUT;
    }
    
    /* 데이터 송신 */
    while (offset < size) {
        uint32_t chunk = (size - offset > PACKET_SIZE_1024) ? 
                        PACKET_SIZE_1024 : (size - offset);
        
        if (send_packet(data + offset, chunk, seq_num++) != YMODEM_OK) {
            abort_transfer();
            return YMODEM_ERROR;
        }
        
        offset += chunk;
    }
    
    /* EOT 송신 및 ACK 대기 */
    send_control(EOT);
    if (uart_receive_byte(&response, TIMEOUT_PACKET) != 0 || response != ACK) {
        return YMODEM_ERROR;
    }
    
    return YMODEM_OK;
}