/*!********************************************************************************************
  @file     smartsensor_private.h

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

#ifndef OMEGA_SMARTSENSOR_C_SMARTSENSOR_PRIVATE_H
#define OMEGA_SMARTSENSOR_C_SMARTSENSOR_PRIVATE_H

#include "smartsensor.h"
#include "port.h"



typedef enum {
    ST_READY,
    ST_DISABLED,
    ST_SEND,
    ST_RECV,
} state_t;





typedef enum {
    SENSOR_EVT_HARD_INTR,
    SENSOR_EVT_TIMEOUT,
    SENSOR_EVT_POWER_RESET,
    SENSOR_EVT_POWER_FAULT
} event_type_t;

uint16_t    get_register_data_size(ss_register_t ss_register);

int sensor_poll(sensor_t* sensor);
int sensor_is_opened(sensor_t* sensor);



#endif //OMEGA_SMARTSENSOR_C_SMARTSENSOR_PRIVATE_H
