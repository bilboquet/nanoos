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
#define DEVICE_CALL( dev, op, ...) switch (((device_t*)dev)->type) {\
        case DEVICE_I2C:\
        ((device_i2c_ops_t*)((dev)->ops))->op(dev, __VA_ARGS__);\
        break;\
        case DEVICE_UART:\
        ((uart_ops_t*)((dev)->ops))->op(dev,__VA_ARGS__);\
        break;\
        case DEVICE_TIME:\
        break;\
        case DEVICE_SPI:\
        break;\
        default:\
        return -1;\
    }
//
//#define init(X,...) _Generic((X), \
//        device_i2c_t * : i2c_init, \
//        device_usart_t * : usart_init \
//) (X, __VA_ARGS__)
//
//#define open(X, ...) _Generic((X), \
//        device_i2c_t * : i2c_open, \
//        device_usart_t* : usart_open \
//) (X, __VA_ARGS__)
#endif /* INCLUDE_DEVICE_OPERATION_H_ */
