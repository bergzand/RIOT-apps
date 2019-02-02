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
USEMODULE += ili9341
USEMODULE += ccs811_full
USEMODULE += bh1750fvi
USEMODULE += fmt

USEPKG += lvgl
USEMODULE += xtimer
USEMODULE += lv_core
USEMODULE += lv_objx
USEMODULE += lv_misc
USEMODULE += lv_themes
USEMODULE += lv_draw
USEMODULE += lv_fonts
USEMODULE += lv_hal

CFLAGS += -I$(CURDIR)
CFLAGS += -DHDC1000_I2C_ADDRESS=0x40

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP -DDEBUG_ASSERT_VERBOSE

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

CFLAGS += -DILI9341_LE_MODE
CFLAGS += -DCCS811_PARAM_MODE=CCS811_MODE_60S


include $(RIOTBASE)/Makefile.include

DEFAULT_PANID ?= 0x483b
DEFAULT_CHANNEL ?= 26
CFLAGS += -DIEEE802154_DEFAULT_CHANNEL=$(DEFAULT_CHANNEL) -DIEEE802154_DEFAULT_PANID=$(DEFAULT_PANID)

