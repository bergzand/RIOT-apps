/*
 * Copyright (c) 2015-2016 Koen Zandberg. All rights reserved.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>
#include "msg.h"

#include "net/gcoap.h"
#include "kernel_types.h"
#include "shell.h"
#include "cbor.h"
#include "net/netstats.h"
#include "net/netstats/neighbor.h"

#define ALIVE_STR   "alive"
#define INTERVAL    30000000U
#define SENSOR_INTERVAL  1000000U


static char beaconing_stack[THREAD_STACKSIZE_DEFAULT];
static char sensor_stack[THREAD_STACKSIZE_DEFAULT];
#define MAIN_QUEUE_SIZE (4)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static msg_t _sensor_msg_queue[MAIN_QUEUE_SIZE];
static void _null_handler(unsigned req_state, coap_pkt_t* pdu, sock_udp_ep_t *remote);

static void _notify_thread(netstats_nb_t *stats, void *arg);
static netstats_nb_hook_t hook;

/* 13:16:72:26:12:2d:14:02 */
static const uint8_t mac_addr[] = { 0x13, 0x16, 0x72, 0x26,\
                                    0x12, 0x2d, 0x14, 0x02 };

extern int gcoap_cli_cmd(int argc, char **argv);
extern void gcoap_cli_init(void);

static const shell_command_t shell_commands[] = {
    { "coap", "CoAP example", gcoap_cli_cmd },
    { NULL, NULL, NULL }
};

static void _null_handler(unsigned req_state, coap_pkt_t* pdu, sock_udp_ep_t *remote)
{
    (void)req_state;
    (void)pdu;
    (void)remote;
}

static size_t _send_stats(uint8_t *buf, size_t len, sock_udp_ep_t* remote, netstats_nb_t *stats)
{
    size_t bytes_sent;
    float tx_power = -36 * (stats->tx_attenuation / 255.0);
    coap_pkt_t pkt;
    cbor_stream_t s;
    
    int res = gcoap_req_init(&pkt, buf, len, COAP_METHOD_POST, "/server");
    if(res < 0) {
        return -1;
    }
    coap_hdr_set_type(pkt.hdr, COAP_TYPE_CON);
    /* write payload */
    cbor_init(&s, pkt.payload, pkt.payload_len); 
    cbor_serialize_map(&s, 2);
    cbor_serialize_unicode_string(&s, "etx");
    cbor_serialize_float(&s, stats->etx/128.0);
    cbor_serialize_unicode_string(&s, "tx");
    cbor_serialize_float(&s, tx_power);
    ssize_t pkt_len = gcoap_finish(&pkt, s.pos, COAP_FORMAT_CBOR); 

    bytes_sent = gcoap_req_send2(buf, pkt_len, remote, _null_handler);
    return bytes_sent;
}

static size_t _send_alive(uint8_t *buf, size_t len, sock_udp_ep_t* remote)
{
    size_t bytes_sent;
    coap_pkt_t pkt;
    
    int res = gcoap_req_init(&pkt, buf, len, COAP_METHOD_POST, "/alive");
    if(res < 0) {
        return -1;
    }
    printf("res: %d\n", res);
    coap_hdr_set_type(pkt.hdr, COAP_TYPE_CON);
    res += sizeof(ALIVE_STR);
    memcpy(pkt.payload, ALIVE_STR, sizeof(ALIVE_STR));
    ssize_t pkt_len = gcoap_finish(&pkt, res, COAP_FORMAT_TEXT); 

    bytes_sent = gcoap_req_send2(buf, pkt_len, remote, _null_handler);
    return bytes_sent;
}

static void _register_callback(int *pid) {
    kernel_pid_t ifs[GNRC_NETIF_NUMOF];
    size_t numof = gnrc_netif_get(ifs);

    for (size_t i = 0; i < numof && i < GNRC_NETIF_NUMOF; i++) {
        kernel_pid_t dev = ifs[i];
        int is_wired = gnrc_netapi_get(dev, NETOPT_IS_WIRED, 0, NULL, 0);
        if (is_wired != 1) {
            netstats_nb_t *stats;
            gnrc_netapi_get(dev, NETOPT_STATS_NEIGHBOR, 0, &stats, sizeof(&stats));
            netstats_nb_create(stats, mac_addr, sizeof(mac_addr));
            hook.threshold = 0;
            hook.arg = pid;
            hook.callback = _notify_thread;
            netstats_nb_add_callback(stats, &hook);        
        }

    }
}

static void _notify_thread(netstats_nb_t *stats, void *arg) {
    int *pid = arg;
    msg_t msg;
    msg.content.ptr = stats;
    msg_try_send(&msg, *pid);
}

static void *_alive_thread(void *args) {
    (void)args;
    uint8_t buf[128];

    sock_udp_ep_t remote;
    remote.family = AF_INET6;
    remote.netif  = SOCK_ADDR_ANY_NETIF;
    remote.port = BROKER_PORT;
    
    ipv6_addr_t addr;
    ipv6_addr_from_str(&addr, BROKER_HOST);
    memcpy(&remote.addr.ipv6[0], &addr.u8[0], sizeof(addr.u8));
    xtimer_usleep(4000000);

    for(;;) {
        _send_alive(buf, sizeof(buf), &remote);
        xtimer_usleep(INTERVAL);
    }
    return NULL;
}

static void *_sensor_thread(void *args) {
    (void)args;
    msg_init_queue(_sensor_msg_queue, MAIN_QUEUE_SIZE);
    msg_t m;
    uint8_t buf[128];
    
    sock_udp_ep_t remote;
    remote.family = AF_INET6;
    remote.netif  = SOCK_ADDR_ANY_NETIF;
    remote.port = BROKER_PORT;
    
    ipv6_addr_t addr;
    ipv6_addr_from_str(&addr, BROKER_HOST);
    memcpy(&remote.addr.ipv6[0], &addr.u8[0], sizeof(addr.u8));

    for(;;) {
        msg_receive(&m);
        netstats_nb_t *stats = m.content.ptr;
        printf("Received IPC, new ETX: %3.2f\n", stats->etx/128.0);
        xtimer_usleep(SENSOR_INTERVAL);
        _send_stats(buf, sizeof(buf), &remote, stats);
    }
    return NULL;
}


int main(void)
{
    /* for the thread running the shell */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    gcoap_cli_init();
    puts("gcoap example app");

    /* start shell */
    puts("All up, running the shell now");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    int beacon_pid = thread_create(beaconing_stack, sizeof(beaconing_stack),
                                   THREAD_PRIORITY_MAIN - 1,
                                   THREAD_CREATE_STACKTEST, _alive_thread,
                                   NULL, "Beacon");
    if (beacon_pid == -EINVAL || beacon_pid == -EOVERFLOW) {
        puts("Error: failed to create beaconing thread, exiting\n");
    }
    else {
        puts("Successfuly created beaconing thread !\n");
    }
    int sensor_pid = thread_create(sensor_stack, sizeof(sensor_stack),
                                   THREAD_PRIORITY_MAIN - 1,
                                   THREAD_CREATE_STACKTEST, _sensor_thread,
                                   NULL, "Sensor");
    if (sensor_pid == -EINVAL || sensor_pid == -EOVERFLOW) {
        puts("Error: failed to create sensor thread, exiting\n");
    }
    else {
        puts("Successfuly created sensor thread !\n");
    }
    _register_callback(&sensor_pid);

    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    /* should never be reached */
    return 0;
}
