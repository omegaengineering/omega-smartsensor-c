#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "smartsensor_private.h"
#include "port_linux.h"

struct _hal {
    pthread_t       thread;
    uint8_t         do_exit;
    linux_i2c_t     i2c;
};


void* port_thread(void* args)
{
    if (args)
    {
        hal_t* sensor = (hal_t*) args;
        while (!sensor->do_exit)
        {
            sensor_poll();
        }
    }
    return NULL;
}


int port_platform_init(void** phal, const port_cfg_t* config)
{
    int ret;
    *phal = NULL;

    hal_t* sensor = malloc(sizeof(hal_t));
    if (sensor) {
        if ((ret = linux_i2c_open(&sensor->i2c, config->bus_res)) != E_OK)
            goto ERROR;

        if ((ret = pthread_create(&sensor->thread, NULL, port_thread, &sensor)) != 0)
            goto ERROR;
        *phal = sensor;
        return ret;
    }
ERROR:
    if (sensor)
        free(sensor);
    return -1;
}

int port_platform_deinit(hal_t* hal)
{
    hal_t * sensor = hal;
    sensor->do_exit = 1;
    pthread_join(hal->thread, NULL);
    linux_i2c_close(&sensor->i2c);

    return E_OK;
}

int port_comm_write(hal_t* hal, const uint8_t* buffer, uint16_t buffer_size)
{
    return linux_i2c_write(&hal->i2c, buffer, buffer_size);
}

int port_comm_read (hal_t* hal, uint8_t* buffer, uint16_t buffer_size)
{
    return linux_i2c_read(&hal->i2c, buffer, buffer_size);
}

int port_timer_register  (hal_t* hal, int period, port_timer_callback callback)
{

}

int port_timer_cancel(hal_t* hal, port_timer_callback callback)
{

}

void port_sleep_ms(uint32_t msec)
{
    usleep(msec);
}
