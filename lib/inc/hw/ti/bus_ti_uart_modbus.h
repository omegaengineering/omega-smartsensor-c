//
// Created by dev on 5/28/19.
//

#ifndef OMEGA_SMARTSENSOR_C_BUS_UART_H
#define OMEGA_SMARTSENSOR_C_BUS_UART_H

#include "common/common.h"
#include "hw/bus.h"

int bus_ti_uart_modbus_new(bus_t **ctx);
int bus_ti_uart_modbus_free(bus_t *ctx);
int bus_ti_uart_modbus_open(bus_t *ctx, void *cfg);
int bus_ti_uart_modbus_read(bus_t *bus, uint16_t reg_addr, data_buffer_t *buffer);
int bus_ti_uart_modbus_write(bus_t *bus, uint16_t reg_addr, data_buffer_t *buffer);

int bus_ti_uart_modbus_close(bus_t *bus);
int bus_ti_uart_not_implemented(bus_t *bus, data_buffer_t* buffer);
#endif //OMEGA_SMARTSENSOR_C_BUS_UART_H
