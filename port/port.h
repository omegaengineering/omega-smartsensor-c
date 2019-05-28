//
// Created by dev on 5/20/19.
//

#ifndef OMEGA_SMARTSENSOR_C_SUPPORT_H
#define OMEGA_SMARTSENSOR_C_SUPPORT_H

#include "smartsensor_private.h"

extern bus_new_dr   port_bus_i2c_new;
extern void*        port_bus_i2c_cfg;
extern bus_new_dr   port_bus_modbus_new;
extern void*        port_bus_modbus_cfg;
extern int          port_intr_init(sensor_t *sensor);
extern int          port_platform_init(sensor_t *sensor);
extern int          port_platform_exit(sensor_t *sensor);

#endif //OMEGA_SMARTSENSOR_C_SUPPORT_H
