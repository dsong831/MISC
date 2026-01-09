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
    YMODEM_ABORT = -5           /* 사용자 중단 */
} ymodem_status_t;

/* Ymodem 수신 정보 구조체 */
typedef struct {
    char filename[128];  /* 파일명 */
    uint32_t file_size;  /* 전체 파일 크기 (0이면 미지정) */
} ymodem_file_info_t;

/**
 * @brief 패킷 데이터 처리 콜백 함수 타입
 * 
 * @param data 수신된 데이터
 * @param size 데이터 크기
 * @param offset 파일 내 오프셋 위치
 * @return 0: 계속, -1: 중단
 * 
 * @note 이 콜백에서 Flash 쓰기 등을 수행하세요
 */
typedef int (*ymodem_packet_handler_t)(const uint8_t *data, uint32_t size, uint32_t offset);

/**
 * @brief Ymodem 프로토콜로 데이터 수신
 * 
 * @param handler 패킷 수신 시 호출될 콜백 함수
 * @param file_info 파일 정보 (출력)
 * @return ymodem_status_t 수신 상태
 * 
 * @note 패킷마다 handler가 호출되어 데이터를 처리합니다
 */
ymodem_status_t ymodem_receive(ymodem_packet_handler_t handler, ymodem_file_info_t *file_info);

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