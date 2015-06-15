#ifndef _DEVICE_H
#define _DEVICE_H

/* ***** TYPES ***** */
typedef enum _device_id_t {
	DEVICE_I2C = 0,
	DEVICE_USART,
} device_type_t;


typedef struct _device_t {
	char devname[16];   /** Device name */

	device_type_t type; /** Device type */

	void *hw;           /** Hardware specific operation */
	void *op;           /** Operation possible on device */
} device_t;


/* ***** PUBLIC METHODS ***** */
int32_t device_register(device_t *dev, device_id_t id, const char *devname);

#endif /* ! _DEVICE_H */
