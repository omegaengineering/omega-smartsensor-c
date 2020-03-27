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

#include "port/port.h"
#include "smartsensor.h"

typedef struct {
    uint8_t     sensor_attached;
    uint32_t    stat_attach_counter;
    uint32_t    heartbeat_misses;
} data_t;

typedef struct _sensor {
    uint8_t             sensor_id;
    uint8_t             init;
    uint8_t             bus_id;
    sensor_bus_type_t   bus_type;           /**< type of bus that is being used */
//    int                 gpio_pin;           /**< interrupt pin number */
//    bus_t*              bus;                /**< bus object */
//    void*               bus_cfg;            /**< initial config for the bus, hw specific */
//    s19_mutex_t*        bus_lock;           /**< bus mutex */
    event_callback_t    event_callback;     /**< user provided event callback */
    event_callback_t    event_callback_ctx; /**< user provided context for event callback */
    uint8_t             heartbeat_period;   /**< time period in seconds for sensor heartbeat */
//    s19_timer_t*        timer;              /**< timer object platform dependent */
//    s19_timer_entry_t*  heartbeat;          /**< timer entry for heartbeat */
    data_t              data;               /**< internal data */
//    s19_mutex_t*        data_lock;          /**< data mutex */
    uint8_t             ready;              /**< flag to prevent interrupts from using the SDK if sensor has not been opened */
} sensor_t;

typedef enum {
    SENSOR_EVT_HARD_INTR,
    SENSOR_EVT_TIMEOUT,
    SENSOR_EVT_POWER_RESET,
    SENSOR_EVT_POWER_FAULT
} event_type_t;

uint16_t    get_register_data_size(ss_register_t ss_register);


#define s19_log_info    printf
#define s19_log_warn    printf
#define s19_log_dbg     printf

#endif //OMEGA_SMARTSENSOR_C_SMARTSENSOR_PRIVATE_H
