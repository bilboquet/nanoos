#include "time.h"

#include "./drv/bus/i2c/i2c.h"
#include "device_operation.h"
#include "drv/serial/uart.h"

int main(void)
{
    test_uart();
    device_t i2c, usart;
    uint8_t buffer[1024];
    uint16_t length;

    i2c_param_t i2c_param;

    time_open();
    /* should not be used this way ?
     i2c_open(&i2c, ID_I2C1, 0x00, 400000);*/

//    device_init(&i2c, DEVICE_I2C, "I2C1");
    device_open(&i2c, O_RDWR);

//    device_init(&usart, DEVICE_UART, "USART2");
    device_open(&usart, O_RDWR);

    while (1) {
        // Receive next command length coded on 2 bytes from host
        //((uart_ops_t *) (usart.ops))->recv(&usart, buffer, 2);
        length = buffer[1] << 8 | buffer[0];

        // Receive I2C command from host
        usart_recv(&usart, buffer, length);

        // Send I2C command to slave
//        ((device_op_i2c_t*) (i2c.ops))->transfer(&i2c, I2C_OP_SEND, 0x76,
//                buffer, length, I2C_STOP_COND_NOSTOP);
        // Read length of next answer length from  I2C slave
//        ((device_op_i2c_t*) (i2c.ops))->transfer(&i2c, I2C_OP_RECV, 0x76,
//                buffer, 2, I2C_STOP_COND_STOP);

        // Forward length to host
        length = buffer[1] << 8 | buffer[0];
        usart_send(&usart, length);

        // Receive I2C answer from slave
//        ((device_op_i2c_t*) (i2c.ops))->transfer(&i2c, I2C_OP_RECV, 0x76,
//                buffer, length, I2C_STOP_COND_STOP);

        // Forward answer from i2c slave to host
        /*TODO: move the loop inside usart_send*/
//        int count = 0;
        uint16_t *data;
//        while (count < length - 1) {
//            data = (uint16_t *) &buffer[count];
//            ((uart_ops_t *) (usart.ops))->send(&usart, *data);
//            count += 2;
//        }
        // count - length == 2, one last run
//        if (count - length == 2) {
//            data = (uint16_t *) &buffer[count];
//            ((uart_ops_t *) (usart.ops))->send(&usart, *data);
//        } else if (count - length == 1) { //one last char to send
//            ((uart_ops_t *) (usart.ops))->send(&usart, buffer[length - 1]);

//    }

    }
}

/**
 * Register the systick timer handler
 */
void SysTick_Handler(void)
{
    time_irq();
}
