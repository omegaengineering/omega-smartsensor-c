//
// Created by dev on 5/13/19.
//

#ifndef OMEGA_SMARTSENSOR_C_BUS_H
#define OMEGA_SMARTSENSOR_C_BUS_H

#include "common/common.h"

#define BUS_ID_MAX_LEN          128


/// Opaque declaration of bus object
struct _bus;

/// Typedef declaration of bus object. User should use this declaration when operating with bus
typedef struct _bus bus_t;

typedef int (*bus_new_dr)(bus_t **ctx);
typedef int (*bus_open_dr)(bus_t *ctx, void *cfg);
typedef int (*bus_read_dr)(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer);
typedef int (*bus_write_dr)(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer);
typedef int (*bus_read_raw_dr)(bus_t *ctx, data_buffer_t *buffer);
typedef int (*bus_write_raw_dr)(bus_t *ctx, data_buffer_t *buffer);
typedef int (*bus_close_dr)(bus_t *ctx);
typedef int (*bus_free_dr)(bus_t *ctx);

typedef struct {
    bus_open_dr            bus_open;
    bus_read_dr            bus_read;
    bus_write_dr           bus_write;
    bus_close_dr           bus_close;
    bus_read_raw_dr        bus_read_raw;
    bus_write_raw_dr       bus_write_raw;
    bus_free_dr            bus_free;
} bus_driver_t;

////////////////////////////////////////////

/// Actual definition of _bus object
struct _bus {
    const bus_driver_t* driver;
    char                bus_id[BUS_ID_MAX_LEN];
//    void*               driver_data;
};



int bus_open(bus_t *bus, void *cfg);
int bus_read(bus_t *bus, uint16_t reg_addr, data_buffer_t *buffer);
int bus_write(bus_t *bus, uint16_t reg_addr, data_buffer_t *buffer);
int bus_read_raw(bus_t *bus, data_buffer_t *buffer);
int bus_write_raw(bus_t *bus, data_buffer_t *buffer);
int bus_close(bus_t *bus);
int bus_free(bus_t *bus);



#endif //OMEGA_SMARTSENSOR_C_BUS_H
