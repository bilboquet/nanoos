#ifndef _USART_H
#define _USART_H

/* ***** INCLUDES ***** */
#include "common.h"
#include "device.h"
#include "types.h"

/* ***** TYPES ***** */
typedef enum _usart_id_t {
    ID_USART1,
    ID_USART2
} usart_id_t;

/*typedef struct _device_usart_t {
    void *hw;

    uint32_t baudrate;
} device_usart_t;*/

typedef device_t device_usart_t;

typedef struct _device_op_usart_t {
    int32_t (*open)(device_t *, uint32_t, uint32_t);
    int32_t (*close)(device_t *);
    int32_t (*send)(device_t *, uint16_t);
    int32_t (*recv)(device_t *, uint8_t *, uint16_t);
} device_op_usart_t;

/* ***** PUBLIC METHODS ***** */
int32_t usart_init();

static inline int32_t usart_open(device_usart_t *dev, uint32_t id, uint32_t baudrate)
{
    return ((device_op_usart_t*) (dev->ops))->open(dev, id, baudrate);
}

static inline int32_t usart_close(device_t *dev)
{
    return ((device_op_usart_t*) (dev->ops))->close(dev);
}

int32_t usart_send(device_t *dev, uint16_t data)
{
    return ((device_op_usart_t*) (dev->ops))->send(dev, data);
}

int32_t usart_recv(device_t *dev, uint8_t *str, uint16_t length)
{
    return ((device_op_usart_t*) (dev->ops))->recv(dev, str, length);

}

#endif /* ! _USART_H */
