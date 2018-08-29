# name of your application
APPLICATION = rdcli_simple

# If no BOARD is found in the environment, use this default:
BOARD ?= samr21-xpro

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT

BOARD_INSUFFICIENT_MEMORY := chronos hifive1 msb-430 msb-430h nucleo-f030r8 nucleo-l053r8 \
                             nucleo-f031k6 nucleo-f042k6 nucleo-l031k6 \
                             stm32f0discovery telosb wsn430-v1_3b wsn430-v1_4 \
                             z1 nucleo-f303k8 nucleo-f334r8
CFLAGS += -I$(CURDIR)

# Enable GNRC networking
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_sixlowpan_router_default
USEMODULE += gnrc_rpl
USEMODULE += auto_init_gnrc_rpl

# Run the simple CoRE resource directory
USEMODULE += rdcli_simple_standalone
USEMODULE += gnrc_icmpv6_echo
USEMODULE += netstats_l2
USEMODULE += netstats_ipv6
USEMODULE += netstats_rpl

# Include the shell for testing purposes
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

USEMODULE += saul_default
USEMODULE += bme280

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

RD_LT ?= 300
# dmn.bergzand.net
RD_ADDR ?= \"2001:470:78ad::20\"
RD_PORT ?= 5683

HOSTNAME ?= \"samr\"

CFLAGS += -DRDCLI_LT=$(RD_LT)
CFLAGS += -DRDCLI_SERVER_ADDR=$(RD_ADDR)
CFLAGS += -DRDCLI_SERVER_PORT=$(RD_PORT)
CFLAGS += -DHOSTNAME=$(HOSTNAME)

include $(RIOTBASE)/Makefile.include

DEFAULT_PANID ?= 0x483b
DEFAULT_CHANNEL ?= 26
CFLAGS += -DIEEE802154_DEFAULT_CHANNEL=$(DEFAULT_CHANNEL) -DIEEE802154_DEFAULT_PANID=$(DEFAULT_PANID)
