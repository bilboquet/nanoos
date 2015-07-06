#ifndef _TIME_H
#define _TIME_H

/* ***** INCLUDES ***** */
#include <stdint.h>

#include "common.h"

typedef device_time_ops_t;
/**
 * Possible operations on a time device
 */
extern const device_time_ops_t time_ops;


/* ***** PUBLIC METHODS ***** */
/**
 * @brief Init the systick timer
 */
void time_open(void);

/**
 * @brief Return the current time in ms from the initialisation of the time peripheral
 * @return time in ms from start
 */
uint32_t time(void);

/**
 * @brief Active wait in ms
 */
inline void mdelay(uint16_t delay_ms);

/**
 * @brief Wait for a condition and execute (err) on timeout
 */
#define wait(cond, timeout, on_err) do {        \
	uint32_t _timeout = time() + timeout;       \
                                                \
	while (!(cond)) {                           \
		if ((int32_t)(_timeout - time()) < 0)   \
			{on_err;}                              \
	}                                           \
} while (0)

/**
 * @brief Operation to be done on systick timer
 */
void time_irq(void);

#endif /* ! _TIME_H */
