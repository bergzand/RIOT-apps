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

#define THREAD_STACKSIZE_MAIN	8096

#include <stdio.h>
#include <string.h>

#include "thread.h"
#include "shell.h"
#include "shell_commands.h"

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

#include "jc42.h"
#include "periph/i2c.h"
#include "thread.h"
#include "xtimer.h"
char stack[THREAD_STACKSIZE_MAIN];
static jc42_t temp1;

void *thread_handler(void *arg)
{
    float temperature = 0;
    while(1){
        jc42_get_temperature(&temp1, &temperature);
        printf("Temperature: %3.2f\n", (float)temperature);
        xtimer_sleep(1);
    }
    return NULL;
}


int main(void)
{


#ifdef FEATURE_PERIPH_RTC
    rtc_init();
#endif
    
    jc42_init(&temp1, I2C_0, I2C_SPEED_NORMAL, 0x18);

    (void) puts("Welcome to RIOT!");
    kernel_pid_t pid = thread_create(stack, sizeof(stack),
                                     THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST,
                                     thread_handler, NULL,
                                     "thread");

    printf("thread running as %c", (char)pid);

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
