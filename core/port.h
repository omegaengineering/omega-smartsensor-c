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

typedef struct _hal hal_t;

typedef enum {
    EV_COMM_SEND_DONE,
    EV_COMM_RECV_DONE,
    EV_COMM_TIMEOUT,
    EV_HEARTBEAT,
    EV_DAT_INTR,
    EV_SHUTDOWN, // is it really necessary ?
} port_event_t;


int     port_platform_init(void** hal, const port_cfg_t* config, uint16_t config_sz, sensor_t* sensor);
int     port_platform_deinit(hal_t* hal, sensor_t* sensor);

int     port_comm_write(hal_t* hal, const uint8_t* buffer, uint16_t buffer_size);
int     port_comm_read (hal_t* hal, uint8_t* buffer, uint16_t buffer_size);

int     port_heartbeat_start(hal_t* hal, int period_ms);
int     port_heartbeat_stop(hal_t* hal);

int     port_event_get(hal_t* hal, port_event_t* event);
int     port_event_put(hal_t* hal, port_event_t event);

void    port_sleep_ms(uint32_t ms);


#define         port_ENTER_CRITICAL_SECTION()
#define         port_EXIT_CRITICAL_SECTION()

#endif //OMEGA_SMARTSENSOR_C_PORT_H
