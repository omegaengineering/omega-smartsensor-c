CONTIKI_PROJECT = basic_sensor
all: $(CONTIKI_PROJECT)

TARGET = simplelink
BOARD = launchpad/cc1352r1
PLATFORMS_ONLY = simplelink

# Path configurations
CONTIKI = /home/dev/Projects/contiki-ng
SMARTSENSOR_SDK = ../..

CFLAGS += -g -w -O0
# Contiki include path
CFLAGS += -I$(CONTIKI)/arch/platform/simplelink/cc13xx-cc26xx/
CFLAGS += -I$(CONTIKI)/arch/cpu/simplelink-cc13xx-cc26xx/lib/coresdk_cc13xx_cc26xx/source/

# SDK include path
CFLAGS += -I$(SMARTSENSOR_SDK)
CFLAGS += -I$(SMARTSENSOR_SDK)/core
CFLAGS += -I$(SMARTSENSOR_SDK)/lib/inc

# SDK sources
vpath %.c $(SMARTSENSOR_SDK)/core
PROJECT_SOURCEFILES += smartsensor.c smartsensor_private.c

vpath %.c $(SMARTSENSOR_SDK)/lib/src/hw/ti
PROJECT_SOURCEFILES += bus_ti_i2c.c bus_ti_uart_modbus.c

vpath %.c $(SMARTSENSOR_SDK)/lib/src/hw
PROJECT_SOURCEFILES += bus.c gpio.c

vpath %.c $(SMARTSENSOR_SDK)/lib/src/platform/contiki
PROJECT_SOURCEFILES += contiki_mutex.c contiki_memory.c contiki_timer.c

vpath %.c $(SMARTSENSOR_SDK)/port/contiki
PROJECT_SOURCEFILES += port_contiki.c

# example source
#vpath %.c $(SMARTSENSOR_SDK)/examples/contiki
#PROJECT_SOURCEFILES += basic_sensor.c

# setup cpu specifics, such as driverlib ...
include $(CONTIKI)/Makefile.identify-target
MODULES_REL += $(TARGET)
MODULES_REL += arch/platform/$(TARGET)

include $(CONTIKI)/Makefile.include