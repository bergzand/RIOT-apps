# Default Makefile, for host native GNRC-based networking

# name of your application
APPLICATION = nucleo_node

# If no BOARD is found in the environment, use this default:
BOARD ?= nucleo-f446

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT

BOARD_INSUFFICIENT_MEMORY := chronos msb-430 msb-430h nucleo32-f031 nucleo32-f042 \
                             nucleo32-l031 nucleo-f030 nucleo-f334 nucleo-l053 \
                             stm32f0discovery telosb wsn430-v1_3b wsn430-v1_4 z1

## Uncomment to redefine port, for example use 61616 for RFC 6282 UDP compression.
#CFLAGS += -DNANOCOAP_DEBUG

BROKER_PORT ?= 5683
BROKER_HOST ?= \"fc01::1\"

CFLAGS += -DBROKER_PORT=$(BROKER_PORT) -DBROKER_HOST=$(BROKER_HOST)

#GCOAP_PORT = 5683
#CFLAGS += -DGCOAP_PORT=$(GCOAP_PORT)

## Uncomment to redefine request token length, max 8.
#GCOAP_TOKENLEN = 2
#CFLAGS += -DGCOAP_TOKENLEN=$(GCOAP_TOKENLEN)


BOARD_INSUFFICIENT_MEMORY := airfy-beacon b-l072z-lrwan1 \
                             calliope-mini cc2650stk maple-mini \
                             microbit msb-430 msb-430h nrf51dongle nrf6310 \
                             nucleo32-f031 nucleo32-f042 nucleo32-f303 nucleo32-l031 \
                             nucleo-f030 nucleo-f070 nucleo-f072 nucleo-f103 nucleo-f302 \
                             nucleo-f334 nucleo-l053 nucleo-l073 opencm904 pca10000 \
                             pca10005 spark-core stm32f0discovery telosb wsn430-v1_3b \
                             wsn430-v1_4 yunjia-nrf51822 z1


# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
# Specify the mandatory networking modules for 6LoWPAN border router
USEMODULE += gnrc_ipv6_router_default
# Add forwarding table
USEMODULE += fib
# Additional networking modules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_echo
USEMODULE += gnrc_rpl
USEMODULE += auto_init_gnrc_rpl

USEMODULE += gcoap

# Required by gcoap example
USEMODULE += cbor
USEMODULE += cbor_float
USEMODULE += od
USEMODULE += fmt
# Add also the shell, some shell commands
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

USEMODULE += netstats_l2
USEMODULE += netstats_ipv6
USEMODULE += netstats_neighbor
USEMODULE += netstats_neighbor_ext
USEMODULE += gnrc_netdev_power

USEMODULE += netstats_rpl
USEMODULE += mrf24j40
USEMODULE += printf_float

DEFAULT_CHANNEL ?= 26

CFLAGS += -I$(CURDIR)


# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

DEFAULT_PANID ?= 0x512D
CFLAGS += -DIEEE802154_DEFAULT_PANID=$(DEFAULT_PANID)

include $(RIOTBASE)/Makefile.include

# Set a custom channel if needed
ifneq (,$(filter cc110x,$(USEMODULE)))          # radio is cc110x sub-GHz
  DEFAULT_CHANNEL ?= 0
  CFLAGS += -DCC110X_DEFAULT_CHANNEL=$(DEFAULT_CHANNEL)
else
  ifneq (,$(filter at86rf212b,$(USEMODULE)))    # radio is IEEE 802.15.4 sub-GHz
    DEFAULT_CHANNEL ?= 5
    CFLAGS += -DIEEE802154_DEFAULT_SUBGHZ_CHANNEL=$(DEFAULT_CHANNEL)
  else                                          # radio is IEEE 802.15.4 2.4 GHz
    DEFAULT_CHANNEL ?= 26
    CFLAGS += -DIEEE802154_DEFAULT_CHANNEL=$(DEFAULT_CHANNEL)
  endif
endif
