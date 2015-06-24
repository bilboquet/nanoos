#ifndef _DEVICE_H
#define _DEVICE_H

/* ***** INCLUDES ***** */
#include <stdint.h>
#include <stdbool.h>
#include "lock.h"
#include "list.h"

/* ***** TYPES ***** */
typedef enum _device_type_t {
    DEVICE_I2C = 0, /** I2C device */
    DEVICE_USART, /** USART device */
} device_type_t;

typedef enum _pm_state_t {
    PM_STATE_OFF = 0, /** Device OFF */
    PM_STATE_LOW_POWER, /** Device switch to low power mode */
    PM_STATE_FULL /** Device working at full power mode */
} device_pm_state_t;

typedef enum _device_state_t {
    UNINITIALIZED = 0,
    INITIALIZED,
    READY,
    DRV_OPENED,
    BUSY,
    DRV_CLOSED
} device_state_t;

typedef struct _device_t device_t;
typedef struct _device_t {
    /* Device name */
    char devname[16];
    /* Device type */
    device_type_t type;
    device_state_t state;

    lock_t lock; /* Device protection against multiple access */
    uint32_t irq; /* IRQ id */
    bool use_dma; /* Indication on dma usage by device */
    bool async; /* If async is set, asynchronous mode use */
    device_t *drv; /* Driver using this device */
    device_t *dev; /* Underlying device to use */

    void *private; /* Hardware specific */

    /* List of devices */
    list_t *list;

    /* Common operation on devices */
    int32_t (*open)(device_t *dev, void *params);
    int32_t (*close)(device_t *dev);
    int32_t (*pm)(device_t *dev, device_pm_state_t newstate);

    /* Device specific operation */
    void *ops;
} device_t;

/* ***** PUBLIC METHODS ***** */
/**
 * @brief Initialise a device
 * @param dev     Device to initialise
 * @param type    Device type
 * @param devname Device name
 */
int32_t device_init(device_t *dev, device_type_t type, const char *devname);

int32_t device_open(device_t *dev, void *hw_params);

int32_t device_close(device_t *dev);

int32_t device_pm(device_t *dev, device_pm_state_t state);

int32_t device_list(device_t *dev);

int32_t driver_attach(device_t *drv, device_t *dev);

#endif /* ! _DEVICE_H */
