#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <poll.h>
#include "smartsensor_private.h"
#include "log.h"
#include "../port/linux/port_linux.h"
#include "linux.h"

#define MAX_POLL    2
#define MAX_EVENTS  10

struct _hal {
    pthread_t       platform;
    pthread_t       sensor;
    uint8_t         platform_exit;
    linux_i2c_t     i2c;
    pthread_mutex_t bus_lock;
    linux_timer_t   heartbeat;
    ev_queue_t      events;
    pthread_mutex_t evq_lock;
    pthread_cond_t  evq_cond;
    gpio_t          data_gpio;
    uint8_t         data_gpio_enable;
};

typedef enum {
    POLL_HEARTBEAT = 1,
    POLL_DAT_INTR = 2,
} poll_type_t;


typedef struct poll_data {
    struct {
        int         fd;
        poll_type_t type;
    } descriptors[MAX_POLL];
    struct pollfd polls[MAX_POLL];
    int n_poll;
} poll_data_t;

static poll_data_t g_poll_data;

static void update_poll_data(poll_data_t * poll, hal_t* hal)
{
    poll->n_poll = 0;
    // timer poll
    if (linux_timer_is_running(&hal->heartbeat)) {
        poll->descriptors[poll->n_poll].type = POLL_HEARTBEAT;
        poll->descriptors[poll->n_poll].fd = linux_timer_get_fd(&hal->heartbeat);
        poll->polls[poll->n_poll].fd = poll->descriptors[poll->n_poll].fd;
        poll->polls[poll->n_poll].events = POLLIN;
        poll->n_poll++;
    }
    // data interrupt
    if (hal->data_gpio_enable) {
        poll->descriptors[poll->n_poll].type = POLL_DAT_INTR;
        poll->descriptors[poll->n_poll].fd = gpio_get_fd(&hal->data_gpio);
        poll->polls[poll->n_poll].fd = poll->descriptors[poll->n_poll].fd;
        poll->polls[poll->n_poll].events = POLLPRI;
        poll->n_poll++;
    }

}

static void process_poll_data(poll_data_t * poll, hal_t* hal)
{
    int ret;

    for (int i = 0; i < poll->n_poll; i++)
    {
        switch (poll->descriptors[i].type)
        {
            case POLL_HEARTBEAT:
                if (poll->polls[i].revents & POLLIN)
                {
                    // dummy read
                    uint64_t temp;
                    temp = read(poll->polls[i].fd, &temp, sizeof(temp));
                    if (temp != sizeof(uint64_t))
                        continue;
                    if ((ret = port_event_put(hal, EV_HEARTBEAT) != E_OK))
                        s_log("WARN: event queue full\n");
                }
                break;
            case POLL_DAT_INTR:
                if (poll->polls[i].revents & POLLPRI)
                {
                    char temp[3];
                    lseek(poll->polls[i].fd, 0, SEEK_SET);  // consume the interrupt
                    ret = read(poll->polls[i].fd, temp, 3); // dummy read
                    if ((ret = port_event_put(hal, EV_DAT_INTR) != E_OK))
                        s_log("WARN: event queue full\n");
                }
                break;
            default:
                break;
        }
    }
}

void* platform_thread(void* args)
{
    if (!args)
        return NULL;

    hal_t* sensor = (hal_t*) args;
    int read_fds;

    while (!sensor->platform_exit)
    {
        update_poll_data(&g_poll_data, sensor);
        read_fds = poll(g_poll_data.polls, g_poll_data.n_poll, 500);
        if (read_fds > 0)
            process_poll_data(&g_poll_data, sensor);
    }
    return NULL;
}


