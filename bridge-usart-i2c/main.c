#include "time.h"
#include "i2c.h"

int main(void)
{
	device_t i2c, usart;
	uint8_t buffer[1024];
	uint16_t length;

	time_open();
	i2c_open(&ic, ID_I2C1, 0x00, 400000);

	device_register(&i2c, DEVICE_I2C, "I2C1");
	i2c.op->open(&i2c, 0x78, 400000);

	device_register(&usart, DEVICE_USART, "USART2");
	usart.op->open(&usart, 115200);

    while(1) {
    	// Receive next command length coded on 2 bytes from host
    	usart.op->recv(&usart, buffer, 2);
    	length = buffer[1] << 8 | buffer[0];

    	// Receive I2C command from host
    	usart.op->recv(&usart, buffer, length);

    	// Send I2C command to slave
    	i2c.op->transfer(&i2c, I2C_OP_WRITE, 0x76, buffer, length, I2C_STOP_COND_NOSTOP);
    	// Read length of next answer length from  I2C slave
    	i2c.op->transfer(&i2c, I2C_OP_READ, 0x76, buffer, 2, I2C_STOP_COND_STOP);

    	// Forward length to host
    	length = buffer[1] << 8 | buffer[0];
    	usart.op->send(&usart, buffer, 2);

    	// Receive I2C answer from slave
    	i2c.op->transfer(&i2c, I2C_OP_READ, 0x76, buffer, length, I2C_STOP_COND_STOP);

    	// Forward anwer from i2c slave to host
    	usart.op->send(&usart, buffer, length);
    }
}

/**
 * Register the systick timer handler
 */
void SysTick_Handler(void)
{
	time_irq();
}
