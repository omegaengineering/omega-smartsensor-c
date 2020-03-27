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


extern int      port_platform_init(int instance);
extern int      port_platform_deinit(int instance);

int             port_bus_write(int bus_id, data_buffer_t *buffer);
int             port_bus_read(int bus_id, data_buffer_t *buffer);

typedef void    (*port_timer_callback)(int instance);
extern int      port_timer_register(int instance, int period, port_timer_callback callback);
extern int      port_timer_deregister(int instance, port_timer_callback callback);


void            port_sleep_ms(uint32_t ms);

void            sensor_interrupt_triggered(int instance);
void            sensor_timeout_triggered(int instance);

#define         port_ENTER_CRITICAL_SECTION()
#define         port_EXIT_CRITICAL_SECTION()

#endif //OMEGA_SMARTSENSOR_C_PORT_H
