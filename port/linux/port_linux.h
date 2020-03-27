#ifndef SMARTSENSOR_PORT_LINUX_H
#define SMARTSENSOR_PORT_LINUX_H

#include "port.h"

#define SMARTSENSOR_0_I2C       17       // found at /dev/i2c-X

int  port_bus_open(int bus_id);
int  port_bus_close(int bus_id);

int  port_interrupt_open(int instance);
int  port_interrupt_close(int instance);

int  port_timer_init();
void port_timer_close(int instance);


#endif //SMARTSENSOR_PORT_LINUX_H
