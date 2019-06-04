#include <zconf.h>
#include <pthread.h>
#include <string.h>
#include "smartsensor_private.h"
#include "hw/linux/bus_i2c_smbus.h"
#include "hw/linux/bus_uart_modbus.h"
#include "hw/linux/gpio_sysfs.h"
#include "platform/memory.h"
#include "common/log.h"

s19_log_create("PORT", LOG_LEVEL_DEBUG);


static const bus_uart_modbus_cfg_t modbus_cfg = {
    .dev_addr = SMARTSENSOR_MODBUS_ADDR,
    .baud_rate = SMARTSENSOR_MODBUS_BAUDRATE,
    .parity = 'E',
    .data_bits = 8,
    .stop_bits = 1,
};

static const bus_smbus_cfg_t i2c_cfg = {
    .dev_addr = SMARTSENSOR_I2C_ADDR
};

const bus_new_dr    port_bus_i2c_new = bus_smbus_new;
const void*         port_bus_i2c_cfg = &i2c_cfg;
const bus_new_dr    port_bus_modbus_new = bus_uart_modbus_new;
const void*         port_bus_modbus_cfg = &modbus_cfg;
static gpio_sysfs_t ss_intr = {.fd = -1, .pin = -1};
static pthread_t    sensor_thread;
static int          errno;

void* port_task(void *ptr)
{
    sensor_t * sensor = (sensor_t*) ptr;
    int intr_fd = ss_intr.fd;
    fd_set exceptfds;

    if (ss_intr.fd < 0)
    {
        errno = E_API_ERROR;
        return &errno;
    }

    ///TODO install sig handler
    while (!sensor->shutdown_req)
    {
        int ret;
        struct timeval tv, *ptv;
        int timeout = 3;
        tv.tv_sec = 3;
        tv.tv_usec = 0;
        static char value_str[3];

        ptv = timeout > 0 ? &tv : NULL;

        FD_ZERO(&exceptfds);
        FD_SET(intr_fd, &exceptfds);

        ret = select(intr_fd + 1,
                     NULL,               // readfds
                     NULL,               // writefds
                     &exceptfds,         // exceptionfds
                     ptv);               // timeout (or never)

        if (ret > 0 && FD_ISSET(intr_fd, &exceptfds))
        {
            lseek(intr_fd, 0, SEEK_SET);        /* consume interrupt */
            ret = read(intr_fd, value_str, 3);  /* read it then discard */
            hard_intr_triggered(sensor);
        }
    }
    s19_log_dbg("Port task exited\n");
}

int port_intr_init(sensor_t *sensor)
{
    int ret;
    ss_intr.pin = sensor->gpio_pin;
    if ((ret = gpio_sysfs_export((gpio_t *) &ss_intr)) != E_OK)
        return ret;
    if ((ret = gpio_sysfs_open((gpio_t *) &ss_intr, NULL)) != E_OK)
        return ret;
    if ((ret = gpio_sysfs_set_direction((gpio_t *) &ss_intr, GPIO_DIRECTION_INPUT)) != E_OK)
        return ret;
    if ((ret = gpio_sysfs_set_interrupt_edge((gpio_t *) &ss_intr, GPIO_EDGE_FALLING)) != E_OK)
        return ret;
    return E_OK;
}


int port_platform_init(sensor_t *sensor)
{
    int ret;

    pthread_attr_t attr;
    if ((ret = pthread_attr_init(&attr)) < 0)
        return ret;
    if ((ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE)) < 0)
        return ret;
    if ((ret = pthread_create(&sensor_thread, &attr, port_task, (void *) sensor)) < 0)
        return ret;

    return E_OK;
}

int port_platform_exit(sensor_t * sensor)
{
    if (sensor_thread)
    {
//        pthread_cancel(sensor_thread);
        pthread_join(sensor_thread, NULL);
    }
}



