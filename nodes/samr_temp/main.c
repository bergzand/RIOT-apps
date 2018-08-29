/*
 * Copyright (C) 2017 Freie Universität Berlin
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
 * @brief       Test application demonstrating the simple registration
 *              process to a CoRE RD using gcoap
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "shell.h"
#include "net/gcoap.h"
#include "net/rdcli_common.h"
#include "fmt.h"
#include "saul.h"
#include "saul_reg.h"
#include "log.h"

#define BUFSIZE             (64U)

static char riot_info[BUFSIZE];

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

/* define some dummy CoAP resources */
static ssize_t text_resp(coap_pkt_t *pdu, uint8_t *buf, size_t len,
                         const char *text, unsigned format)
{
    gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
    size_t slen = strlen(text);
    memcpy(pdu->payload, text, slen);
    return gcoap_finish(pdu, slen, format);
}

static ssize_t handler_info(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx)
{
    (void)ctx;
    return text_resp(pdu, buf, len, riot_info, COAP_FORMAT_JSON);
}

static ssize_t handler_text(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx)
{
    return text_resp(pdu, buf, len, (char *)ctx, COAP_FORMAT_TEXT);
}

static ssize_t handler_saul(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx)
{
    char fmt[8];
    uint32_t type = (uint32_t)ctx;
    phydat_t data;
    saul_reg_t* sens = saul_reg_find_type(type);
    saul_reg_read(sens, &data);
    size_t fmtlen = fmt_s16_dfp(fmt, data.val[0], data.scale);
    fmt[fmtlen] = '\0';
    return text_resp(pdu, buf, len, (char*)fmt, COAP_FORMAT_TEXT);
}

static coap_resource_t resources[] = {
    { "/riot/bar",  COAP_GET, handler_text, "foo" },
    { "/riot/foo",  COAP_GET, handler_text, "bar" },
    { "/riot/info", COAP_GET, handler_info, NULL },
    { "/riot/hostname", COAP_GET, handler_text, HOSTNAME },
    { "/s/hum", COAP_GET, handler_saul, (void*)SAUL_SENSE_HUM },
    { "/s/press", COAP_GET, handler_saul, (void*)SAUL_SENSE_PRESS },
    { "/s/temp", COAP_GET, handler_saul, (void*)SAUL_SENSE_TEMP }
};

static gcoap_listener_t listener = {
    .resources     = &resources[0],
    .resources_len = sizeof(resources) / sizeof(resources[0]),
    .next          = NULL
};

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("CoAP simplified RD registration example!\n");

    /* fill riot info */
    sprintf(riot_info, "{\"ep\":\"%s\",\"lt\":%u}",
            rdcli_common_get_ep(), RDCLI_LT);

    /* register resource handlers with gcoap */
    gcoap_register_listener(&listener);

    /* print RD client information */
    puts("RD client information:");
    printf(" RD addr: %s\n", RDCLI_SERVER_ADDR);
    printf(" RD port: %u\n", (unsigned)RDCLI_SERVER_PORT);
    printf("      ep: %s\n", rdcli_common_get_ep());
    printf("      lt: %is\n", (int)RDCLI_LT);

    /* run the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
