#ifndef _USART_H
#define _USART_H

/* ***** INCLUDES ***** */
#include "../../../include/common.h"
#include "../../../include/device.h"
#include "../../../include/types.h"

/* ***** TYPES ***** */
typedef enum _usart_id_t {
	ID_USART1, ID_USART2
} usart_id_t;

typedef struct _device_usart_t {
	void *hw;

	uint32_t baudrate;
} device_usart_t;

typedef struct _device_op_usart_t {
	int32_t (*open)(device_t *, uint32_t, uint32_t);
	int32_t (*close)(device_t *);
	int32_t (*send)(device_t *, uint16_t);
	int32_t (*recv)(device_t *, uint8_t *, uint16_t);
} device_op_usart_t;

/* ***** PUBLIC METHODS ***** */
int32_t usart_init();
int32_t usart_open(device_t *dev, uint32_t id, uint32_t baudrate);

int32_t usart_close(device_t *dev);

int32_t usart_send(device_t *dev, uint16_t data);

int32_t usart_recv(device_t *dev, uint16_t *data);
//int32_t usart_recv(device_t *dev, uint8_t *str, uint16_t length);

#endif /* ! _USART_H */
