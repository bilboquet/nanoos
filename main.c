#include "time.h"

#include "../drv/bus/i2c/i2c.h"
#include "../drv/serial/usart.h"

int main(void)
{
	device_t i2c, usart;
	uint8_t buffer[1024];
	uint16_t length;

	time_open();
	/* should not be used this way ?
	 i2c_open(&i2c, ID_I2C1, 0x00, 400000);*/

	device_init(&i2c, DEVICE_I2C, "I2C1");
	((device_op_i2c_t*) (i2c.ops))->open(&i2c, ID_I2C1, 0x78, 400000);

	device_init(&usart, DEVICE_USART, "USART2");
	((device_op_usart_t*) (usart.ops))->open(&usart, ID_USART1, 115200);

	while (1) {
		// Receive next command length coded on 2 bytes from host
		((device_op_usart_t*) (usart.ops))->recv(&usart, buffer, 2);
		length = buffer[1] << 8 | buffer[0];

		// Receive I2C command from host
		((device_op_usart_t*) (usart.ops))->recv(&usart, buffer, length);

		// Send I2C command to slave
		((device_op_i2c_t*) (i2c.ops))->transfer(&i2c, I2C_OP_WRITE, 0x76, buffer, length, I2C_STOP_COND_NOSTOP);
		// Read length of next answer length from  I2C slave
		((device_op_i2c_t*) (i2c.ops))->transfer(&i2c, I2C_OP_READ, 0x76, buffer, 2, I2C_STOP_COND_STOP);

		// Forward length to host
		length = buffer[1] << 8 | buffer[0];
		((device_op_usart_t*) (usart.ops))->send(&usart, length);

		// Receive I2C answer from slave
		((device_op_i2c_t*) (i2c.ops))->transfer(&i2c, I2C_OP_READ, 0x76, buffer, length, I2C_STOP_COND_STOP);

		// Forward answer from i2c slave to host
		/*TODO: move the loop inside usart_send*/
		int count = 0;
		uint16_t *data;
		while (count < length - 1) {
			data = (uint16_t *) &buffer[count];
			((device_op_usart_t*) (usart.ops))->send(&usart, *data);
			count += 2;
		}
		// count - length == 2, one last run
		if (count - length == 2) {
			data = (uint16_t *) &buffer[count];
			((device_op_usart_t*) (usart.ops))->send(&usart, *data);
		} else if (count - length == 1) { //one last char to send
			((device_op_usart_t*) (usart.ops))->send(&usart, buffer[length - 1]);

		}

	}
}

/**
 * Register the systick timer handler
 */
void SysTick_Handler(void)
{
	time_irq();
}
