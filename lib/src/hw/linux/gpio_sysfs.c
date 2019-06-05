/*!********************************************************************************************
  @file     gpio_sysfs.c

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

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "platform/memory.h"
#include "hw/linux/gpio_sysfs.h"
#include "common/errors.h"

#define MAX_GPIO_NAME_LEN   128


gpio_driver_t gpio_posix_dr = {
        .gpio_open = gpio_sysfs_open,
        .gpio_close = gpio_sysfs_close,
        .gpio_set_direction = gpio_sysfs_set_direction,
        .gpio_set_value = gpio_sysfs_set_value,
        .gpio_get_value = gpio_sysfs_get_value
};

int gpio_sysfs_export(gpio_t *ctx)
{
    int ret;
    gpio_sysfs_t *gpio = (gpio_sysfs_t*) ctx;
    char file[MAX_GPIO_NAME_LEN];
    sprintf(file, "/sys/class/gpio/gpio%d", gpio->pin);
    if (access(file, F_OK) >= 0)
        return E_OK;
    sprintf(file, "/sys/class/gpio/export");
    int fd = open(file, O_RDWR);
    if (fd < 0)
        return E_BUS_UNAVAILABLE;
    sprintf(file, "%d\n", gpio->pin);
    ret = write(fd, file, strlen(file));
    close(fd);
    if (ret < 0)
        return E_BUS_UNAVAILABLE;
    usleep(200000);    // wait a bit for the sysfs to show up
    return E_OK;
}

int gpio_sysfs_unexport(gpio_t *ctx)
{
    int ret;
    char file[MAX_GPIO_NAME_LEN];
    gpio_sysfs_t *gpio = (gpio_sysfs_t*) ctx;
    sprintf(file, "/sys/class/gpio/unexport");
    if (access(file, F_OK) >= 0)
        return E_OK;
    int fd = open(file, O_RDWR);
    if (fd < 0)
        return E_BUS_UNAVAILABLE;
    sprintf(file, "%d\n", gpio->pin);
    ret = write(fd, file, strlen(file));
    close(fd);
    if (ret < 0)
        return E_BUS_UNAVAILABLE;
    return E_OK;
}

int gpio_sysfs_open(gpio_t *ctx, void *cfg)
{
    int ret;
    gpio_sysfs_t *gpio = (gpio_sysfs_t*) ctx;
    gpio_sysfs_cfg_t *gpio_cfg = (gpio_sysfs_cfg_t*) cfg;


    char file[MAX_GPIO_NAME_LEN];
    sprintf(file, "/sys/class/gpio/gpio%d/value", gpio->pin);
    ret = open(file, O_RDONLY);
    if (ret < 0)
        return E_BUS_UNAVAILABLE;
    gpio->fd = ret;
    return E_OK;
}

int gpio_sysfs_close(gpio_t *ctx)
{
    gpio_sysfs_t *gpio = (gpio_sysfs_t*) ctx;
    if (gpio->fd >= 0)
        return close(gpio->fd);
    return E_OK;
}

int gpio_sysfs_set_value(gpio_t *ctx, int value)
{
    return E_NOT_SUPPORTED;
}

int gpio_sysfs_get_value(gpio_t *ctx, int *value)
{
    return E_NOT_SUPPORTED;
}

int gpio_sysfs_set_direction(gpio_t *ctx, gpio_direction_t direction)
{
    int ret, fd;
    gpio_sysfs_t *gpio = (gpio_sysfs_t*) ctx;
    char file[MAX_GPIO_NAME_LEN];
    sprintf(file, "/sys/class/gpio/gpio%d/direction", gpio->pin);
    fd = open(file, O_RDWR);
    if (fd < 0)
        return E_BUS_UNAVAILABLE;
    if (direction == GPIO_DIRECTION_INPUT)
        ret = write(fd, "in\n", sizeof("in\n"));
    else if (direction == GPIO_DIRECTION_OUTPUT)
        ret = write(fd, "out\n", sizeof("out\n"));
    else
        ret = -1;
    close(fd);
    if (ret <= 0)
        return E_BUS_OPERATION;
    return E_OK;
}

int gpio_sysfs_set_interrupt_edge(gpio_t *ctx, gpio_edge_t edge)
{
    int ret, fd;
    char file[MAX_GPIO_NAME_LEN];
    gpio_sysfs_t *gpio = (gpio_sysfs_t*) ctx;
    sprintf(file, "/sys/class/gpio/gpio%d/edge", gpio->pin);
    fd = open(file, O_RDWR);
    if (fd < 0)
        return E_BUS_UNAVAILABLE;
    if (edge == GPIO_EDGE_RISING)
        ret = write(fd, "rising\n", sizeof("rising\n"));
    else if (edge == GPIO_EDGE_FALLING)
        ret = write(fd, "falling\n", sizeof("falling\n"));
    else
        ret = -1;
    close(fd);
    if (ret <= 0)
        return E_BUS_OPERATION;
    return E_OK;
}