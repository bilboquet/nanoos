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
    DEVICE_I2C = 0, ///< I2C device
    DEVICE_UART,    ///< UART device or UART feature on USART physical port
    DEVICE_TIME,    ///< TIME device
    DEVICE_SPI,     ///< SPI device
} device_type_t;

/**
 * Device states.
 */
typedef enum _device_state_t {
    UNINITIALIZED = 0,///< UNINITIALIZED
    INITIALIZED,      ///< INITIALIZED
    READY,            ///< READY
    BUSY,             ///< BUSY
} device_state_t;

/**
 * List of device IOCTLs
 */
typedef enum _device_ioctl_t {
    IOCTL_I2C_SET_FREQ = 0,
    IOCTL_I2C_SET_OWNADDRESS,
    IOCTL_I2C_SET_MODE, /* Select master/slave mode */

    IOCTL_SPI_SET_FREQ,

    IOCTL_TIME_SET_ALARM,
    IOCTL_TIME_CLEAR_ALARM,

    IOCTL_UART_SET_BAUDRATE,
    IOCTL_UART_CHRLEN,
    IOCTL_UART_SET_PARITY,
    IOCTL_UART_SET_STOPBIT,

} device_ioctl_t;

/**
 * Device options
 */
typedef enum _device_option_t {
    O_NONE = 0,     ///< No option
    O_RDONLY,       ///< Read only device
    O_WRONLY,       ///< Write only device
    O_RDWR,         ///< Read/write device
    O_NONBLOCKING,  ///< Non blocking operations
} device_option_t

typedef struct _device_t device_t;
/**
 * Structure describing a device.
 */
typedef struct _device_t {
    char device_name[16];    ///< Device name
    device_type_t type;      ///< Device type
    device_state_t state;    ///< Device state

    lock_t lock;             ///< Device protection against multiple access
    uint32_t irq;            ///< IRQ id
    device_option_t options; ///< Device options

    list_t *drv;             ///<  Driver using this device

    device_t *dev;           ///< Underlying device to use

    void *private;           ///< Device specific

    /* Standard operations on devices */
    int32_t (*open)(device_t *dev, device_option_t options);                ///< Open a device
    int32_t (*close)(device_t *dev);                                        ///< Close a device
    int32_t (*suspend)(device_t *dev);                                      ///< Power management: suspend
    int32_t (*resume)(device_t *dev);                                       ///< Power management: suspend
    int32_t (*attach)(device_t *dev, device_t *drv);                        ///< Attach driver drv to device dev
    int32_t (*ioctl)(device_t *dev, device_ioctl_t ioctl_id, void *args);   ///< Interfere with configuration of the device through unified API

    void *ops;     ///< Specific device operations
} device_t;


/* ***** PUBLIC METHODS ***** */
/**
 * @brief Initialize a device with the required type and name.
 * Operations are set according to type.
 * @param dev      The device to use
 * @param type     The type of the device to use
 * @param devname  The name of the device
 * @return error status
 */
int32_t device_init(device_t *dev, device_type_t type, const char *devname);

/**
 * @brief Open a device with the given parameters
 * @param dev      The device to open
 * @param options  Device options
 * @return error status
 */
int32_t device_open(device_t *dev, device_option_t options);

/**
 * @brief Close a device
 * @param dev      The device to close
 * @return error status
 */
int32_t device_close(device_t *dev);

/**
 * @brief Drive a device to low power mode
 * @param dev      The device to drive
 * @return error status
 */
int32_t device_suspend(device_t *dev);

/**
 * @brief Drive a device to full power mode
 * @param dev      The device to drive
 * @return error status
 */
int32_t device_resume(device_t *dev);

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
int32_t ioctl(device_t *dev, device_ioctl_t id, void *arg);

//int32_t device_call(device_t *dev, device_op_t id, void *agrs);

#endif /* ! _DEVICE_H */
