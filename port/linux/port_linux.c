#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <poll.h>
#include <errno.h>
#include "smartsensor_private.h"
#include "log.h"
#include "linux_private.h"

#define MAX_POLL            2
#define MAX_EVENTS          10

typedef struct {
    port_t          base;
    int             interrupt_pin;
    char            bus[128];
    sensor_bus_type_t bus_type;
    pthread_t       platform_thdl;
    pthread_t       sensor_thdl;
    uint8_t         platform_exit;
    uint8_t         i2c_addr;
#ifdef I2C_SENSOR
    linux_i2c_t     i2c;
#endif
#ifdef MODBUS_SENSOR
    linux_modbus_t  modbus;
#endif
    pthread_mutex_t bus_lock;
    linux_timer_t   heartbeat;
    ev_queue_t      events;
    pthread_mutex_t evq_lock;
    pthread_cond_t  evq_cond;
    gpio_t          interrupt_gpio;
    event_callback_t event_callback;
    void*           event_callback_ctx;
} portLinux_t;

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

int port_event_put(void* p, port_event_t event);
int port_event_get(void* p, port_event_t* event);
static int is_interrupt_used(portLinux_t* port);

static void update_poll_data(poll_data_t * poll, portLinux_t* hal)
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
#if I2C_SENSOR
    if (is_interrupt_used(hal)) {
        poll->descriptors[poll->n_poll].type = POLL_DAT_INTR;
        poll->descriptors[poll->n_poll].fd = gpio_get_fd(&hal->interrupt_gpio);
        poll->polls[poll->n_poll].fd = poll->descriptors[poll->n_poll].fd;
        poll->polls[poll->n_poll].events = POLLPRI;
        poll->n_poll++;
    }
#endif
}

static void process_poll_data(poll_data_t * poll, portLinux_t * p)
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
                    if ((ret = port_event_put(p, EV_HEARTBEAT) != E_OK))
                        s_log("WARN: event queue full\n");
                }
                break;
            case POLL_DAT_INTR:
                if (poll->polls[i].revents & POLLPRI)
                {
                    char temp[3];
                    lseek(poll->polls[i].fd, 0, SEEK_SET);  // consume the interrupt
                    ret = read(poll->polls[i].fd, temp, 3); // dummy read
                    if ((ret = port_event_put(p, EV_DAT_INTR) != E_OK))
                        s_log("WARN: event queue full\n");
                }
                break;
            default:
                break;
        }
    }
}

/** handle gpio event & timer event,
 * and put the event into a queue */
void* platform_thread(void* args)
{
    portLinux_t * p = args;
    int read_fds;
    if (!p)
        return NULL;

    while (!p->platform_exit)
    {
        update_poll_data(&g_poll_data, p);
        read_fds = poll(g_poll_data.polls, g_poll_data.n_poll, -1);
        if (read_fds == 0) {
            // timeout
        }
        else if (read_fds > 0) {
            process_poll_data(&g_poll_data, p);
        }
    }
    return NULL;
}

void* sensor_thread(void* args)
{
    int ret;
    portLinux_t * p = args;
    if (!p)
        return NULL;
    sensor_t* sensor = port_get_sensor(&p->base);
    api_event_t event;

    while (!p->platform_exit)
    {
        ret = sensor_poll_event(sensor, &event);
        if (ret == E_OK && p->event_callback) {
            p->event_callback(event, p->event_callback_ctx);
        }
    }
    return NULL;
}

static int is_interrupt_used(portLinux_t* port)
{
    return port->interrupt_pin >= 0 && port->bus_type == SENSOR_BUS_I2C;
}

int linux_init (void* port)
{
    int ret;
    if (!port)
        return E_INVALID_PARAM;
    portLinux_t * p = port;

    if ((ret = pthread_mutex_init(&p->evq_lock, NULL)) != 0)
        goto ERROR;

    if ((ret = pthread_cond_init(&p->evq_cond, NULL)) != 0)
        goto ERROR;

    if ((ret = evq_init(&p->events, MAX_EVENTS)) != E_OK)
        goto ERROR;

    if ((ret = pthread_mutex_init(&p->bus_lock, NULL)) != 0)
        goto ERROR;

    ret = E_PORT_UNAVAILABLE;
    if (p->bus_type == SENSOR_BUS_I2C) {
#ifdef I2C_SENSOR
        if ((ret = linux_i2c_open(&p->i2c, p->bus, p->i2c_addr)) != E_OK)
            goto ERROR;
#endif
    }
    else if (p->bus_type == SENSOR_BUS_MODBUS) {
#ifdef MODBUS_SENSOR
        if ((ret = linux_modbus_open(&p->modbus, p->bus)) != E_OK)
            goto ERROR;
#endif
    }
    if (ret)
        goto ERROR;

#if I2C_SENSOR
    // modbus uart does not use interrupt pin
    if (is_interrupt_used(p)) {
        if ((ret = gpio_init(&p->interrupt_gpio, p->interrupt_pin)) != E_OK)
            goto ERROR;

        if ((ret = gpio_export(&p->interrupt_gpio)) != E_OK)
            goto ERROR;

        if ((ret = gpio_set_direction(&p->interrupt_gpio, 1)) != E_OK)
            goto ERROR;

        if ((ret = gpio_set_interrupt_edge(&p->interrupt_gpio, 0)) != E_OK)
            goto ERROR;

        if ((ret = gpio_open(&p->interrupt_gpio)) != E_OK)
            goto ERROR;
    }
#endif

    if ((ret = pthread_create(&p->platform_thdl, NULL, platform_thread, p)) != 0)
        goto ERROR;

    if (p->event_callback
        && (ret = pthread_create(&p->sensor_thdl, NULL, sensor_thread, p)) != 0)
        goto ERROR;

    return ret;

ERROR:
    perror(__FUNCTION__);
    evq_close(&p->events);
#if I2C_SENSOR
    if (is_interrupt_used(p))
        gpio_close(&p->interrupt_gpio);
#endif
    return SET_PORT_ERR(errno);
}

