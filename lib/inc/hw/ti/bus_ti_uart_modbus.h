/*!********************************************************************************************
  @file     bus_ti_uart_modbus.h

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

#ifndef OMEGA_SMARTSENSOR_C_BUS_UART_H
#define OMEGA_SMARTSENSOR_C_BUS_UART_H

#include "common/common.h"
#include "hw/bus.h"

int bus_ti_uart_modbus_new(bus_t **ctx);
int bus_ti_uart_modbus_free(bus_t *ctx);
int bus_ti_uart_modbus_open(bus_t *ctx, void *cfg);
int bus_ti_uart_modbus_read(bus_t *bus, uint16_t reg_addr, data_buffer_t *buffer);
int bus_ti_uart_modbus_read_raw(bus_t *bus, data_buffer_t *buffer);
int bus_ti_uart_modbus_write(bus_t *bus, uint16_t reg_addr, data_buffer_t *buffer);
int bus_ti_uart_modbus_write_raw(bus_t *bus, data_buffer_t *buffer);
int bus_ti_uart_modbus_close(bus_t *bus);

#endif //OMEGA_SMARTSENSOR_C_BUS_UART_H
