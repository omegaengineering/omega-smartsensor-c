/*!********************************************************************************************
  @file     gpio.h

  @copyright
            Copyright (c) 2019, Omega Engineering Inc.

            Permission is hereby granted, free of charge, to any person obtaining
            a copy of this software and associated documentation files (the
            'Software'), to deal in the Software without restriction, including
            without limitation the rights to use, copy, modify, merge, publish,
            distribute, sublicense, and/or sell copies of the Software, and to
            permit persons to whom the Software is furnished to do so, subject to
            the following conditions:

            The above copyright notice and this permission notice shall be
            included in all copies or substantial portions of the Software.

            THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
            EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
            MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
            IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
            CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
            TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
            SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  @author   Binh Dinh
  @date     June 5th, 2019
  @details


***********************************************************************************************/

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
