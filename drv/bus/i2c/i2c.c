/* ***** INCLUDES *****/
#include "../../../../drv/bus/i2c/i2c.h"

#include "../../../../include/common.h"
#include "../../../../include/tricks.h"

#ifdef STM32F10x
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#elif STM32F429xx
//#include "stm32f4xx_rcc.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#endif /*STM32F429xx*/

#include "time.h"

#include <stdlib.h>

/* ***** OPERATIONS **** */
static device_op_i2c_t ops = {
		.open = i2c_open,
		.close = i2c_close,
		.transfer = i2c_transfer,
};

/* ***** PRIVATE_METHODS ***** */
/**
 * @brief Read method on an i2c bus to the slave address addr
 * @param I2Cx      I2C bus
 * @param addr      Slave address
 * @param buf       Data buffer
 * @param len       Length of data buffer
 * @param stop_cond I2C_COND_STOP_STOP to generate (or not) a stop after the read
 * @return 0 on success, -X for error
 */
int32_t _i2c_read(I2C_TypeDef* I2Cx, uint16_t addr, uint8_t *buf, uint32_t len, i2c_stop_cond_t stop_cond);

/**
 * @brief Write method on an i2c bus to the slave address addr
 * @param I2Cx      I2C bus
 * @param addr      Slave address
 * @param buf       Data buffer
 * @param len       Length of data buffer
 * @param stop_cond I2C_COND_STOP_STOP to generate (or not) a stop after the read
 * @return 0 on success, -X for error
 */
int32_t _i2c_write(I2C_TypeDef* I2Cx, uint16_t addr, const uint8_t *buf, uint32_t len, i2c_stop_cond_t stop_cond);


