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
    DEVICE_I2C = 0, /**< I2C device */
    DEVICE_USART,   /**< USART device */
} device_type_t;

/**
 * Power management states.
 */
typedef enum _pm_state_t {
    PM_STATE_OFF = 0,   /**< Device OFF */
    PM_STATE_LOW_POWER, /**< Device switch to low power mode */
    PM_STATE_FULL       /**< Device working at full power mode */
} device_pm_state_t;

/**
 * Device states
 */
typedef enum _device_state_t {
    UNINITIALIZED = 0, //!< UNINITIALIZED
    INITIALIZED,      //!< INITIALIZED
    READY,            //!< READY
    DRV_OPENED,       //!< DRV_OPENED
    BUSY,             //!< BUSY
    DRV_CLOSED        //!< DRV_CLOSED
} device_state_t;

/**
 *  Possible operations on generic device
 *  */
typedef struct _device_t device_t;
typedef struct _device_op_t {
    int32_t (*open)(device_t *dev, void *params); /**< Open the device and apply parameters */
    int32_t (*close)(device_t *dev);              /**< Close the device, free resources */
    int32_t (*pm)(device_t *dev, device_pm_state_t newstate); /**< Change power state of the device */
} device_op_t;

/**
 * Structure describing a device.
 */
typedef struct _device_t {

    char devname[16];     /**< Device name */

    device_type_t type;   /**< Device type */
    device_state_t state; /**< Current state of the device */

    lock_t lock;          /**< Device protection against multiple access */
    uint32_t irq;         /**< IRQ id */
    bool use_dma;         /**< Indication on dma usage by device */
    bool async;           /**< If async is set, asynchronous mode use */
    device_t *drv;        /**< Driver using this device */
    device_t *dev;        /**< Underlying device to use */

    void *private;        /**< Hardware specific */

    /* List of devices */
    list_t *list;         /**< List of devices composing the system */

    void *ops;            /**< Possible operations on device */
} device_t;

/* ***** PUBLIC METHODS ***** */
/**
 * @brief Initialise a device
 * @param dev     Device to initialise
 * @param type    Device type
 * @param devname Device name
 */
//int32_t device_init(device_t *dev, device_type_t type, const char *devname);

//int32_t device_open(device_t *dev, void *hw_params);

int32_t device_close(device_t *dev);

int32_t device_pm(device_t *dev, device_pm_state_t state);

int32_t device_list(device_t *dev);

int32_t driver_attach(device_t *drv, device_t *dev);

#endif /* ! _DEVICE_H */
