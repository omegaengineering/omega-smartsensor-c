#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "port_linux.h"

#define MAX_GPIO_NAME_LEN   128



int gpio_init(gpio_t * gpio, int pin)
{
    gpio->fd = -1;
    gpio->pin = pin;
    return E_OK;
}

int gpio_export(gpio_t *gpio)
{
    int ret;
    int fd;
    char file[MAX_GPIO_NAME_LEN];
    sprintf(file, "/sys/class/gpio/gpio%d", gpio->pin);
    if (access(file, F_OK) >= 0)
        return E_OK;
    sprintf(file, "/sys/class/gpio/export");
    if ((fd = open(file, O_RDWR)) < 0)
        return E_UNAVAILABLE;
    sprintf(file, "%d\n", gpio->pin);
    ret = write(fd, file, strlen(file));
    close(fd);
    usleep(200000);    // wait a bit for the sysfs to show up
    return E_OK;
}

int gpio_unexport(gpio_t *gpio)
{
    int ret;
    int fd;
    char file[MAX_GPIO_NAME_LEN];
    sprintf(file, "/sys/class/gpio/unexport");
    if (access(file, F_OK) >= 0)
        return E_OK;
    if ((fd = open(file, O_RDWR)) < 0)
        return E_UNAVAILABLE;
    sprintf(file, "%d\n", gpio->pin);
    ret = write(fd, file, strlen(file));
    close(fd);
    return E_OK;
}

int gpio_open(gpio_t *gpio)
{
    int ret;
    char file[MAX_GPIO_NAME_LEN];
    sprintf(file, "/sys/class/gpio/gpio%d/value", gpio->pin);
    if ((gpio->fd = open(file, O_RDONLY)) < 0) {
        return E_UNAVAILABLE;
    }
    else {
        return E_OK;
    }
}

int gpio_close(gpio_t *gpio)
{
    if (gpio->fd >= 0)
        return close(gpio->fd);
    return E_OK;
}


int gpio_set_direction(gpio_t *gpio, uint8_t direction_input)
{
    int ret, fd;
    char file[MAX_GPIO_NAME_LEN];
    sprintf(file, "/sys/class/gpio/gpio%d/direction", gpio->pin);
    if ((fd = open(file, O_RDWR)) < 0)
        return E_UNAVAILABLE;
    if (direction_input)
        ret = write(fd, "in\n", sizeof("in\n"));
    else
        ret = write(fd, "out\n", sizeof("out\n"));;
    close(fd);
    return E_OK;
}

int gpio_set_interrupt_edge(gpio_t *gpio, uint8_t edge_rising)
{
    int ret, fd;
    char file[MAX_GPIO_NAME_LEN];
    sprintf(file, "/sys/class/gpio/gpio%d/edge", gpio->pin);
    if ((fd = open(file, O_RDWR)) < 0)
        return E_UNAVAILABLE;
    if (edge_rising)
        ret = write(fd, "rising\n", sizeof("rising\n"));
    else
        ret = write(fd, "falling\n", sizeof("falling\n"));
    close(fd);
    return E_OK;
}

int gpio_get_fd(gpio_t* gpio)
{
    return gpio->fd;
}
