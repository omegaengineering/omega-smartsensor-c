CONTIKI_PROJECT = basic_sensor
all: $(CONTIKI_PROJECT)

TARGET = simplelink
BOARD = launchpad/cc1352r1
PLATFORMS_ONLY = simplelink

# Path configurations
CONTIKI = /home/dev/Projects/contiki-ng
SMARTSENSOR_SDK = ../..

include $(CONTIKI)/Makefile.include

CFLAGS += -g -w
# Contiki include path
CFLAGS += -I$(CONTIKI)/arch/platform/simplelink/cc13xx-cc26xx/
CFLAGS += -I$(CONTIKI)/arch/cpu/simplelink-cc13xx-cc26xx/lib/coresdk_cc13xx_cc26xx/source/

# SDK include path
CFLAGS += -I$(SMARTSENSOR_SDK)
CFLAGS += -I$(SMARTSENSOR_SDK)/core
CFLAGS += -I$(SMARTSENSOR_SDK)/lib/inc

vpath %.c $(SMARTSENSOR_SDK)/core
PROJECT_SOURCEFILES += smartsensor.c smartsensor_private.c
PROJECT_SOURCEFILES += $(SMARTSENSOR_SDK)/lib/src/ti/bus_i2c.c

vpath %.c $(SMARTSENSOR_SDK)/examples/contiki
PROJECT_SOURCEFILES += basic_sensor.c