int linux_deinit(void* port)
{
    portLinux_t * p = port;
    p->platform_exit = 1;       // signal thread to exit
    if (p->sensor_thdl) {
        pthread_cancel(p->sensor_thdl);
        pthread_join(p->sensor_thdl, NULL);
    }
    if (p->platform_thdl) {
        pthread_cancel(p->platform_thdl);
        pthread_join(p->platform_thdl, NULL);
    }
    if (p->bus_type == SENSOR_BUS_I2C) {
#if I2C_SENSOR
        linux_i2c_close(&p->i2c);
#endif
    }
    else if (p->bus_type == SENSOR_BUS_MODBUS) {
#if MODBUS_SENSOR
        linux_modbus_close(&p->modbus);
#endif
    }
#if I2C_SENSOR
    if (is_interrupt_used(p)) {
        gpio_close(&p->interrupt_gpio);
    }
#endif
    evq_close(&p->events);
    free(p);
    return E_OK;
}

int port_comm_write(void* p, uint16_t reg, const uint8_t* buffer, uint16_t buffer_size)
{
    int ret = E_PORT_UNAVAILABLE;
    portLinux_t * hal = p;
    pthread_mutex_lock(&hal->bus_lock);

    if (hal->bus_type == SENSOR_BUS_I2C) {
#if I2C_SENSOR
        ret = linux_i2c_write(&hal->i2c, reg, buffer, buffer_size);
#endif
    }
    else if (hal->bus_type == SENSOR_BUS_MODBUS) {
#if MODBUS_SENSOR
        ret = linux_modbus_write(&hal->modbus, reg, buffer, buffer_size);
#endif
    }
    pthread_mutex_unlock(&hal->bus_lock);
    return ret;
}

int port_comm_read (void* p, uint16_t reg, uint8_t* buffer, uint16_t buffer_size)
{
    int ret = E_PORT_UNAVAILABLE;
    portLinux_t * hal = p;
    pthread_mutex_lock(&hal->bus_lock);

    if (hal->bus_type == SENSOR_BUS_I2C) {
#if I2C_SENSOR
        ret = linux_i2c_read(&hal->i2c, reg, buffer, buffer_size);
#endif
    }
    else if (hal->bus_type == SENSOR_BUS_MODBUS) {
#if MODBUS_SENSOR
        ret = linux_modbus_read(&hal->modbus, reg, buffer, buffer_size);
#endif
    }
    pthread_mutex_unlock(&hal->bus_lock);
    return ret;
}

int port_heartbeat_start (void* p, uint32_t period_ms)
{
    portLinux_t * hal = p;
    return linux_timer_start(&hal->heartbeat, period_ms);
}

int port_heartbeat_stop(void* p)
{
    portLinux_t * hal = p;
    return linux_timer_stop(&hal->heartbeat);
}

void port_sleep_ms(uint32_t msec)
{
    usleep(msec);
}

int port_event_get(void* p, port_event_t* event)
{
    int ret;
    portLinux_t * hal = p;
    pthread_mutex_lock(&hal->evq_lock);
    while (!hal->platform_exit
            && (ret = evq_get(&hal->events, event)) == E_EMPTY)
    {
        if ((pthread_cond_wait(&hal->evq_cond, &hal->evq_lock) != 0)) {
            // was interrupted
            ret = E_CONTINUE;
            break;
        }
    }
    pthread_mutex_unlock(&hal->evq_lock);
    return ret;
}

int port_event_put(void* p, port_event_t event)
{
    int ret;
    portLinux_t * hal = p;
    pthread_mutex_lock(&hal->evq_lock);
    ret = evq_put(&hal->events, &event);
    if (ret == E_OK) {
        pthread_cond_signal(&hal->evq_cond);
    }
    pthread_mutex_unlock(&hal->evq_lock);
    return ret;
}

sensor_bus_type_t port_bus_type(void* port)
{
    portLinux_t * p = port;
    return p->bus_type;
}

char* port_strerror(int errnum)
{
    return strerror(errnum);
}

void* get_platform(void* cfg)
{
    linuxConfig_t * config = cfg;
    portLinux_t* portLinux = malloc(sizeof(portLinux_t));
    if (portLinux) {
        memset(portLinux, 0, sizeof(portLinux_t));
        portLinux->interrupt_pin = config->interrupt_pin;
        portLinux->i2c_addr = config->i2c_addr > 0 ? config->i2c_addr : SMARTSENSOR_I2C_ADDR;
        strncpy(portLinux->bus, config->bus_res, sizeof(portLinux->bus));
        portLinux->event_callback = config->event_callback;
        portLinux->event_callback_ctx = config->event_callback_ctx;
        portLinux->bus_type = config->bus_type;
        portLinux->base.init = linux_init;
        portLinux->base.deinit = linux_deinit;
        portLinux->base.read = port_comm_read;
        portLinux->base.write = port_comm_write;
        portLinux->base.delay = port_sleep_ms;
        portLinux->base.bus_type = port_bus_type;
        portLinux->base.event_get = port_event_get;
        portLinux->base.event_put = port_event_put;
        portLinux->base.timer_start = port_heartbeat_start;
        portLinux->base.timer_stop = port_heartbeat_stop;
        portLinux->base.strerror = port_strerror;
    }
    return (void*) portLinux;
}