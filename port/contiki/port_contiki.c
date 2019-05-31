#include "smartsensor_private.h"
#include "hw/ti/bus_ti_i2c.h"
#include "hw/ti/bus_ti_uart_modbus.h"

bus_ti_i2c_cfg_t my_config = {
        .periph_idx = 0,
        .bit_rate = I2C_100kHz,
        .sda_pin = 4,
        .clk_pin = 5,
        .slave_addr = 0x68
};

const bus_new_dr    port_bus_i2c_new = bus_ti_i2c_new;
const void*         port_bus_i2c_cfg = &my_config;
const bus_new_dr    port_bus_modbus_new = &bus_ti_uart_modbus_new;
const void*         port_bus_modbus_cfg = (const void*)NULL;


port_intr_init(sensor_t *sensor)
{

}
int port_platform_init(sensor_t *sensor)
{

}
int port_platform_exit(sensor_t *sensor)
{

}
