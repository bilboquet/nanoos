#ifndef _DEVICE_H
#define _DEVICE_H

/* ***** INCLUDES ***** */
#include <lock.h>
#include <list.h>

/* ***** TYPES ***** */
typedef enum _device_type_t {
    DEVICE_I2C = 0, /** I2C device */
    DEVICE_USART, /** USART device */
} device_type_t;

typedef struct _pm_state_t {
    PM_STATE_OFF = 0, /** Device OFF */
    PM_STATE_LOW_POWER, /** Device switch to low power mode */
    PM_STATE_FULL, /** Device working at full power mode */
} device_pm_state_t;

typedef enum _device_state_t {
    UNINITIALIZED = 0,
    INITIALIZED,
    READY,
    BUSY,
} device_state_t;

typedef enum _device_pm_state_t {
    PM_OFF = 0,
    PM_LOW,
    PM_FULL,
} device_pm_state_t;


typedef struct _device_t {
    char device_name[16];  /* Device name */
    device_type_t type;    /* Device type */
    device_state_t state;  /* Device state */

    lock_t lock; /* Device protection against multiple access */
    uint32_t irq; /* IRQ id */
    bool use_dma; /* Indication on dma usage by device */
    bool async; /* If async is set, asynchronous mode use */
    device_t *drv; /*  Driver using this device */
    device_t *dev; /* Underlying device to use */

    void *private; /* Hardware specific */

    /* List of devices */
    list_t *list;

    /* Basic operation on devices */
    int32_t (*init)(device_t *dev, device_type_t type, void *params);
    int32_t (*open)(device_t *dev, void *params);
    int32_t (*close(device_t *dev);
    int32_t (*pm)(device_t *dev, device_pm_state_t newstate);
} device_t;


/* ***** PUBLIC METHODS ***** */
/**
 * @brief Initialise a device
 * @param dev     Device to initialise
 * @param id      Device type
 * @param devname Device name
 */
int32_t device_init(device_t *dev, device_type_t type, const char *devname);

int32_t device_open(device_t *dev, void *hw_params);

in32_t device_close(device_t *dev);

int32_t device_pm(device_t *dev, device_pm_state_t state);

in32_t device_list(device_t *dev);

int32_t driver_attach(driver_t *drv, device_t *dev);

int32_t i2c_init(device_t *dev, driver_t *drv, uint16_t addr);

int32_t i2c_open(driver_t *drv, uint16_t addr);

int32_t i2c_close(driver_t *drv)
{
    if (drv == NULL) return -1;
    if (drv->state != DRV_OPENED) return -2;

        drv->state = DRV_CLOSED
}

int32_t i2c_transfer(driver_t *drv, i2c_op_t op, uint8_t buffer, uint16_t length, i2c_stop_t stop)
{
    i2c_ops_t *ops = NULL;
    i2c_hw_t *hw = NULL;

    if (drv == NULL) return -1;
    if (drv->state != DRV_OPENED) return -2;

    if (length == 0) return 0;

    ops = (i2c_ops_t *)drv->dev->hw->ops;

    while (!lock_get(drv->dev->lock));
    switch (op) {
        case I2C_OP_WRITE:
            ops->send(buffer, length, stop);
            break;
        case I2C_OP_READ:
            ops->recv(buffer, length, stop);
            break;
        default:
            return -2;
    }
    lock_release(drv->dev->lock);

    return length;
}

#endif /* ! _DEVICE_H */
