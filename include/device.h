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

typedef enum _device_ioctl_t {
    IOCTL_I2C_SET_FREQ = 0,
    IOCTL_I2C_SET_OWNADDRESS,

    IOCTL_SPI_SET_FREQ,

    IOCTL_TIME_SET_ALARM,
    IOCTL_TIME_CLEAR_ALARM,

    IOCTL_USART_SET_BAUDRATE,
    IOCTL_USART_CHRLEN,
    IOCTL_USART_SET_PARITY,
    IOCTL_USART_SET_STOPBIT,

} device_ioctl_t;

typedef struct _device_t device_t;

/**
 *  Possible operations on generic device
 */
typedef struct _device_ops_default_t {
    int32_t (*open)(device_t *dev, void *params);                           ///< Open a device
    int32_t (*close)(device_t *dev);                                        ///< Close a device
    int32_t (*suspend)(device_t *dev);                                      ///< Power management: suspend
    int32_t (*resume)(device_t *dev);                                       ///< Power management: suspend
    int32_t (*attach)(device_t *dev, device_t *drv);                        ///< Attach driver drv to device dev
    int32_t (*ioctl)(device_t *dev, device_ioctl_t ioctl_id, void *args);   ///< Interfere with configuration of the device through unified API
} device_ops_default_t;

/**
 * Structure describing a device.
 */
typedef struct _device_t {
    char device_name[16];  ///< Device name
    device_type_t type;    ///< Device type
    device_state_t state;  ///< Device state

    lock_t lock;           ///< Device protection against multiple access
    uint32_t irq;          ///< IRQ id
    bool use_dma;          ///< Indication on dma usage by device
    bool blocking;         ///< If blocking is set, set to polling mode

    list_t *drv;           ///<  Driver using this device

    device_t *dev;         ///< Underlying device to use

    void *private;         ///< Device specific

    /* Operation on devices */
    void *ops;             ///< Device operations
} device_t;

/* ***** PUBLIC METHODS ***** */
/**
 * @brief Initialize a device with the required type, name and operations
 * @param dev      The device to use
 * @param type     The type of the device to use
 * @param devname  The name of the device
 * @param ops      The operation set to use on the device
 * @return error status
 */
int32_t device_init(device_t *dev, device_type_t type, const char *devname, void *ops);

/**
 * @brief Open a device with the given parameters
 * @param dev      The device to open
 * @param params   Parameters to be used to  configure the device before opening
 * @return error status
 */
int32_t device_open(device_t *dev, void *params);

/**
 * @brief Close a device
 * @param dev      The device to close
 * @return error status
 */
int32_t device_close(device_t *dev);

/**
 * @brief Drive a device t olow power mode
 * @param dev      The device to drive
 * @return error status
 */
int32_t device_suspend(device_t dev);

/**
 * @brief Drive a device to full power mode
 * @param dev      The device to drive
 * @return error status
 */
int32_t device_resume(device_t dev);

/**
 * @brief Attach a driver #drv to an underlying device #dev
 * @param dev      The device to be used by drv
 * @param drv      The required driver
 * @return error status
 */
int32_t device_attach(device_t *dev, device_t *drv);

/**
 * @brief Control the parameters of a running device
 * @param dev      The device to reconfigure
 * @param id       The id of the argument to modify
 * @param arg      A pointer to the required parameters to modify
 * @return error status
 */
int32_t device_ioctl(device_t *dev, device_ioctl_t id, void *arg);

#endif /* ! _DEVICE_H */
