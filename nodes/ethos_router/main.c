/*
 * Copyright (C) 2015 Freie Universität Berlin
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
 * @brief       Example application for demonstrating the RIOT network stack
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "shell.h"
#include "msg.h"
#include "net/gcoap.h"
#include "net/rdcli_common.h"

#include "xtimer.h"

#define BUFSIZE             (64U)

static char riot_info[BUFSIZE];
#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];


int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("RIOT Bergzand border router");
    /* fill riot info */
    sprintf(riot_info, "{\"ep\":\"%s\",\"lt\":%u}",
            rdcli_common_get_ep(), RDCLI_LT);

    /* print RD client information */
    puts("RD client information:");
    printf(" RD addr: %s\n", RDCLI_SERVER_ADDR);
    printf(" RD port: %u\n", (unsigned)RDCLI_SERVER_PORT);
    printf("      ep: %s\n", rdcli_common_get_ep());
    printf("      lt: %is\n", (int)RDCLI_LT);

    /* start shell */
    puts("All up, running the shell now");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);


    while(1) {
        xtimer_sleep(100);
    }
    /* should be never reached */
    return 0;
}
