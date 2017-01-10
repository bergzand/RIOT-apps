# Default Makefile, for host native networking

# name of your application
APPLICATION = gcoap

# If no BOARD is found in the environment, use this default:
BOARD ?= maple-mini

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT

BOARD_INSUFFICIENT_MEMORY := chronos msb-430 msb-430h nucleo-f030 nucleo-f334 \
                             stm32f0discovery telosb weio wsn430-v1_3b wsn430-v1_4 \
                             z1

# Must read nordic_softdevice_ble package before nanocoap package. However,
# can't read it explicitly here because it is read later, as a dependency for
# the nrf52dk board.
BOARD_BLACKLIST := nrf52dk

## Uncomment to redefine port, for example use 61616 for RFC 6282 UDP compression.
#GCOAP_PORT = 5683
#CFLAGS += -DGCOAP_PORT=$(GCOAP_PORT)

## Uncomment to redefine request token length, max 8.
#GCOAP_TOKENLEN = 2
#CFLAGS += -DGCOAP_TOKENLEN=$(GCOAP_TOKENLEN)

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_udp
# Add a routing protocol
USEMODULE += gnrc_rpl
# Additional networking modules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_echo

USEMODULE += auto_init_gnrc_rpl

DEFAULT_CHANNEL ?= 26
# Add also the shell, some shell commands
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps


USEMODULE += netstats_l2
USEMODULE += netstats_ipv6
USEMODULE += netstats_rpl

USEMODULE += mrf24j40


USEMODULE += gnrc_sock_udp

USEMODULE += saul_reg
USEMODULE += saul_default
USEMODULE += auto_init_saul
USEMODULE += jc42

USEPKG += nanocoap
USEMODULE += gcoap
# Required by nanocoap, but only due to issue #5959.
USEMODULE += posix

# This application dumps received packets to STDIO using the

# Required by gcoap example
USEMODULE += od
USEMODULE += fmt

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DGNRC_RPL_DODAG_CONF_OPTIONAL_ON_JOIN
CFLAGS += -DGNRC_PKTBUF_SIZE=2000
CFLAGS += -DNANOCOAP_DEBUG
CFLAGS += -DDEVELHELP

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
