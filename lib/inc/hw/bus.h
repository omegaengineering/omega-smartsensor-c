/*!********************************************************************************************
  @file     bus.h

  @copyright
            Copyright (c) 2019, Omega Engineering Inc.

            Permission is hereby granted, free of charge, to any person obtaining
            a copy of this software and associated documentation files (the
            'Software'), to deal in the Software without restriction, including
            without limitation the rights to use, copy, modify, merge, publish,
            distribute, sublicense, and/or sell copies of the Software, and to
            permit persons to whom the Software is furnished to do so, subject to
            the following conditions:

            The above copyright notice and this permission notice shall be
            included in all copies or substantial portions of the Software.

            THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
            EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
            MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
            IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
            CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
            TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
            SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  @author   Binh Dinh
  @date     June 5th, 2019
  @details


***********************************************************************************************/

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
