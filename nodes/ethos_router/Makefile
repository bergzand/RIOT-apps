HOSTNAME ?= hamond
# name of your application
APPLICATION = ethos_router

# If no BOARD is found in the environment, use this default:
BOARD ?= nucleo-f446re

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT

# use ethos (ethernet over serial) for network communication and stdio over
# UART, but not on native, as native has a tap interface towards the host.
GNRC_NETIF_NUMOF := 2
USEMODULE += ethos
ETHOS_BAUDRATE ?= 115200
CFLAGS += -DETHOS_BAUDRATE=$(ETHOS_BAUDRATE) -DUSE_ETHOS_FOR_STDIO

CFLAGS += -I${CURDIR}

# Include packages that pull up and auto-init the link layer.
# NOTE: 6LoWPAN will be included if IEEE802.15.4 devices are present
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
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
# Add also the shell, some shell commands
USEMODULE += gnrc_rpl
USEMODULE += mrf24j40

# Run the simple CoRE resource directory
USEMODULE += rdcli_simple_standalone
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

# include UHCP client
USEMODULE += gnrc_uhcpc

RD_LT ?= 300
# dmn.bergzand.net
RD_ADDR ?= \"2001:470:78ad::20\"
RD_PORT ?= 5683

CFLAGS += -DRDCLI_LT=$(RD_LT)
CFLAGS += -DRDCLI_SERVER_ADDR=$(RD_ADDR)
CFLAGS += -DRDCLI_SERVER_PORT=$(RD_PORT)
CFLAGS += -DHOSTNAME=$(HOSTNAME)

CFLAGS += -DGNRC_NETIF_IPV6_ADDRS_NUMOF=4

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP -DDEBUG_ASSERT_VERBOSE

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1
TAP ?= tapethos
IPV6_PREFIX ?= 2001:db8::/64

ifeq (native,$(BOARD))
TERMDEPS += uhcpd-daemon

.PHONY: uhcpd-daemon

uhcpd-daemon: host-tools
	$(RIOTTOOLS)/uhcpd/bin/uhcpd $(TAP) $(IPV6_PREFIX) &
else
# We override the `make term` command to use ethos
TERMPROG ?= sudo sh $(RIOTTOOLS)/ethos/start_network.sh
TERMFLAGS ?= $(PORT) $(TAP) $(IPV6_PREFIX)

# We depend on the ethos host tools to run the border router, we build them
# if necessary
TERMDEPS += host-tools
endif

.PHONY: host-tools

host-tools:
	$(Q)env -u CC -u CFLAGS make -C $(RIOTTOOLS)

include $(RIOTBASE)/Makefile.include

DEFAULT_PANID ?= 0x483b
DEFAULT_CHANNEL ?= 26
CFLAGS += -DIEEE802154_DEFAULT_CHANNEL=$(DEFAULT_CHANNEL) -DIEEE802154_DEFAULT_PANID=$(DEFAULT_PANID)


