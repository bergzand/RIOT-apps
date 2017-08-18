/*
 * Copyright (C) 2008, 2009, 2010  Kaspar Schleiser <kaspar@schleiser.de>
 * Copyright (C) 2013 INRIA
 * Copyright (C) 2013 Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Default application that shows a lot of functionality of RIOT
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Oliver Hahm <oliver.hahm@inria.fr>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>

#include "thread.h"
#include "shell.h"
#include "shell_commands.h"

#include "hd44780.h"

#if FEATURE_PERIPH_RTC
#include "periph/rtc.h"
#endif

#ifdef MODULE_LTC4150
#include "ltc4150.h"
#endif

#ifdef MODULE_NETIF
#include "net/gnrc/pktdump.h"
#include "net/gnrc.h"
#endif

const hd44780_params_t params =
{
    .cols   = 20,                   \
    .rows   = 4,                    \
    .rs     = (GPIO_PIN(2, 13)),     \
    .rw     = HD44780_RW_OFF,       \
    .enable = (GPIO_PIN(2,14)),        \
    .data   = {(GPIO_PIN(1, 7)), (GPIO_PIN(2, 15)), (GPIO_PIN(1, 15)), (GPIO_PIN(1, 6)), \
               HD44780_RW_OFF, HD44780_RW_OFF, HD44780_RW_OFF, HD44780_RW_OFF} \
};

int main(void)
{
    
    hd44780_t dev;


#ifdef MODULE_LTC4150
    ltc4150_start();
#endif

#ifdef FEATURE_PERIPH_RTC
    rtc_init();
#endif

#ifdef MODULE_NETIF
    gnrc_netreg_entry_t dump = GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL,
                                                          gnrc_pktdump_pid);
    gnrc_netreg_register(GNRC_NETTYPE_UNDEF, &dump);
#endif
    //extern hd44780_t* hd44780_devs;

	hd44780_init(&dev, &params);
    hd44780_clear(&dev);
    hd44780_home(&dev);
    hd44780_print(&dev, "Testing 1, 2, 4..."); 	
    hd44780_set_cursor(&dev, 0,3);
    hd44780_print(&dev, "[Bug][minor][Ready for CI]"); 	
    hd44780_set_cursor(&dev, 0,1);
    hd44780_print(&dev, "This is line 2"); 
    hd44780_set_cursor(&dev, 0,2);
    hd44780_print(&dev, "This is line 3");

    (void) puts("Welcome to RIOT! hd44780 test");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
