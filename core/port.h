/*!********************************************************************************************
  @file     port.h

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
            Example for using smartsensor device in interrupt mode

***********************************************************************************************/

#ifndef OMEGA_SMARTSENSOR_C_PORT_H
#define OMEGA_SMARTSENSOR_C_PORT_H

#include "smartsensor.h"
#include "smartsensor_private.h"


typedef enum {
    EV_COMM_SEND_DONE,
    EV_COMM_RECV_DONE,
    EV_COMM_TIMEOUT,
    EV_HEARTBEAT,
    EV_DAT_INTR,
} port_event_t;

typedef int     (*port_bus_open_t)(void* p);
typedef int     (*port_bus_close_t)(void* p);
typedef int     (*port_bus_read_t)(void* p, uint16_t reg, uint8_t* buf, uint16_t buf_size);
typedef int     (*port_bus_write_t)(void* p, uint16_t reg, const uint8_t* buf, uint16_t buf_size);
typedef int     (*port_bus_indirect_read_t)(void* p, uint16_t reg, uint8_t* buf, uint16_t buf_size, uint16_t access_reg, const uint8_t *access_buf, uint16_t access_buf_size);
typedef int     (*port_bus_indirect_write_t)(void* p, uint16_t reg, const uint8_t* buf, uint16_t buf_size, uint16_t access_reg, const uint8_t *access_buf, uint16_t access_buf_size);
typedef void    (*port_bus_delay_t)(uint32_t ms);
typedef int     (*port_init_t)(void*p);
typedef int     (*port_deinit_t)(void*p);

typedef int     (*port_event_get_t)(void* p, port_event_t* event);
typedef int     (*port_event_put_t)(void* p, port_event_t event);
typedef int     (*port_timer_start_t)(void* p, uint32_t period_ms);
typedef int     (*port_timer_stop_t)(void* p);
typedef char*   (*strerror_t)(int errnum);
typedef sensor_bus_type_t (*port_bus_type_t)();

typedef struct {
    sensor_t *          sensor;
    port_init_t         init;
    port_deinit_t       deinit;
    port_bus_type_t     bus_type;
    port_bus_read_t     read;
    port_bus_indirect_read_t indirect_read;
    port_bus_indirect_write_t indirect_write;
    port_bus_write_t    write;
    port_bus_delay_t    delay;
    port_timer_start_t  timer_start;
    port_timer_stop_t   timer_stop;
    port_event_get_t    event_get;
    port_event_put_t    event_put;
    strerror_t          strerror;
} port_t;

extern sensor_t*           port_get_sensor(const port_t* p);

#define         port_ENTER_CRITICAL_SECTION()
#define         port_EXIT_CRITICAL_SECTION()

#endif //OMEGA_SMARTSENSOR_C_PORT_H
