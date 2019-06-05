/*!********************************************************************************************
  @file     bus_ti_uart_modbus.c

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

#include "common/errors.h"
#include "hw/ti/bus_ti_uart_modbus.h"

static const bus_driver_t bus_uart_modbus_driver = {
        .bus_open = bus_ti_uart_modbus_open,
        .bus_read = bus_ti_uart_modbus_read,
        .bus_write = bus_ti_uart_modbus_write,
        .bus_read_raw = bus_ti_uart_modbus_read_raw,
        .bus_write_raw = bus_ti_uart_modbus_write_raw,
        .bus_close = bus_ti_uart_modbus_close,
        .bus_free = bus_ti_uart_modbus_free
};

int bus_ti_uart_modbus_new(bus_t **ctx)
{
    return E_NOT_SUPPORTED;
}

int bus_ti_uart_modbus_free(bus_t *ctx)
{
    return E_NOT_SUPPORTED;
}


int bus_ti_uart_modbus_open(bus_t *ctx, void *cfg)
{
    return E_NOT_SUPPORTED;
}


int bus_ti_uart_modbus_read(bus_t *bus, uint16_t reg_addr, data_buffer_t *buffer)
{
    return E_NOT_SUPPORTED;
}

int bus_ti_uart_modbus_read_raw(bus_t *bus, data_buffer_t *buffer)
{
    return E_NOT_SUPPORTED;
}

int bus_ti_uart_modbus_write(bus_t *bus, uint16_t reg_addr, data_buffer_t *buffer)
{
    return E_NOT_SUPPORTED;
}

int bus_ti_uart_modbus_write_raw(bus_t *bus, data_buffer_t *buffer)
{
    return E_NOT_SUPPORTED;
}

int bus_ti_uart_modbus_close(bus_t *bus)
{
    return E_NOT_SUPPORTED;
}
