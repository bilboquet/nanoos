/* ***** INCLUDSE ***** */
#include "device.h"
#include "i2c.h"
#include "uart.h"
#include "time.h"
#include "spi.h"


/* ***** PRIVATE METHOD PROTOTYPES ***** */
static int32_t _device_open(device_t *dev, void *params);                           /** Open a device */
static int32_t _device_close(device_t *dev);                                        /** Close a device */
static int32_t _device_suspend(device_t *dev);                                      /** Power management: suspend */
static int32_t _device_resume(device_t *dev);                                       /** Power management: suspend */
static int32_t _device_attach(device_t *dev, device_t *drv);                        /** Attach driver drv to device dev */
static int32_t _device_ioctl(device_t *dev, device_ioctl_t ioctl_id, void *args);   /** Interfere with configuration of the device through unified API */


/* ***** PUBLIC METHODS ***** */
int32_t device_init(device_t *dev, device_type_t type, const char *devname)
{
    if (dev == NULL) return -1;

    dev->type = type;
    strlcpy(dev->device_name, devname, 16); // TODO: replace strlcpy

    /* Initialize default operations on device */
    dev->open =    _device_open;
    dev->close =   _device_close;
    dev->suspend = _device_suspend;
    dev->resume =  _device_resume;
    dev->attach =  _device_attach;
    dev->ioctl =   _device_ioctl;

    // TODO:xxx_ops may need to be type casted
    switch (type) {
    case DEVICE_I2C:
        dev->ops = &i2c_ops;
        break;

    case DEVICE_USART:
        dev->ops = &usart_ops;
        break;

    case DEVICE_TIME:
        dev->ops = &time_ops;
        break;

    case DEVICE_SPI:
        dev->ops = &spi_ops;
        break;

    default:
        return -1;
    }
    //TODO: why this check?
    if (dev == NULL) return -1;

    return 0;
}

/**
 * @brief Open a device with the given parameters
 * @param dev      The device to open
 * @param params   Parameters to be used to  configure the device before opening
 * @return error status
 */
int32_t device_open(device_t *dev, void *params)
{
    if (dev == NULL) return -1;
    
    if (dev->open != NULL)
        dev->open(dev, params);

    return 0;
}

/**
 * @brief Close a device
 * @param dev      The device to close
 * @return error status
 */
int32_t device_close(device_t *dev)
{
    if (dev == NULL) return -1;

    if (dev->close != NULL)
        dev->close(dev);

    return 0;
}

/**
 * @brief Drive a device to low power mode
 * @param dev      The device to drive
 * @return error status
 */
int32_t device_suspend(device_t *dev)
{
    if (dev == NULL) return -1;

    if (dev->suspend != NULL)
        dev->suspend(dev);

    return 0;
}

/**
 * @brief Drive a device to full power mode
 * @param dev      The device to drive
 * @return error status
 */
int32_t device_resume(device_t *dev)
{
    if (dev == NULL) return -1;

    if (dev->resume != NULL)
        dev->resume(dev);

    return 0;
}

/**
 * @brief Attach a driver #drv to an underlying device #dev
 * @param dev      The device to be used by drv
 * @param drv      The required driver
 * @return error status
 */
int32_t device_attach(device_t *dev, device_t *drv)
{
    if (dev == NULL) return -1;

    if (dev->attach != NULL)
        dev->attach(dev, params);

    return 0;
}

/**
 * @brief Control the parameters of a running device
 * @param dev      The device to reconfigure
 * @param id       The id of the argument to modify
 * @param arg      A pointer to the required parameters to modify
 * @return error status
 */
int32_t ioctl(device_t *dev, device_ioctl_t id, void *arg)
{
    if (dev == NULL) return -1;

    if (dev->ioctl != NULL)
        dev->ioctl(dev, params);

    return 0;
}


/* ***** PRIVATE METHOD ***** */
static int32_t _device_open(device_t *dev, void *params)
{
    return 0;
}

static int32_t _device_close(device_t *dev)
{
    return 0;
}

static int32_t _device_suspend(device_t *dev)
{
    return 0;
}

static int32_t _device_resume(device_t *dev)
{
    return 0;
}

static int32_t _device_attach(device_t *dev, device_t *drv)
{
    return 0;
}

static int32_t _device_ioctl(device_t *dev, device_ioctl_t ioctl_id, void *args)
{
    return 0;
}

/* *****  EOF ***** */
