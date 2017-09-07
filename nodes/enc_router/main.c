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
#include "net/fib.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/ipv6/nc.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/rpl.h"
#include "net/ipv6/addr.h"

#define MAIN_QUEUE_SIZE     (8)

#define WIRED_IPV6_ADDR     "2001:470:78ad::35"
#define WIRED_IPV6_DEFROUTE "2001:470:78ad::"
#define RPL_IPV6_PANID      "2001:470:78ad:8088::1"

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static kernel_pid_t gnrc_border_interface;
static kernel_pid_t gnrc_wireless_interface;

static void set_if_roles(void)
{
    kernel_pid_t ifs[GNRC_NETIF_NUMOF];
    size_t numof = gnrc_netif_get(ifs);

    for (size_t i = 0; i < numof && i < GNRC_NETIF_NUMOF; i++) {
        kernel_pid_t dev = ifs[i];
        int is_wired = gnrc_netapi_get(dev, NETOPT_IS_WIRED, 0, NULL, 0);
        if ((!gnrc_border_interface) && (is_wired == 1)) {
            ipv6_addr_t addr, defroute;
            gnrc_border_interface = dev;

            ipv6_addr_from_str(&addr, WIRED_IPV6_ADDR);
            gnrc_ipv6_netif_add_addr(dev, &addr, 64,
                                     GNRC_IPV6_NETIF_ADDR_FLAGS_UNICAST);

            ipv6_addr_from_str(&addr, WIRED_IPV6_DEFROUTE);
            fib_add_entry(&gnrc_ipv6_fib_table, dev, defroute.u8, 16,
                    0x00, addr.u8, 16, 0,
                    (uint32_t)FIB_LIFETIME_NO_EXPIRE);
        }
        else if ((!gnrc_wireless_interface) && (is_wired != 1)) {
            ipv6_addr_t addr;
            gnrc_wireless_interface = dev;
            ipv6_addr_from_str(&addr, RPL_IPV6_PANID);
            gnrc_ipv6_netif_add_addr(dev, &addr, 64,
                                     GNRC_IPV6_NETIF_ADDR_FLAGS_UNICAST);
            gnrc_rpl_init(dev);
            gnrc_rpl_root_init(gnrc_wireless_interface, &addr, false, false);
        }

        if (gnrc_border_interface && gnrc_wireless_interface) {
            break;
        }
    }
}

int main(void)
{
    /* we need a message queue for the thread running the shell in order to
     * receive potentially fast incoming networking packets */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    set_if_roles();
    puts("enc28j60 based border router application");

    /* start shell */
    puts("All up, running the shell now");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should be never reached */
    return 0;
}
