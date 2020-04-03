#ifndef SMARTSENSOR_PORT_LINUX_H
#define SMARTSENSOR_PORT_LINUX_H

#include "port.h"

struct _port_cfg {
    sensor_bus_type_t   bus_type;
    const char*         bus_res;
};


typedef struct _bus_i2c {
    int         fd;
} linux_i2c_t;

int  linux_i2c_open(linux_i2c_t* i2c, const char* resource);
int  linux_i2c_close(linux_i2c_t* i2c);
int  linux_i2c_read(linux_i2c_t* i2c, uint8_t* buffer, uint16_t buffer_size);
int  linux_i2c_write(linux_i2c_t* i2c, const uint8_t* buffer, uint16_t buffer_size);

int  port_interrupt_open(int instance);
int  port_interrupt_close(int instance);

int  port_timer_init();
void port_timer_close(int instance);


#endif //SMARTSENSOR_PORT_LINUX_H
