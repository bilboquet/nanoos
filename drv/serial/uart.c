/* ***** INCLUDES ***** */
#include  <stddef.h>
#include <stdlib.h>
#include "uart.h"

#ifdef STM32F10x
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#elif STM32F429xx
//#include "stm32f4xx_hal_rcc.h"
//#include "stm32f4xx_hal_gpio.h"
#endif /*STM32F429xx*/

int32_t _uart_init(device_t *uart);
int32_t _uart_open(device_t *uart, device_option_t options);
int32_t _uart_close(device_t *uart);
int32_t _uart_suspend(device_t *uart);
int32_t _uart_resume(device_t *uart);
int32_t _uart_ioctl(device_t *uart, device_ioctl_t id, void *arg);
int32_t _uart_send(device_t *uart, const char *str, uint16_t len);
int32_t _uart_recv(device_t *uart, const char *str, uint16_t len);

/* ***** OPERATIONS ***** */
static uart_ops_t uart_ops = {
    .init = _uart_init,
    .open = _uart_open,
    .close = _uart_close,
    .suspend = _uart_suspend,
    .resume = _uart_resume,
    .ioctl = _uart_ioctl,
    .send = _uart_send,
    .recv = _uart_recv};

/* ***** PUBLIC METHODS ***** */
int32_t uart_init(device_t *uart, const char *uartname)
{
    if (uart == NULL)
        return -1;

    return device_init(uart, DEVICE_UART, uartname, &uart_ops);
}

int32_t uart_open(device_t *uart, device_option_t options)
{
    return device_open(uart, options);
}

int32_t uart_close(device_t *uart)
{
    if (uart == NULL)
        return -1;

    return device_close(uart);
}

int32_t uart_suspend(device_t *uart)
{
    if (uart == NULL)
        return -1;

    return device_suspend(uart);
}

int32_t uart_resume(device_t *uart)
{
    if (uart == NULL)
        return -1;

    return device_resume(uart);
}

int32_t uart_attach(device_t *dev, device_t *uart)
{
    if (uart == NULL)
        return -1;

    return device_attach(dev, uart);
}

int32_t uart_ioctl(device_t *uart)
{
    if (uart == NULL)
        return -1;

    return device_ioctl(uart);
}

int32_t uart_send(device_t *uart, const char *str, uint16_t len)
{
    if (uart == NULL)
        return -1;

    return ((uart_ops_t *) (uart->ops))->send(uart, str, len);
}

int32_t uart_recv(device_t *uart, const char *str, uint16_t len)
{
    if (uart == NULL)
        return -1;

    return ((uart_ops_t *) (uart->ops))->recv(uart, str, len);
}

void test_uart(void)
{
    device_t uart;
    char buffer_in[16];

    uart_init(&uart, "/dev/uart-0");

    uart_open(&uart, O_RDWR);

    uart_send(&uart, "Echo test\r\n", 11);

    uart_recv(&uart, buffer_in, 1);
    uart_send(&uart, buffer_in, 1);

    uart_close(&uart);
}

/* ***** PRIVATE METHODS ***** */
int32_t _uart_init(device_t *uart)
{
    if (uart == NULL)
        return -1;

    return arch_uart_init(uart);
}

int32_t _uart_open(device_t *uart, device_option_t options)
{
    if (uart == NULL)
        return -1;

    return arch_uart_open(uart, options);

}

int32_t _uart_close(device_t *uart)
{
    if (uart == NULL)
        return -1;

//uart->private = arg; ??? uart->private = uart

    return arch_uart_close(uart);

}

int32_t _uart_suspend(device_t *uart)
{
    if (uart == NULL)
        return -1;

    return arch_uart_open(uart);
}

int32_t _uart_resume(device_t *uart)
{
    if (uart == NULL)
        return -1;

    return arch_uart_open(uart);

}

