#ifndef YMODEM_H
#define YMODEM_H

#include <stdint.h>
#include <stddef.h>

/* Ymodem protocol status codes */
typedef enum {
    YMODEM_OK = 0,              /* Success */
    YMODEM_ERROR = -1,          /* General error */
    YMODEM_TIMEOUT = -2,        /* Timeout */
    YMODEM_CANCEL = -3,         /* Transfer cancelled */
    YMODEM_INVALID_PACKET = -4, /* Invalid packet */
    YMODEM_ABORT = -5           /* User abort */
} ymodem_status_t;

/* Ymodem file information */
typedef struct {
    char filename[128];  /* Filename */
    uint32_t file_size;  /* Total file size (0 if unknown) */
} ymodem_file_info_t;

/**
 * @brief Packet data handler callback
 * 
 * @param data Received data
 * @param size Data size
 * @param offset Offset in file
 * @return 0: continue, -1: abort
 * 
 * @note Use this callback to write to Flash
 */
typedef int (*ymodem_packet_handler_t)(const uint8_t *data, uint32_t size, uint32_t offset);

/**
 * @brief Receive data via Ymodem protocol
 * 
 * @param handler Callback function called for each packet
 * @param file_info File information (output)
 * @return ymodem_status_t Reception status
 * 
 * @note Handler is called for each packet to process data
 */
ymodem_status_t ymodem_receive(ymodem_packet_handler_t handler, ymodem_file_info_t *file_info);

/**
 * @brief Transmit data via Ymodem protocol
 * 
 * @param data Data buffer to transmit
 * @param size Data size
 * @param filename Filename (can be NULL)
 * @return ymodem_status_t Transmission status
 */
ymodem_status_t ymodem_transmit(const uint8_t *data, uint32_t size, const char *filename);

/* Platform-dependent functions - must be implemented by user */

/**
 * @brief Receive one byte via UART (with timeout)
 * 
 * @param data Pointer to store received data
 * @param timeout_ms Timeout in milliseconds
 * @return int 0: success, -1: timeout/error
 */
extern int uart_receive_byte(uint8_t *data, uint32_t timeout_ms);

/**
 * @brief Transmit one byte via UART
 * 
 * @param data Data to transmit
 * @return int 0: success, -1: error
 */
extern int uart_transmit_byte(uint8_t data);

#endif /* YMODEM_H */