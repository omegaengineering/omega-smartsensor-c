#ifndef SMARTSENSOR_LINUX_H
#define SMARTSENSOR_LINUX_H

#include "port.h"

typedef struct {
    sensor_bus_type_t   bus_type;
    const char*         bus_res;
    int                 interrupt_pin;
    event_callback_t    event_callback;     // set to handle sensor event automatically, unset to opt out
    void*               event_callback_ctx;
} linuxConfig_t;

void* get_platform(void* config);

#endif //SMARTSENSOR_LINUX_H