int port_platform_init(void** phal, const port_cfg_t* config, uint16_t config_sz, sensor_t* sensor)
{
    int ret;
    if (*phal)
        return E_UNAVAILABLE;
    if (config_sz != sizeof(port_cfg_t))
        return E_INVALID_PARAM;
    if (config->bus_type != sensor->bus_type)
        return E_INVALID_PARAM;
    if (config->bus_type != SENSOR_BUS_I2C && config->bus_type != SENSOR_BUS_MODBUS)
        return E_INVALID_PARAM;

    hal_t* hal = malloc(sizeof(hal_t));
    if (hal) {
        memset(hal, 0, sizeof(hal_t));
        if ((ret = pthread_mutex_init(&hal->evq_lock, NULL)) != 0)
            goto ERROR;

        if ((ret = pthread_cond_init(&hal->evq_cond, NULL)) != 0)
            goto ERROR;

        if ((ret = evq_init(&hal->events, MAX_EVENTS)) != E_OK)
            goto ERROR;

        if ((ret = pthread_mutex_init(&hal->bus_lock, NULL)) != 0)
            goto ERROR;

        if (config->bus_type == SENSOR_BUS_I2C) {
            if ((ret = linux_i2c_open(&hal->i2c, config->bus_res, config->bus_type)) != E_OK)
                goto ERROR;
        }
        else if (config->bus_type == SENSOR_BUS_MODBUS) {

        }

        if (config->pin_intr_enable) {
            if ((ret = gpio_init(&hal->data_gpio, config->pin_intr)) != E_OK)
                goto ERROR;

            if ((ret = gpio_export(&hal->data_gpio)) != E_OK)
                goto ERROR;

            if ((ret = gpio_set_direction(&hal->data_gpio, 1)) != E_OK)
                goto ERROR;

            if ((ret = gpio_set_interrupt_edge(&hal->data_gpio, 0)) != E_OK)
                goto ERROR;

            if ((ret = gpio_open(&hal->data_gpio)) != E_OK)
                goto ERROR;

            hal->data_gpio_enable = 1;
        }

        if ((ret = pthread_create(&hal->platform, NULL, platform_thread, hal)) != 0)
            goto ERROR;

        *phal = hal; //TODO race condition btw this and the line below, need solution
                    // sensor thread will access null hal

        if ((ret = pthread_create(&hal->sensor, NULL, sensor_thread, sensor)) != 0)
            goto ERROR;

        return ret;
    }
ERROR:
    perror(__FUNCTION__);
    if (hal) {
        evq_close(&hal->events);
        if (config->pin_intr >= 0)
            gpio_close(&hal->data_gpio);
        free(hal);
    }
    return -1;
}

int port_platform_deinit(hal_t* hal, sensor_t* sensor)
{
    // shutdown sensor thread
    port_event_put(hal, EV_SHUTDOWN);   // ?
    pthread_join(hal->sensor, NULL);
    // shutdown hardwares
    hal->platform_exit = 1;
    pthread_join(hal->platform, NULL);
    linux_i2c_close(&hal->i2c);
    gpio_close(&hal->data_gpio);
    evq_close(&hal->events);
    free(hal);
    return E_OK;
}

int port_comm_write(hal_t* hal, const uint8_t* buffer, uint16_t buffer_size)
{
    int ret;
    pthread_mutex_lock(&hal->bus_lock);
    ret = linux_i2c_write(&hal->i2c, buffer, buffer_size);
    pthread_mutex_unlock(&hal->bus_lock);
    return ret;
}

int port_comm_read (hal_t* hal, uint8_t* buffer, uint16_t buffer_size)
{
    int ret;
    pthread_mutex_lock(&hal->bus_lock);
    ret = linux_i2c_read(&hal->i2c, buffer, buffer_size);
    pthread_mutex_unlock(&hal->bus_lock);
    return ret;
}

int port_heartbeat_start (hal_t* hal, int period_ms)
{
    return linux_timer_start(&hal->heartbeat, period_ms);
}

int port_heartbeat_stop(hal_t* hal)
{
    return linux_timer_stop(&hal->heartbeat);
}

void port_sleep_ms(uint32_t msec)
{
    usleep(msec);
}

int port_event_get(hal_t* hal, port_event_t* event)
{
    int ret;
    pthread_mutex_lock(&hal->evq_lock);
    while ((ret = evq_get(&hal->events, event)) == E_EMPTY)
    {
        pthread_cond_wait(&hal->evq_cond, &hal->evq_lock);
    }
    pthread_mutex_unlock(&hal->evq_lock);
    return ret;
}

int port_event_put(hal_t* hal, port_event_t event)
{
    int ret;
    pthread_mutex_lock(&hal->evq_lock);
    ret = evq_put(&hal->events, &event);
    if (ret == E_OK) {
        pthread_cond_signal(&hal->evq_cond);
    }
    pthread_mutex_unlock(&hal->evq_lock);
    return ret;
}
