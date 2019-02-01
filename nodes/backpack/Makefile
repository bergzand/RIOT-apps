# name of your application
APPLICATION = backpack

# If no BOARD is found in the environment, use this default:
BOARD ?= dmn_meetstation

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= /home/hydrazine/dev/RIOT-apps/RIOT
RIOTBOARD ?= /home/hydrazine/dev/boards_dmn

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

USEMODULE += printf_float

USEMODULE += bme280
USEMODULE += hdc1000
USEMODULE += gcoap

CFLAGS += -I$(CURDIR)
CFLAGS += -DHDC1000_I2C_ADDRESS=0x40

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP -DDEBUG_ASSERT_VERBOSE

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

DIRS += $(CURDIR)/../../modules/coap_common
INCLUDES += -I$(CURDIR)/../../modules/coap_common
DIRS += $(CURDIR)/../../modules/coap_utils
INCLUDES += -I$(CURDIR)/../../modules/coap_utils

DIRS += $(CURDIR)/../../modules/coap_bmx280
INCLUDES += -I$(CURDIR)/../../modules/coap_bmx280
DIRS += $(CURDIR)/../../modules/coap_hdc1000
INCLUDES += -I$(CURDIR)/../../modules/coap_hdc1000


include $(RIOTBASE)/Makefile.include

DEFAULT_PANID ?= 0x483b
DEFAULT_CHANNEL ?= 26
CFLAGS += -DIEEE802154_DEFAULT_CHANNEL=$(DEFAULT_CHANNEL) -DIEEE802154_DEFAULT_PANID=$(DEFAULT_PANID)

