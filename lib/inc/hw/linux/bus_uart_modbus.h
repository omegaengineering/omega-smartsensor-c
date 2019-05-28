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
