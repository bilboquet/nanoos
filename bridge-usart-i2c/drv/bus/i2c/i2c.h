#ifndef _DRV_I2C_H
#define _DRV_I2C_H

/* ***** INCLUDES ***** */
#include "types.h"
#include "device.h"
#include "stm32f10x_i2c.h"


/* ***** TYPES ***** */
typedef enum _i2c_op_t {
	I2C_OP_READ = 0,
	I2C_OP_WRITE
} i2c_op_t;

typedef enum _i2c_stop_cond_t {
	I2C_STOP_COND_NOSTOP = 0,
	I2C_STOP_COND_STOP
} i2c_stop_cond_t;

typedef struct _device_hw_t {
	void *hw;

	uint16_t address;
	uint32_t freq;
} i2c_device_t;

typedef struct _device_op_i2c_t {
	int32_t (*open)(device_t *, uint16_t, uint32_t);
	int32_t (*close)(device_t *);
	int32_t (*transfer)(device_t *, uint16_t, uint8_t *, uint16_t, i2c_stop_cond_t);
} device_op_i2c_t;



/* ***** PUBLIC METHODS ***** */
/**
 * @brief Open and initialize an I2C device
 * @param dev      The device representing the I2C peripheral
 * @param id       The id of the device peripheral
 * @param own_addr The own address of the i2c peripheral
 * @param freq     The frequency of the i2c bus
 * @return 0 on success, -X for errors
 */
int32_t i2c_open(device_t *dev, uint32_t id, uint16_t own_addr, uint32_t freq);

/**
 * @brief Close an I2C device
 * @param dev      The device representing the I2C bus
 * @return 0 on success, -X for errors
 */
int32_t i2c_close(device_t * dev);

/**
 * @brief Transfer I2C data on the bus to the slave represented by its addr
 * @param dev       The device representing the i2c peripheral
 * @param addr      Slave address
 * @param op        Operation to perform: I2C_OP_{READ|WRITE}
 * @param buffer    Data buffer
 * @param size      Data buffer length
 * @param stop_cond Does transfer stop after this transfer I2C_STOP_COND_{STOP|NOSTOP}
 * @return 0 on success, -X on errors
 */
int32_t i2c_transfer(device_t *dev, uint16_t addr, i2c_op_t op, uint8_t buffer, uint16_t size, i2c_stop_cond_t stop_cond);

#endif
