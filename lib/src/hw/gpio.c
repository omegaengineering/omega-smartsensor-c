/*!********************************************************************************************
  @file     gpio.c

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

#include "hw/gpio.h"

int gpio_open(gpio_t * ctx, void * cfg)
{
    return ctx->driver->gpio_open(ctx, cfg);
}

int gpio_close(gpio_t * ctx)
{
    return ctx->driver->gpio_close(ctx);
}

int gpio_set_value(gpio_t * ctx, int value)
{
    return ctx->driver->gpio_set_value(ctx, value);
}

int gpio_get_value(gpio_t * ctx, int * value)
{
    return ctx->driver->gpio_get_value(ctx, value);
}

int gpio_set_direction(gpio_t * ctx, gpio_direction_t direction)
{
    return ctx->driver->gpio_set_direction(ctx, direction);
}
