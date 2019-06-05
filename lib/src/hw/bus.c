/*!********************************************************************************************
  @file     bus.c

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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "common/errors.h"
#include "hw/bus.h"


int bus_open(bus_t *ctx, void *cfg)
{
    return ctx->driver->bus_open(ctx, cfg);
}

int bus_read(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer)
{
    if (buffer->data_len == 0)
        return E_BUFFER_MEM_SIZE;
    return ctx->driver->bus_read(ctx, reg_addr, buffer);
}

int bus_read_raw(bus_t *ctx, data_buffer_t *buffer)
{
    if (buffer->data_len == 0)
        return E_BUFFER_MEM_SIZE;
    return ctx->driver->bus_read_raw(ctx, buffer);
}

int bus_write(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer)
{
    if (buffer->data_len == 0)
        return E_BUFFER_MEM_SIZE;
    return ctx->driver->bus_write(ctx, reg_addr, buffer);
}

int bus_write_raw(bus_t *ctx, data_buffer_t *buffer)
{
    if (buffer->data_len == 0)
        return E_BUFFER_MEM_SIZE;
    return ctx->driver->bus_write_raw(ctx, buffer);
}

int bus_close(bus_t *ctx)
{
    return ctx->driver->bus_close(ctx);
}

int bus_free(bus_t *ctx)
{
    return ctx->driver->bus_free(ctx);
}
