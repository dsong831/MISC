#ifndef YMODEM_H
#define YMODEM_H

#include <stdint.h>
#include <stddef.h>

/* Ymodem 프로토콜 상태 코드 */
typedef enum {
    YMODEM_OK = 0,              /* 성공 */
    YMODEM_ERROR = -1,          /* 일반 에러 */
    YMODEM_TIMEOUT = -2,        /* 타임아웃 */
    YMODEM_CANCEL = -3,         /* 전송 취소 */
    YMODEM_INVALID_PACKET = -4, /* 잘못된 패킷 */
    YMODEM_BUFFER_FULL = -5     /* 버퍼 오버플로우 */
} ymodem_status_t;

/* Ymodem 수신 결과 구조체 */
typedef struct {
    uint32_t size;      /* 실제 수신된 데이터 크기 */
    char filename[128]; /* 파일명 (선택사항) */
} ymodem_result_t;

/**
 * @brief Ymodem 프로토콜로 데이터 수신
 * 
 * @param buffer 데이터를 저장할 버퍼
 * @param max_size 버퍼의 최대 크기
 * @param result 수신 결과 정보 (파일명, 실제 크기)
 * @return ymodem_status_t 수신 상태
 * 
 * @note 모든 데이터를 수신한 후 반환됩니다.
 */
ymodem_status_t ymodem_receive(uint8_t *buffer, uint32_t max_size, ymodem_result_t *result);

/**
 * @brief Ymodem 프로토콜로 데이터 송신
 * 
 * @param data 전송할 데이터 버퍼
 * @param size 전송할 데이터 크기
 * @param filename 파일명 (NULL 가능)
 * @return ymodem_status_t 송신 상태
 */
ymodem_status_t ymodem_transmit(const uint8_t *data, uint32_t size, const char *filename);

/* 플랫폼 의존 함수들 - 사용자가 구현해야 함 */

/**
 * @brief UART로 1바이트 수신 (타임아웃 포함)
 * 
 * @param data 수신된 데이터를 저장할 포인터
 * @param timeout_ms 타임아웃 (밀리초)
 * @return int 0: 성공, -1: 타임아웃/에러
 */
extern int uart_receive_byte(uint8_t *data, uint32_t timeout_ms);

/**
 * @brief UART로 1바이트 송신
 * 
 * @param data 송신할 데이터
 * @return int 0: 성공, -1: 에러
 */
extern int uart_transmit_byte(uint8_t data);

#endif /* YMODEM_H */