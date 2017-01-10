/*
 * Copyright (C) 2015 Kaspar Schleiser <kaspar@schleiser.de>
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
 * @brief       CoAP example server application (using microcoap)
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @}
 */

#include <stdio.h>
#include "msg.h"
#include "xtimer.h"

#include "thread.h"
#include "kernel_types.h"
#include "shell.h"

#define MAIN_QUEUE_SIZE (4)
//static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void microcoap_server_loop(void);

char stack[THREAD_STACKSIZE_MAIN];

void *microcoap_handler(void *arg)
{

    microcoap_server_loop();
    return NULL;
}

int main(void)
{
    puts("RIOT microcoap example application");
    kernel_pid_t pid = thread_create(stack, sizeof(stack),
                                     THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST,
                                     microcoap_handler, NULL,
                                     "microcoap");


    /* start shell */
    printf("Microcoap running in thread: %x\n", pid);
    puts("All up, running the shell now");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);



    /* should be never reached */
    return 0;
}
