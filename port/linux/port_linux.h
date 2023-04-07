#ifndef SMARTSENSOR_LINUX_H
#define SMARTSENSOR_LINUX_H

#include <pthread.h>
#include "port.h"

typedef union {
    struct comm_i2c {
        const char* bus;
        uint8_t     i2c_addr;
        int         interrupt_pin;
    } i2c;
    struct comm_modbus {
        const char* bus;
        uint8_t     modbus_addr;
        uint32_t    baud_rate;
        uint8_t     parity;
        uint8_t     data_bit;
        uint8_t     stop_bit;
    } modbus;
} comm_config_t;

typedef struct {
    sensor_bus_type_t   bus_type;
    comm_config_t       comm;
    event_callback_t    event_callback;     // set to handle sensor event automatically, unset to opt out
    void*               event_callback_ctx;
} linuxConfig_t;

void* get_platform(void* config);
pthread_mutex_t* create_shared_mutex(void);
void destroy_shared_mutex(pthread_mutex_t* shared_mutex);


#endif //SMARTSENSOR_LINUX_H
