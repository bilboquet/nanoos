#ifndef _UART_H
#define _UART_H

/* ***** INCLUDES ***** */
#include "common.h"
#include "device.h"
#include "types.h"

/* ***** TYPES ***** */
typedef enum _usart_id_t {
    ID_USART1,
    ID_USART2
} usart_id_t;

typedef enum _uart_parity_t {
    UART_PARITY_NONE = 0,
    UART_PARITY_1,
    UART_PARITY_2,
} uart_parity_t;

typedef enum _uart_charlen_t {
    UART_CHARLEN_5BITS = 5,
    UART_CHARLEN_6BITS = 6,
    UART_CHARLEN_7BITS = 7,
    UART_CHARLEN_8BITS = 8,
    UART_CHARLEN_9BITS = 9,
} uart_charlen_t;

typedef enum _uart_stop_t {
    UART_STOP_1 = 1,
    UART_STOP_2,
    UART_STOP_1_5,
} uart_stop_t;

typedef enum _uart_baudrate_t {
    UART_BAUDRATE_110 = 0,
    UART_BAUDRATE_300,
    UART_BAUDRATE_1200,
    UART_BAUDRATE_2400,
    UART_BAUDRATE_4800,
    UART_BAUDRATE_9600,
    UART_BAUDRATE_19200,
    UART_BAUDRATE_38400,
    UART_BAUDRATE_57600,
    UART_BAUDRATE_115200,
    UART_BAUDRATE_230400,
    UART_BAUDRATE_460800,
    UART_BAUDRATE_921600,
    UART_BAUDRATE_1843200,
    UART_BAUDRATE_3686400,
} uart_baudrate_t;

/**
 * Parameters for an usart device.
 */
typedef struct _uart_private_t {
    uart_parity_t parity;
    uart_charlen_t charlen;
    uart_stop_t stop;
    uart_baudrate_t baudrate;
} uart_private_t;

typedef struct _device_uart_t device_uart_t;
typedef struct _device_uart_t {
    device_t base;
    int32_t (*open)(device_uart_t *uart, device_option_t options);                ///< Open a device
    int32_t (*close)(device_uart_t *uart);                                        ///< Close a device
    int32_t (*suspend)(device_uart_t *uart);                                      ///< Power management: suspend
    int32_t (*resume)(device_uart_t *uart);                                       ///< Power management: suspend

    int32_t (*ioctl)(device_uart_t *uart, device_ioctl_t ioctl_id, void *args); ///< Interfere with configuration of the device through unified API

    int32_t (*send)(device_uart_t * uart, const char *str, uint16_t len);
    int32_t (*recv)(device_uart_t * uart, const char *str, uint16_t len);
} device_uart_t;

/**
 * Possible operations on an usart device
 */
//extern const uart_ops_t uart_ops;
int32_t uart_init(device_uart_t *uart, const char *devname);
//int32_t uart_open(device_t *uart, device_option_t options);
//int32_t uart_close(device_t *uart);
//int32_t uart_suspend(device_t *uart);
//int32_t uart_resume(device_t *uart);
//int32_t uart_ioctl(device_t *uart);
//int32_t uart_send(device_t *uart, const char *str, uint16_t len);
//int32_t uart_recv(device_t *uart, const char *str, uint16_t len);

#endif /* ! _USART_H */
