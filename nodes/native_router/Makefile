# name of your application
APPLICATION = native_router

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../RIOT

BOARD_INSUFFICIENT_MEMORY := airfy-beacon b-l072z-lrwan1 \
                             calliope-mini cc2650stk maple-mini \
                             microbit msb-430 msb-430h nrf51dongle nrf6310 \
                             nucleo32-f031 nucleo32-f042 nucleo32-f303 nucleo32-l031 \
                             nucleo-f030 nucleo-f070 nucleo-f072 nucleo-f103 nucleo-f302 \
                             nucleo-f334 nucleo-l053 nucleo-l073 opencm904 pca10000 \
                             pca10005 spark-core stm32f0discovery telosb weio wsn430-v1_3b \
                             wsn430-v1_4 yunjia-nrf51822 z1

BOARD_BLACKLIST += mips-malta pic32-wifire pic32-clicker# No full UART available.

# SLIP legacy compatibility
# Uncomment the lines below if you want to use SLIP with this example and don't
# forget to comment the lines above for ethos.
GNRC_NETIF_NUMOF := 2

INCLUDES += -I$(CURDIR)
# Include SLIP package for IP over Serial communication

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_rpl
USEMODULE += gnrc_pktdump
# Specify the mandatory networking modules for 6LoWPAN border router
USEMODULE += gnrc_sixlowpan_border_router_default
# Add forwarding table
USEMODULE += fib
# Additional networking modules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_echo
# Add also the shell, some shell commands
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

USEMODULE += netstats_l2
USEMODULE += netstats_ipv6
USEMODULE += netstats_neighbor
USEMODULE += netstats_neighbor_ext

USEMODULE += netstats_rpl
USEMODULE += mrf24j40
USEMODULE += printf_float

DEFAULT_CHANNEL ?= 26


# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

TAP ?= riottap
IPV6_PREFIX ?= 2001:db8::/64

include $(RIOTBASE)/Makefile.include

DEFAULT_CHANNEL ?= 26
CFLAGS += -DIEEE802154_DEFAULT_CHANNEL=$(DEFAULT_CHANNEL)
