#ifndef _LOCK_H
#define _LOCK_H

/* ***** TYPES ***** */
/** Lock structure */
typedef struct _lock_t {
	uint8_t lock;
} lock_t;

/* ***** MACROS ***** */
/**
 * @brief Acquire a lock
 * @param lock  The lock to acquire
 */
#define lock_acquire(lock) do { \
    while ((lock) != 0);        \
	lock = 1;                   \
} while (0)
	
/**
 * @brief Release a lock
 * @param lock  The lock to release
 */
#define lock_release(lock) do { \
	if ((lock) == 1)            \
		(lock) = 0;             \
} while (0)

#endif /* ! _LOCK_H */