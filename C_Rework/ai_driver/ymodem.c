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
#define PACKET_HEADER     3    /* SOH/STX + SEQ + SEQ_INV */
#define PACKET_TRAILER    2    /* CRC16 */

/* 타임아웃 설정 (밀리초) */
#define TIMEOUT_INITIAL   3000
#define TIMEOUT_PACKET    1000
#define TIMEOUT_BYTE      1000

/* 재시도 횟수 */
#define MAX_ERRORS        10

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
 * @brief Ymodem 수신 구현
 */
ymodem_status_t ymodem_receive(uint8_t *buffer, uint32_t max_size, ymodem_result_t *result)
{
    uint8_t packet_buf[PACKET_SIZE_1024];
    uint8_t expected_seq = 0;
    uint32_t total_received = 0;
    uint32_t file_size = 0;
    int error_count = 0;
    int packet_errors = 0;
    ymodem_status_t status;
    
    if (!buffer || !result || max_size == 0) {
        return YMODEM_ERROR;
    }
    
    memset(result, 0, sizeof(ymodem_result_t));
    
    /* 첫 번째 패킷(헤더) 수신 */
    send_control(CRC);
    
    while (error_count < MAX_ERRORS) {
        uint32_t packet_size;
        uint8_t seq_num;
        
        status = receive_packet(packet_buf, &packet_size, &seq_num);
        
        if (status == YMODEM_CANCEL) {
            return YMODEM_CANCEL;
        }
        
        if (status != YMODEM_OK) {
            send_control(CRC);
            error_count++;
            continue;
        }
        
        if (seq_num != 0) {
            send_control(CRC);
            error_count++;
            continue;
        }
        
        /* 빈 파일명 = 배치 종료 */
        if (packet_buf[0] == 0) {
            send_control(ACK);
            return YMODEM_OK;
        }
        
        /* 파일명 추출 */
        int name_len = 0;
        while (name_len < PACKET_SIZE_128 &&
               name_len < (int)sizeof(result->filename) - 1 &&
               packet_buf[name_len] != 0) {
            result->filename[name_len] = packet_buf[name_len];
            name_len++;
        }
        result->filename[name_len] = '\0';
        
        /* 파일 크기 추출 */
        if (name_len < PACKET_SIZE_128 - 1) {
            char *p = (char *)&packet_buf[name_len + 1];
            while (*p == ' ') p++;  /* 공백 건너뛰기 */
            
            file_size = 0;
            int digits = 0;
            while (digits < 20 && *p >= '0' && *p <= '9') {
                file_size = file_size * 10 + (*p - '0');
                p++;
                digits++;
            }
        }
        
        send_control(ACK);
        send_control(CRC);
        expected_seq = 1;
        break;
    }
    
    if (error_count >= MAX_ERRORS) {
        abort_transfer();
        return YMODEM_TIMEOUT;
    }
    
    /* 데이터 패킷 수신 */
    while (packet_errors < MAX_ERRORS) {
        uint32_t packet_size;
        uint8_t seq_num;
        
        status = receive_packet(packet_buf, &packet_size, &seq_num);
        
        /* EOT 수신 */
        if (status == YMODEM_OK && packet_size == 0) {
            send_control(ACK);
            result->size = (file_size > 0 && file_size <= total_received) ? 
                          file_size : total_received;
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
        
        /* 복사할 크기 결정 */
        uint32_t copy_size = packet_size;
        
        if (file_size > 0 && total_received + packet_size > file_size) {
            copy_size = (file_size > total_received) ? 
                       (file_size - total_received) : 0;
        }
        
        /* 버퍼 오버플로우 체크 */
        if (total_received + copy_size > max_size) {
            abort_transfer();
            return YMODEM_BUFFER_FULL;
        }
        
        /* 데이터 복사 */
        if (copy_size > 0) {
            memcpy(buffer + total_received, packet_buf, copy_size);
            total_received += copy_size;
        }
        
        send_control(ACK);
        expected_seq++;
        packet_errors = 0;  /* 성공 시 에러 카운터 리셋 */
    }
    
    abort_transfer();
    return YMODEM_ERROR;
}

/**
 * @brief 패킷 송신
 */
static ymodem_status_t send_packet(const uint8_t *data, uint32_t size, uint8_t seq_num)
{
    uint8_t packet[PACKET_HEADER + PACKET_SIZE_1024 + PACKET_TRAILER];
    uint16_t packet_size = (size <= PACKET_SIZE_128) ? PACKET_SIZE_128 : PACKET_SIZE_1024;
    uint16_t crc;
    
    /* 헤더 */
    packet[0] = (packet_size == PACKET_SIZE_128) ? SOH : STX;
    packet[1] = seq_num;
    packet[2] = (uint8_t)(~seq_num);
    
    /* 데이터 및 패딩 */
    memcpy(&packet[PACKET_HEADER], data, size);
    if (size < packet_size) {
        memset(&packet[PACKET_HEADER + size], 0x1A, packet_size - size);
    }
    
    /* CRC */
    crc = crc16(&packet[PACKET_HEADER], packet_size);
    packet[PACKET_HEADER + packet_size] = (uint8_t)(crc >> 8);
    packet[PACKET_HEADER + packet_size + 1] = (uint8_t)(crc & 0xFF);
    
    /* 전송 시도 */
    for (int retry = 0; retry < MAX_ERRORS; retry++) {
        /* 패킷 송신 */
        int tx_ok = 1;
        for (uint32_t i = 0; i < PACKET_HEADER + packet_size + PACKET_TRAILER; i++) {
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
            /* NAK이면 재전송 */
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
    
    /* 'C' 대기 */
    while (init_retry < 30) {
        if (uart_receive_byte(&response, 100) == 0) {
            if (response == CRC) {
                break;
            }
            if (response == CAN) {
                return YMODEM_CANCEL;
            }
        }
        init_retry++;
    }
    
    if (init_retry >= 30) {
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
    while (init_retry < 10) {
        if (uart_receive_byte(&response, 100) == 0 && response == CRC) {
            break;
        }
        init_retry++;
    }
    
    if (init_retry >= 10) {
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