int32_t _uart_ioctl(device_t *uart, device_ioctl_t id, void *arg)
{
    int32_t err;
    if (uart == NULL)
        return -1;

    switch (id) {
    case IOCTL_UART_SET_PARITY:
        err = arch_uart_set_parity(uart, arg);
        break;
    case IOCTL_UART_SET_STOPBIT:
        err = arch_uart_set_stopbit(uart, arg);
        break;
    case IOCTL_UART_SET_CHRLEN:
        err = arch_uart_set_charlen(uart, arg);
        break;
    case IOCTL_UART_SET_BAUDRATE:
        err = arch_uart_set_baudrate(uart, arg);
        break;
    default:
        err = -2;
    }

    return err;
}

int32_t _uart_send(device_t *uart, const char *str, uint16_t len)
{
    if (uart == NULL)
        return -1;

    return arch_uart_send(uart, str, len);
}

int32_t _uart_recv(device_t *uart, const char *str, uint16_t len)
{
    if (uart == NULL)
        return -1;

    return arch_uart_recv(uart, str, len);
}

//int32_t usart_open(device_t *dev, uint32_t id, uint32_t baudrate)
//{
//    device_usart_t *usart;
//    USART_InitTypeDef _usart_init;
//    GPIO_InitTypeDef _gpio_init;
//
//    if (dev == NULL)
//        return -1;
//
//    dev->private = malloc(sizeof(device_usart_t));
//    usart = (device_usart_t *) dev->private;
//    if (usart == NULL)
//        return -2;
//
//    dev->id = id;
//    switch (id) {
//    case ID_USART1:
//        usart->hw = USART1;
//
//        // Turn on clocks
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
//
//        // Configure TX pin
//        _gpio_init.GPIO_Pin = GPIO_Pin_9;
//        _gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
//        _gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
//        GPIO_Init(GPIOA, &_gpio_init);
//
//        // Configure RX pin
//        _gpio_init.GPIO_Pin = GPIO_Pin_10;
//        _gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//        GPIO_Init(GPIOA, &_gpio_init);
//        break;
//#ifdef STM32F4xx
//        case ID_USART2:
//        usart->hw = USART2;
//
//        // Turn on clocks
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART2, ENABLE);
//
//        // Configure TX pin
//        _gpio_init.GPIO_Pin = GPIO_Pin_2;
//        _gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
//        _gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
//        GPIO_Init(GPIOA, &_gpio_init);
//
//        // Configure RX pin
//        _gpio_init.GPIO_Pin = GPIO_Pin_3;
//        _gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//        GPIO_Init(GPIOA, &_gpio_init);
//        break;
//#endif /* STM32F4xx */
//
//    default:
//        return -3;
//    }
//
//    usart->baudrate = baudrate;
//    // Configure the UART
//    USART_StructInit(&_usart_init);
//    _usart_init.USART_BaudRate = baudrate;
//    _usart_init.USART_WordLength = USART_WordLength_8b;
//    _usart_init.USART_StopBits = USART_StopBits_1;
//    _usart_init.USART_Parity = USART_Parity_No;
//    _usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    _usart_init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//    USART_Init(usart->hw, &_usart_init);
//    USART_Cmd(usart->hw, ENABLE);
//
//    return 0;
//}
//
//int32_t usart_close(device_t *dev)
//{
//    device_usart_t *usart;
//
//    if (dev == NULL)
//        return -1;
//    usart = (device_usart_t *) dev->private;
//
//    if (usart == NULL)
//        return -2;
//
//    USART_DeInit(usart->hw);
//    free(usart);
//
//    return 0;
//}
//
//static int32_t usart_send(device_t *dev, uint16_t data)
//{
//    USART_SendData((USART_TypeDef*) (dev->private), data);
//    while (
//        USART_GetFlagStatus((USART_TypeDef*) (dev->private), USART_FLAG_TXE) == RESET)
//        ;
//}
//
//static int32_t usart_recv(device_t *dev, uint16_t *data)
//{
//    int32_t status = -1;
//    if (USART_GetFlagStatus((USART_TypeDef*) (dev->private), USART_FLAG_RXNE)) {
//        *data = USART_ReceiveData((USART_TypeDef*) (dev->private));
//        status = 0;
//    }
//    return status;
//}
