/* ***** INCLUDES ***** */
#include "../../lib/time.h"

#include "stdint.h"
#include "../../arch/stm32/f1/stm_lib/inc/misc.h"

/* ***** TYPES ***** */
typedef struct _time_t {
	uint32_t epoch;
	uint32_t now;
} time_t;

/* ***** GLOBAL VARIABLES ***** */
static time_t _time = { 0, 0 };

/* ***** PUBLIC METHODS ***** */
void time_open(void)
{
	/* Setup SysTick Timer for 1 msec interrupts  */
	if (SysTick_Config(SystemCoreClock / 1000)) {
		/* Capture error */
		while (1)
			;
	}
}

void time_irq(void)
{
	if (_time.now + 1 == 0) {
		_time.epoch++;
	}
	_time.now++;
}

inline uint32_t time(void)
{
	return _time.now;
}

void mdelay(uint16_t delay_ms)
{
	volatile uint32_t deadline = time() + delay_ms;
	while ((int32_t) (deadline - time()) > 0)
		;
}

