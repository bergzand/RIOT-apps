#ifndef BOARD_H
#define BOARD_H

#include "board_common.h"

#ifdef __cplusplus
extern "C" {
#endif


#define MRF24J40_PARAM_CS          (GPIO_PIN(0, 8))
#define MRF24J40_PARAM_INT         (GPIO_PIN(1, 10))
#define MRF24J40_PARAM_RESET       (GPIO_PIN(1, 4))

//#define JC42_PARAM_I2C_DEV         I2C_DEV(0)
//#define JC42_PARAM_ADDR            (0x18)


/**
 * @name xtimer configuration
 * @{
 */
#define XTIMER_DEV          TIMER_DEV(0)
#define XTIMER_CHAN         (0)
#define XTIMER_OVERHEAD     (6)
#define XTIMER_BACKOFF      (5)
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
