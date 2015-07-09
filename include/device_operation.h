/*
 * device_operation.h
 *
 *  Created on: 29 juin 2015
 *      Author: jf
 */

#ifndef INCLUDE_DEVICE_OPERATION_H_
#define INCLUDE_DEVICE_OPERATION_H_

#include "i2c.h"
#include "uart.h"

/* ***** MACROS ***** */
#define DEVICE_CALL( dev, op, ...) _Generic((dev),  \
        device_t * : (dev)->op,                       \
        device_uart_t * : (dev)->op                   \
) (dev, __VA_ARGS__)

#endif /* INCLUDE_DEVICE_OPERATION_H_ */
