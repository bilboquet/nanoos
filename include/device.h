#ifndef _DEVICE_H
#define _DEVICE_H

#include <stdint.h>
/* ***** TYPES ***** */
typedef enum _device_type_t {
	DEVICE_I2C = 0, /** I2C device */
	DEVICE_USART,   /** USART device */
} device_type_t;


typedef struct _device_t {
	// Device name
	char devname[16];

	device_type_t type; /** Device type */
	// Id of the device
	int id;
	void *private;      /** Hidden implementation */
	void *hw;           /** Hardware specific operation */
	void *op;           /** Operation possible on device */
} device_t;


/* ***** PUBLIC METHODS ***** */
/**
 * @brief Initialise a device
 * @param dev     Device to initialise
 * @param type    Device type
 * @param devname Device name
 */
int32_t device_init(device_t *dev, device_type_t type, const char *devname);

#endif /* ! _DEVICE_H */
