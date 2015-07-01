#ifndef _DEVICE_H
#define _DEVICE_H
/** \file device.h
 * Definition of the generic device_t type.
 */

/* ***** INCLUDES ***** */
#include <stdint.h>
#include <stdbool.h>
#include "lock.h"
#include "list.h"

/* ***** TYPES ***** */

/**
 * Supported device types.
 */
typedef enum _device_type_t {
    DEVICE_I2C = 0, /** I2C device */
    DEVICE_USART,   /** USART device */
    DEVICE_TIME,    /** TIME device */
    DEVICE_SPI,     /** SPI device */
} device_type_t;

/**
 * Device states
 */
typedef enum _device_state_t {
    UNINITIALIZED = 0,
    INITIALIZED,
    READY,
    BUSY,
} device_state_t;

/**
 *  Possible operations on generic device
 */
typedef struct _device_ops_t {
    int32_t (*open)(device_t *dev, void *params);                     /** Open a device */
    int32_t (*close)(device_t *dev);                                  /** Close a device */
    int32_t (*suspend)(device_t *dev);                                /** Power management: suspend */
    int32_t (*resume)(device_t *dev);                                 /** Power management: suspend */
    int32_t (*attach)(device_t *dev, device_t *drv);                  /** Attach driver drv to device dev */
    int32_t (*ioctl)(device_t *dev, device_ioctl_t id, void *args);   /** Interfere with configuration of the device through unified API */
    int32_t (*send)(device_t *dev, uint8_t *data, uint16_t data_len); /** Transmit data to the underlying device */
    int32_t (*recv)(device_t *dev, uint8_t *data, uint16_t data_len); /** Receive data from the underlying device */
} device_ops_t;

/**
 * Structure describing a device.
 */
typedef struct _device_t {
    char device_name[16]; /** Device name */
    device_type_t type;   /** Device type */
    device_state_t state; /** Device state */

    lock_t lock;          /** Device protection against multiple access */
    uint32_t irq;         /** IRQ id */
    bool use_dma;         /** Indication on dma usage by device */
    bool blocking;        /** If blocking is set, set to polling mode */

    list_t *drv;          /**  Driver using this device */

    device_t *dev;        /** Underlying device to use */

    void *private;        /** Device specific */

    /* Basic operation on devices */
    device_ops_t *ops;   /** Device operations */
} device_t;

/* ***** PUBLIC METHODS ***** */
int32_t device_init(device_t *dev, device_type_t type, const char *devname, device_ops_t *ops);

int32_t device_open(device_t *dev, void *params);

int32_t device_close(device_t *dev);

int32_t device_suspend(device_t dev);

int32_t device_attach(device_t *dev, device_t *drv);

int32_t ioctl(device_t *dev, device_ioctl_t id, void *args);

#endif /* ! _DEVICE_H */
