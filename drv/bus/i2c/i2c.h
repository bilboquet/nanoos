#ifndef _DRV_I2C_H
#define _DRV_I2C_H
/** \file i2c.h
 * Definition of the device_i2c_t type that allows to manage i2c buses.
 */

/* ***** INCLUDES ***** */
#include <stdint.h>
#include "device.h"
#include "types.h"

#ifdef STM32F10x
#elif STM32F429xx
#include "stm32f4xx_hal_dma.h" //seems to be missing in stm32f4xx_hal_i2c.h
#include "stm32f4xx_hal_i2c.h"
#endif /*STM32F429xx*/

/* ***** TYPES ***** */
typedef enum _i2c_op_t {
    I2C_OP_RECV = 0,
    I2C_OP_SEND
} i2c_op_t;

typedef enum _i2c_stop_cond_t {
    I2C_STOP_COND_NOSTOP = 0,
    I2C_STOP_COND_STOP
} i2c_stop_cond_t;

/**
 * Parameters for i2c devices.
 */
typedef struct _i2c_param_t {
    uint16_t address;
    uint32_t freq;
} i2c_param_t;


typedef device_i2c_ops_t;
/**
 * Possible operations on an i2c device
 */
extern const device_i2c_ops_t i2c_ops;

typedef enum _i2c_id_t {
    ID_I2C1,
    ID_I2C2
} i2c_id_t;

/* ***** PUBLIC METHODS ***** */
int32_t i2c_init(device_t *dev, device_t *drv, uint16_t addr);

/**
 * @brief Open and initialize an I2C device
 * @param dev      The device representing the I2C peripheral
 * @param id       The id of the device peripheral
 * @param own_addr The own address of the i2c peripheral
 * @param freq     The frequency of the i2c bus
 * @return 0 on success, -X for errors
 */

#endif
