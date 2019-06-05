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
