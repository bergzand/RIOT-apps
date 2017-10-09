#ifndef BOARD_H
#define BOARD_H

#include "board_common.h"

#ifdef __cplusplus
extern "C" {
#endif


#define MRF24J40_PARAM_SPI         (SPI_DEV(2))
#define MRF24J40_PARAM_SPI_SPEED   (SPI_CLK_5MHZ)
#define MRF24J40_PARAM_CS          (GPIO_PIN(0, 14))
#define MRF24J40_PARAM_INT         (GPIO_PIN(3, 2))
#define MRF24J40_PARAM_RESET       (GPIO_PIN(0, 13))

//#define BMX280_PARAM_I2C_DEV         I2C_DEV(0)
//#define BMX280_PARAM_I2C_ADDR        (0x76)

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
