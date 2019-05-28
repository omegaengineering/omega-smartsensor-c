//
// Created by dev on 5/13/19.
//
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
