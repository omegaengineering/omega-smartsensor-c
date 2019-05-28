//
// Created by dev on 5/13/19.
//

#ifndef OMEGA_SMARTSENSOR_C_BUS_I2C_SMBUS_H
#define OMEGA_SMARTSENSOR_C_BUS_I2C_SMBUS_H

#include "hw/bus.h"

typedef struct {
    bus_t       base;
    int         fd;
} bus_smbus_t;

typedef struct {
    uint16_t    dev_addr;
} bus_smbus_cfg_t;


int bus_smbus_new(bus_t **ctx);
int bus_smbus_open(bus_t *ctx, void *cfg);
int bus_smbus_read(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer);
int bus_smbus_write(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer);
int bus_smbus_read_raw(bus_t *ctx, data_buffer_t *buffer);
int bus_smbus_write_raw(bus_t *ctx, data_buffer_t *buffer);
int bus_smbus_close(bus_t *ctx);
int bus_smbus_free(bus_t *ctx);

#endif //OMEGA_SMARTSENSOR_C_BUS_I2C_SMBUS_H
