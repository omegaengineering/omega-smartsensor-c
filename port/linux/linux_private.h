#ifndef SMARTSENSOR_PORT_LINUX_H
#define SMARTSENSOR_PORT_LINUX_H

#include "port_linux.h"

#if I2C_SENSOR
typedef struct _bus_i2c {
    int         fd;
} linux_i2c_t;

int  linux_i2c_open(linux_i2c_t* i2c, const char* resource, sensor_bus_type_t type);
int  linux_i2c_close(linux_i2c_t* i2c);
int  linux_i2c_read(linux_i2c_t* i2c, uint8_t* buffer, uint16_t buffer_size);
int  linux_i2c_write(linux_i2c_t* i2c, const uint8_t* buffer, uint16_t buffer_size);
#endif

typedef struct {
    int                     fd;
    uint16_t                period;
    uint8_t                 running;
} linux_timer_t;

int linux_timer_start(linux_timer_t* timer, int period_ms);
int linux_timer_stop(linux_timer_t* timer);
int linux_timer_get_fd(linux_timer_t* timer);
int linux_timer_is_running(linux_timer_t* timer);

typedef struct {
    port_event_t     event;

} ev_data_t;

typedef struct _ev_queue {
    uint16_t        size;
    uint16_t        read;
    uint16_t        write;
    ev_data_t *     queue;
} ev_queue_t;


int evq_init(ev_queue_t* evq, int size);
int evq_put(ev_queue_t* evq, const port_event_t* event);
int evq_get(ev_queue_t* evq, port_event_t* event);
int evq_close(ev_queue_t* evq);

typedef struct {
    int         fd;
    int         pin;
} gpio_t;

int gpio_init(gpio_t * gpio, int pin);
int gpio_export(gpio_t *gpio);
int gpio_unexport(gpio_t *gpio);
int gpio_open(gpio_t *gpio);
int gpio_close(gpio_t *gpio);
int gpio_set_direction(gpio_t *gpio, uint8_t direction_input);
int gpio_set_interrupt_edge(gpio_t *gpio, uint8_t edge_rising);
int gpio_get_fd(gpio_t* gpio);

#ifdef MODBUS_SENSOR
#include <modbus/modbus-rtu.h>
#include <modbus/modbus.h>

#define MODBUS_BUFFER_SIZE      128      /**< buffer for manipulating user data to modbus endianess */

typedef struct {
    modbus_t*       mb;
    uint16_t        dev_addr;
    uint8_t         buffer[MODBUS_BUFFER_SIZE];
} linux_modbus_t;

int linux_modbus_open(linux_modbus_t* mb, const char* device);
int linux_modbus_read(linux_modbus_t* mb, uint16_t reg_addr, void* buffer, uint16_t buffer_sz);
int linux_modbus_write(linux_modbus_t* mb, uint16_t reg_addr, void* buffer, uint16_t buffer_sz);
int linux_modbus_close(linux_modbus_t* mb);
#endif

#endif //SMARTSENSOR_PORT_LINUX_H