/* ***** PUBLIC METHODS ***** */
int32_t i2c_open(device_t *dev, uint32_t id, uint16_t own_addr, uint32_t freq)
{
	I2C_InitTypeDef _i2c_init;
	GPIO_InitTypeDef _i2c_gpio;
	i2c_device_t *i2c = NULL;

	if (dev == NULL)
		return -1;

	dev->private = malloc(sizeof(i2c_device_t));
	i2c = (i2c_device_t *)dev->private;
	if (i2c == NULL)
		return -1;

	switch (id) {
	case ID_I2C1:
		dev->id = id;
		i2c->hw = I2C1;

		/* i2c 1 */
		GPIO_StructInit(&_i2c_gpio);

#ifdef STM32F10x
		GPIO_StructInit(&_i2c_gpio);
		_i2c_gpio.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
		_i2c_gpio.GPIO_Speed = GPIO_Speed_50MHz;
		_i2c_gpio.GPIO_Mode = GPIO_Mode_AF_OD;

#elif STM32F429xx
#warning "call HAL_init(); in main()"
		_i2c_gpio.Pin =  GPIO_Pin_6 | GPIO_Pin_7;
		_i2c_gpio.Speed = GPIO_SPEED_FAST;
		_i2c_gpio.Mode = GPIO_MODE_AF_OD;
#else
#error "Only STM32F10x and STM32F4xx are supported"
#endif /*STM32F429xx*/

		GPIO_Init(GPIOB, &_i2c_gpio);
		/* Port B clock enable */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		/*  i2C1 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		break;

	case ID_I2C2:
		dev->id = id;
		i2c->hw = I2C2;

		/* i2c 2 */
		GPIO_StructInit(&_i2c_gpio);
		_i2c_gpio.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
		_i2c_gpio.GPIO_Speed = GPIO_Speed_50MHz;
		_i2c_gpio.GPIO_Mode = GPIO_Mode_AF_OD;
		GPIO_Init(GPIOB, &_i2c_gpio);

		/* Port B clock enable */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
		/*  i2C1 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
		break;

	default:
		return -2;
	}

    _i2c_init.I2C_Mode = I2C_Mode_I2C;
    _i2c_init.I2C_DutyCycle = I2C_DutyCycle_2;
    _i2c_init.I2C_OwnAddress1 = own_addr;
    _i2c_init.I2C_Ack = I2C_Ack_Enable;
    _i2c_init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    _i2c_init.I2C_ClockSpeed = freq;
    I2C_Cmd(i2c->hw, ENABLE);
    I2C_Init(i2c->hw, &_i2c_init);

    return 0;
}

int32_t i2c_close(driver_t *drv)
{
    if (drv == NULL)
        return -1;
    if (drv->state != DRV_OPENED)
        return -2;

    drv->state = DRV_CLOSED
}

/**
 * @brief Transfer I2C
 */
int32_t i2c_transfer(driver_t *drv,
        i2c_op_t op,
        uint8_t buffer,
        uint16_t length,
        i2c_stop_t stop)
{
    i2c_ops_t *ops = NULL;
    i2c_hw_t *hw = NULL;

    if (drv == NULL)
        return -1;
    if (drv->state != DRV_OPENED)
        return -2;

    if (length == 0)
        return 0;

    ops = (i2c_ops_t *) drv->dev->hw->ops;

    while (!lock_get(drv->dev->lock))
        ;
    switch (op) {
    case I2C_OP_WRITE:
        ops->send(buffer, length, stop);
        break;
    case I2C_OP_READ:
        ops->recv(buffer, length, stop);
        break;
    default:
        lock_release(drv->dev->lock);
        return -2;
    }
    lock_release(drv->dev->lock);

    return length;
}



int32_t _i2c_read(I2C_TypeDef* I2Cx, uint16_t addr, uint8_t *buf, uint32_t len, i2c_stop_cond_t stop_cond)
{
	volatile uint32_t Timeout = 5;

	//    I2Cx->CR2 |= I2C_IT_ERR;  interrupts for errors

	if (!len) return 0;

	// Wait for idle I2C interface
	wait(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY), Timeout, goto _i2c_timeout);

	// Enable Acknowledgement, clear POS flag
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

	// Intiate Start Sequence (wait for EV5
	I2C_GenerateSTART(I2Cx, ENABLE);
	wait(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT), Timeout, goto _i2c_timeout);

	// Send Address
	I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Receiver);
	// EV6
	wait(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR), Timeout, goto _i2c_timeout);

	if (len == 1) {
		// Clear Ack bit
		I2C_AcknowledgeConfig(I2Cx, DISABLE);

		// EV6_1 -- must be atomic -- Clear ADDR, generate STOP
		__disable_irq();
		(void) I2Cx->SR2;
		switch (stop_cond) {
		case I2C_STOP_COND_STOP:
			I2C_GenerateSTOP(I2Cx,ENABLE);                  // program stop
			break;

		default:
			break;
		}
		__enable_irq();

		// Receive data   EV7

		wait(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE), Timeout, goto _i2c_timeout);
		*buf++ = I2C_ReceiveData(I2Cx);
	} else if (len == 2) {
		// Set POS flag
		I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Next);

		// EV6_1 -- must be atomic and in this order
		__disable_irq();
		(void)I2Cx->SR2;                           // Clear ADDR flag
		I2C_AcknowledgeConfig(I2Cx, DISABLE);       // Clear Ack bit
		__enable_irq();

		// EV7_3  -- Wait for BTF, program stop, read data twice
		wait(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF), Timeout, goto _i2c_timeout);

		__disable_irq();
		switch (stop_cond) {
		case I2C_STOP_COND_STOP:
			I2C_GenerateSTOP(I2Cx,ENABLE);                  // program stop
			break;
		default: break;
		}
		*buf++ = I2Cx->DR;
		__enable_irq();

		*buf++ = I2Cx->DR;
	} else {
		(void)I2Cx->SR2;                           // Clear ADDR flag
		while (len-- != 3) {
			// EV7 -- cannot guarantee 1 transfer completion time, wait for BTF
			//        instead of RXNE
			wait(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF), Timeout, goto _i2c_timeout);
			*buf++ = I2C_ReceiveData(I2Cx);
		}

		wait(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF), Timeout, goto _i2c_timeout);

		// EV7_2 -- Figure 1 has an error, doesn't read N-2 !
		I2C_AcknowledgeConfig(I2Cx, DISABLE);           // clear ack bit

		__disable_irq();
		*buf++ = I2C_ReceiveData(I2Cx);             // receive byte N-2
		switch (stop_cond) {
		case I2C_STOP_COND_STOP:
			I2C_GenerateSTOP(I2Cx,ENABLE);                  // program stop
			break;
		default: break;
		}
		__enable_irq();
		*buf++ = I2C_ReceiveData(I2Cx);             // receive byte N-1

		// wait for byte N

		wait(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED), Timeout, goto _i2c_timeout);
		*buf++ = I2C_ReceiveData(I2Cx);

		len = 0;
	}

	// Wait for stop
	switch (stop_cond) {
	case I2C_STOP_COND_STOP:
		wait(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF), Timeout, goto _i2c_timeout);
		break;
	default: break;
	}

	return 0;

_i2c_timeout:
	return -1;
}


int32_t _i2c_write(I2C_TypeDef* I2Cx, uint16_t addr, const uint8_t* buf,  uint32_t len, i2c_stop_cond_t stop_cond)
{
    volatile uint32_t Timeout = 5;

    /* Enable Error IT (used in all modes: DMA, Polling and Interrupts */
    //    I2Cx->CR2 |= I2C_IT_ERR;

    if (len) {
		wait(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY), Timeout, goto _i2c_timeout);

		// Intiate Start Sequence
		I2C_GenerateSTART(I2Cx, ENABLE);
		wait(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT), Timeout, goto _i2c_timeout);

		// Send Address  EV5
		I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Transmitter);
		wait(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED), Timeout, goto _i2c_timeout);

		// EV6
		// Write first byte EV8_1
		I2C_SendData(I2Cx, *buf++);
		while (--len) {
			// wait on BTF
			wait(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF), Timeout, goto _i2c_timeout);

			I2C_SendData(I2Cx, *buf++);
		}

		wait(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF), Timeout, goto _i2c_timeout);
		switch (stop_cond) {
		case I2C_STOP_COND_STOP:
			I2C_GenerateSTOP(I2Cx, ENABLE);
			wait(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF), Timeout, goto _i2c_timeout);
			break;

		default:
			break;
        }
    }

    return 0;

_i2c_timeout:
    return -1;
}
