/*!********************************************************************************************
  @file     bus_uart_modbus.h

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

#ifndef OMEGA_SMARTSENSOR_C_BUS_UART_MODBUS_H
#define OMEGA_SMARTSENSOR_C_BUS_UART_MODBUS_H

#include <modbus/modbus.h>
#include "hw/bus.h"

#define MODBUS_BUFFER_SIZE     128      /**< buffer for manipulating user data to modbus endianess */

typedef struct {
    bus_t           base;
    modbus_t*       mb;
    uint16_t        dev_addr;
    uint8_t         buffer[MODBUS_BUFFER_SIZE];
} bus_uart_modbus_t;

typedef struct {
    uint16_t        dev_addr;
    int             baud_rate;
    int             parity;
    int             data_bits;
    int             stop_bits;
} bus_uart_modbus_cfg_t;

int bus_uart_modbus_new(bus_t **ctx);
int bus_uart_modbus_open(bus_t *ctx, void *cfg);
int bus_uart_modbus_read(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer);
int bus_uart_modbus_write(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer);
int bus_uart_modbus_close(bus_t *ctx);
int bus_uart_modbus_free(bus_t *ctx);

#endif //OMEGA_SMARTSENSOR_C_BUS_UART_MODBUS_H
