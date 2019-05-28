#ifndef OMEGA_SMARTSENSOR_C_GPIO_H
#define OMEGA_SMARTSENSOR_C_GPIO_H


typedef enum {
    GPIO_DIRECTION_INPUT,
    GPIO_DIRECTION_OUTPUT
} gpio_direction_t;

typedef enum {
    GPIO_EDGE_RISING,
    GPIO_EDGE_FALLING
} gpio_edge_t;

typedef struct _gpio gpio_t;
typedef void (*interrupt_handler)();

typedef int (*gpio_open_dr)(gpio_t * ctx, void *cfg);
typedef int (*gpio_close_dr)(gpio_t * ctx);
typedef int (*gpio_set_direction_dr)(gpio_t * ctx, gpio_direction_t direction);
typedef int (*gpio_set_value_dr)(gpio_t * ctx, int value);
typedef int (*gpio_get_value_dr)(gpio_t * ctx, int * value);
typedef int (*gpio_get_handle_dr)(gpio_t * ctx);

typedef struct {
    gpio_open_dr            gpio_open;
    gpio_close_dr           gpio_close;
    gpio_set_value_dr       gpio_set_value;
    gpio_get_value_dr       gpio_get_value;
    gpio_set_direction_dr   gpio_set_direction;
    gpio_get_handle_dr      gpio_get_handle;
} gpio_driver_t;

struct _gpio {
    const gpio_driver_t*    driver;
//    void*                   driver_data;
};

int gpio_open(gpio_t * ctx, void *cfg);
int gpio_close(gpio_t * ctx);
int gpio_set_value(gpio_t * ctx, int value);
int gpio_get_value(gpio_t * ctx, int * value);
int gpio_set_direction(gpio_t * ctx, gpio_direction_t direction);
int gpio_get_handle(gpio_t * ctx);
int gpio_set_interrupt_handler(gpio_t * ctx, interrupt_handler handler);

#endif //OMEGA_SMARTSENSOR_C_GPIO_H
