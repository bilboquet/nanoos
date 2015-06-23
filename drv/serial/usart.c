/* ***** INCLUDES ***** */
#include "../../../drv/serial/usart.h"

#include  <stddef.h>
#include <stdlib.h>

#ifdef STM32F10x
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#elif STM32F429xx
//#include "stm32f4xx_hal_rcc.h"
//#include "stm32f4xx_hal_gpio.h"
#endif /*STM32F429xx*/

/* ***** OPERATIONS **** */
static device_op_usart_t ops = { .open = usart_open, .close = usart_close, .send = usart_send, .recv = usart_recv };

/* ***** PUBLIC METHODS ***** */
int32_t usart_open(device_t *dev, uint32_t id, uint32_t baudrate)
{
	device_usart_t *usart;
	USART_InitTypeDef _usart_init;
	GPIO_InitTypeDef _gpio_init;

	if (dev == NULL)
		return -1;

	dev->private = malloc(sizeof(device_usart_t));
	usart = (device_usart_t *) dev->private;
	if (usart == NULL)
		return -2;

	dev->id = id;
	switch (id) {
	case ID_USART1:
		usart->hw = USART1;

		// Turn on clocks
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		// Configure TX pin
		_gpio_init.GPIO_Pin = GPIO_Pin_9;
		_gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
		_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &_gpio_init);

		// Configure RX pin
		_gpio_init.GPIO_Pin = GPIO_Pin_10;
		_gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &_gpio_init);
		break;
#ifdef STM32F4xx
		case ID_USART2:
		usart->hw = USART2;

		// Turn on clocks
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART2, ENABLE);

		// Configure TX pin
		_gpio_init.GPIO_Pin = GPIO_Pin_2;
		_gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
		_gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &_gpio_init);

		// Configure RX pin
		_gpio_init.GPIO_Pin = GPIO_Pin_3;
		_gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &_gpio_init);
		break;
#endif /* STM32F4xx */

	default:
		return -3;
	}

	usart->baudrate = baudrate;
	// Configure the UART
	USART_StructInit(&_usart_init);
	_usart_init.USART_BaudRate = baudrate;
	_usart_init.USART_WordLength = USART_WordLength_8b;
	_usart_init.USART_StopBits = USART_StopBits_1;
	_usart_init.USART_Parity = USART_Parity_No;
	_usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	_usart_init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(usart->hw, &_usart_init);
	USART_Cmd(usart->hw, ENABLE);

	return 0;
}

int32_t usart_close(device_t *dev)
{
	device_usart_t *usart;

	if (dev == NULL)
		return -1;
	usart = (device_usart_t *) dev->private;

	if (usart == NULL)
		return -2;

	USART_DeInit(usart->hw);
	free(usart);

	return 0;
}

int32_t usart_send(device_t *dev, uint16_t data)
{
	USART_SendData((USART_TypeDef*) (dev->hw), data);
	while (USART_GetFlagStatus((USART_TypeDef*) (dev->hw), USART_FLAG_TXE) == RESET)
		;
}

int32_t usart_recv(device_t *dev, uint16_t *data)
{
	int32_t status = -1;
	if (USART_GetFlagStatus((USART_TypeDef*) (dev->hw), USART_FLAG_RXNE)) {
		*data = USART_ReceiveData((USART_TypeDef*) (dev->hw));
		status = 0;
	}
	return status;
}
