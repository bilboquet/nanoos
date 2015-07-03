/*
 * device.c
 *
 *  Created on: 3 juil. 2015
 *      Author: jf
 */

#include "device.h"
#include "i2c.h"
#include "usart.h"

int32_t device_init(device_t *dev, device_type_t type, const char *devname)
{
    void * ops;
    dev->type = type;
    strlcpy(dev->device_name, devname, 16); // TODO: replace strlcpy…
    dev->ops = ops;

    // TODO:xxx_ops may need to be type casted
    switch (type) {
    case DEVICE_I2C:
        ops = i2c_ops;
        break;
    case DEVICE_USART:
        ops = usart_ops;
        break;
    case DEVICE_TIME:
        ops = times_ops;
        break;
    case DEVICE_SPI:
        ops = spi_ops;
        break;
    }
    dev->ops = ops;
    return 0;
}
