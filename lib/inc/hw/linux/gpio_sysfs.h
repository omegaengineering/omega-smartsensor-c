
#ifndef OMEGA_SMARTSENSOR_C_GPIO_SYSFS_H
#define OMEGA_SMARTSENSOR_C_GPIO_SYSFS_H

#include "hw/gpio.h"

typedef struct {
    gpio_t  base;
    int     pin;
    int     fd;
} gpio_sysfs_t;

typedef struct {

} gpio_sysfs_cfg_t;

int gpio_sysfs_open(gpio_t *ctx, void *cfg);
int gpio_sysfs_set_interrupt_edge(gpio_t *ctx, gpio_edge_t edge);
int gpio_sysfs_set_direction(gpio_t *ctx, gpio_direction_t direction);
int gpio_sysfs_get_value(gpio_t *ctx, int *value);
int gpio_sysfs_set_value(gpio_t *ctx, int value);
int gpio_sysfs_close(gpio_t *ctx);


int gpio_sysfs_export(gpio_t *ctx);
int gpio_sysfs_unexport(gpio_t *ctx);

#endif //OMEGA_SMARTSENSOR_C_GPIO_SYSFS_H
