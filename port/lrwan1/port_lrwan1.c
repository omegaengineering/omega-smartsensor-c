#include "smartsensor.h"
#include "hw/stm32/bus_stm32_i2c.h"


static const bus_stm32_i2c_cfg_t i2c_cfg = {
        .slave_addr = SMARTSENSOR_I2C_ADDR,
        .timeout = 100  // milliseconds
};

const bus_new_dr    port_bus_i2c_new = bus_stm32_i2c_new;
const void*         port_bus_i2c_cfg = &i2c_cfg;
const bus_new_dr   port_bus_modbus_new = 0;
const void*         port_bus_modbus_cfg = 0;


int port_intr_init(sensor_t *sensor)
{
    return E_OK;
}

int port_platform_init(sensor_t *sensor)
{
    return E_OK;
}