/*!********************************************************************************************
  @file     bus_uart_modbus.c

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

#include <linux/i2c-dev.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <modbus/modbus-rtu.h>
#include <modbus/modbus.h>
#include "hw/linux/bus_uart_modbus.h"
#include "platform/memory.h"
#include "common/errors.h"

static int bus_uart_not_implemented(bus_t*, data_buffer_t *);

static const bus_driver_t bus_uart_modbus_driver = {
    .bus_open = bus_uart_modbus_open,
    .bus_read = bus_uart_modbus_read,
    .bus_write = bus_uart_modbus_write,
    .bus_read_raw = bus_uart_not_implemented,
    .bus_write_raw = bus_uart_not_implemented,
    .bus_close = bus_uart_modbus_close,
    .bus_free = bus_uart_modbus_free
};

int bus_uart_modbus_new(bus_t **ctx)
{
    bus_uart_modbus_t * new_bus = s19_mem_malloc(sizeof(bus_uart_modbus_t));
    if (new_bus == NULL)
        return E_NO_MEM;
    memset(new_bus, 0, sizeof(bus_uart_modbus_t));
    new_bus->base.driver = &bus_uart_modbus_driver;
    *ctx = (bus_t*) new_bus;
    return E_OK;
}

int bus_uart_modbus_free(bus_t *ctx)
{
    if (ctx)
        s19_mem_free(ctx);
    return E_OK;
}


int bus_uart_modbus_open(bus_t *ctx, void *cfg)
{
    int ret;
    bus_uart_modbus_t *p_bus            = (bus_uart_modbus_t*) ctx;
    bus_uart_modbus_cfg_t *modbus_cfg   = (bus_uart_modbus_cfg_t*) cfg;

    p_bus->mb = modbus_new_rtu(ctx->bus_id,
                               modbus_cfg->baud_rate,
                               modbus_cfg->parity,
                               modbus_cfg->data_bits,
                               modbus_cfg->stop_bits);
    if (p_bus->mb == NULL)
        return E_BUS_UNAVAILABLE;
    ret = modbus_set_slave(p_bus->mb, modbus_cfg->dev_addr);
    if (ret < 0)
        return E_BUS_OPERATION;
    ret = modbus_connect(p_bus->mb);
    if (ret < 0)
        return E_BUS_UNAVAILABLE;

    return E_OK;
}


int bus_uart_modbus_read(bus_t *bus, uint16_t reg_addr, data_buffer_t *buffer)
{
    int ret;
    bus_uart_modbus_t *p_bus = (bus_uart_modbus_t*) bus;
    uint16_t rx_size = buffer->data_len % 2 ? buffer->data_len + 1: buffer->data_len;

    if (rx_size > sizeof(p_bus->buffer))
        return E_API_ERROR;

    ret = modbus_read_registers(p_bus->mb, reg_addr, rx_size/2, (uint16_t *) p_bus->buffer);
    if (ret != rx_size / 2)
    {
        return E_BUS_OPERATION;
    }

    for(int idx = 0; idx <= rx_size/2; idx += 2)
    {
        uint8_t t = p_bus->buffer[idx];
        p_bus->buffer[idx] = p_bus->buffer[idx+1];
        p_bus->buffer[idx+1] = t;
    }
    memcpy(buffer->data, p_bus->buffer, buffer->data_len);
    return E_OK;
}


int bus_uart_modbus_write(bus_t *bus, uint16_t reg_addr, data_buffer_t *buffer)
{
    int ret;
    bus_uart_modbus_t *p_bus = (bus_uart_modbus_t*) bus;
    uint16_t tx_size = buffer->data_len % 2 ? buffer->data_len + 1: buffer->data_len;

    if (tx_size > sizeof(p_bus->buffer))
        return E_API_ERROR;

    memcpy(p_bus->buffer, buffer->data, buffer->data_len);
    for (int idx = 0; idx <= tx_size/2; idx +=2 )
    {
        uint8_t t = p_bus->buffer[idx];
        p_bus->buffer[idx] = p_bus->buffer[idx + 1];
        p_bus->buffer[idx + 1] = t;
    }

    ret = modbus_write_registers(p_bus->mb, reg_addr, tx_size /2, (uint16_t *) p_bus->buffer);
    if (ret != tx_size / 2)
    {
        return E_BUS_OPERATION;
    }
    return E_OK;
}

int bus_uart_modbus_close(bus_t *bus)
{
    bus_uart_modbus_t *p_bus = (bus_uart_modbus_t*) bus;
    if (p_bus->mb)
    {
        modbus_close(p_bus->mb);
        modbus_free(p_bus->mb);
    }
    return E_OK;
}

int bus_uart_not_implemented(bus_t *bus, data_buffer_t* buffer)
{
    return E_NOT_SUPPORTED;
}
