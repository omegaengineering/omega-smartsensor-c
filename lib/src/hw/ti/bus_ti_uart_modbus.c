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
