#ifndef SMARTSENSOR_LINUX_H
#define SMARTSENSOR_LINUX_H

#include <stdint.h>
#include <pthread.h>
#include "port.h"

struct _port_cfg {
    sensor_bus_type_t   bus_type;
    const char*         bus_res;
    int                 pin_intr;
    uint8_t             pin_intr_enable;
};


#endif //SMARTSENSOR_LINUX_H
