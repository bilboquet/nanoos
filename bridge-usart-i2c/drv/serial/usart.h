#ifndef _USART_H
#define _USART_H

/* ***** INCLUDES ***** */
#include "common.h"
#include "types.h"
#include "device.h"


/* ***** TYPES ***** */
typedef struct _device_usart_t {
	USART_TypeDef *hw;

	uint32_t baudrate;
};


/* ***** PUBLIC METHODS ***** */
int32_t usart_open(device_t *dev, uint32_t id, uint32_t baudrate);

int32_t usart_close(device_t *dev);

int32_t usart_send(device_t *dev, char *str);

int32_t usart_recv(device_t *dev, char *str, uint16_t length);

#endif /* ! _USART_H */
