/*
 * device_operation.h
 *
 *  Created on: 29 juin 2015
 *      Author: jf
 */

#ifndef INCLUDE_DEVICE_OPERATION_H_
#define INCLUDE_DEVICE_OPERATION_H_

#include "../drv/bus/i2c/i2c.h"
#include "../drv/serial/usart.h"




#define init(X,...) _Generic((X), \
        device_i2c_t * : i2c_init, \
        device_usart_t * : usart_init \
) (X, __VA_ARGS__)

#define open(X, ...) _Generic((X), \
        device_i2c_t * : i2c_open, \
        device_usart_t* : usart_open \
) (X, __VA_ARGS__)
#endif /* INCLUDE_DEVICE_OPERATION_H_ */
