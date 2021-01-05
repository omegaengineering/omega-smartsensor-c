#include "stm32_freertos.h"
#include "port_stm32.h"
#include "port_freertos.h"

struct _hal {
    stm32_i2c  i2c;
};

static hal_t g_hal;


int port_platform_init(void** phal, const port_cfg_t* config, uint16_t config_sz, sensor_t* sensor)
{
    int ret;
    if (*phal)
        return E_UNAVAILABLE;
    if (config_sz != sizeof(port_cfg_t))
        return E_INVALID_PARAM;

    hal_t* hal = &g_hal;
    if ((ret = i2c_init(&hal->i2c, config->i2cInstance)) != E_OK)
        return ret;
    i2c_set_slave(&hal->i2c, SMARTSENSOR_I2C_ADDR);

    if ((ret = evq_init()) != E_OK)
        return ret;


    *phal = hal;	// assign instance
    return ret;
}

int port_platform_deinit(hal_t* hal, sensor_t* sensor)
{
    int ret;
    ret = i2c_deinit(&hal->i2c);
    return ret;
}

int port_comm_write(hal_t* hal, const uint8_t* buffer, uint16_t buffer_size)
{
    return i2c_write(&hal->i2c, buffer, buffer_size);
}

int port_comm_read (hal_t* hal, uint8_t* buffer, uint16_t buffer_size)
{
    return i2c_read(&hal->i2c, buffer, buffer_size);
}

int port_heartbeat_start(hal_t* hal, int period_ms)
{
    return E_OK;
}

int port_heartbeat_stop(hal_t* hal)
{
    return E_OK;
}

int port_event_get(hal_t* hal, port_event_t* event)
{
    return evq_recv(event);
}

int port_event_put(hal_t* hal, port_event_t event)
{
    return evq_send(&event);
}

void port_sleep_ms(uint32_t ms)
{
    port_task_delay(ms);
}
