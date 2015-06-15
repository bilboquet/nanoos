#include "time.h"
#include "i2c.h"

int main(void)
{
	device_t i2c, usart;
	uint8_t buffer[1024];
	uint16_t length;

	time_open();
	i2c_open(&ic, ID_I2C1, 0x00, 400000);
	console_open(usart, ID_USART2, 115200);

    while(1) {
    	// Receive next command length coded on 2 bytes from host
    	usart_recv(buffer, 2);
    	length = buffer[1] << 8 | buffer[0];

    	// Receive I2C command from host
    	usart_recv(&usart, buffer, length);

    	// Send I2C command to slave
    	i2c_transfer(&i2c, I2C_OP_WRITE, 0x76, buffer, length, I2C_STOP_COND_NOSTOP);
    	// Read length of next answer length from  I2C slave
    	i2c_transfer(&i2c, I2C_OP_READ, 0x76, buffer, 2, I2C_STOP_COND_STOP);

    	// Forward length to host
    	length = buffer[1] << 8 | buffer[0];
    	usart_send(&usart, buffer, 2);

    	// Receive I2C answer from slave
    	i2c_transfer(&i2c, I2C_OP_READ, 0x76, buffer, length, I2C_STOP_COND_STOP);

    	// Forward anwer from i2c slave to host
    	usart_send(&usart, buffer, length);
    }
}

/**
 * Register the systick timer handler
 */
void SysTick_Handler(void)
{
	time_irq();
}
