#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "smartsensor.h"
#include "linux_private.h"


int linux_i2c_open(linux_i2c_t* i2c, const char* resource, sensor_bus_type_t type)
{
    if (i2c) {
        if ((i2c->fd = open(resource, O_RDWR)) < 0)
            return E_UNAVAILABLE;
        if (type == SENSOR_BUS_I2C) {
            if ((ioctl(i2c->fd, I2C_SLAVE, SMARTSENSOR_I2C_ADDR)) < 0)
                return E_BUS_OPERATION;
        }
        return E_OK;
    }
    return E_UNAVAILABLE;
}

int linux_i2c_read(linux_i2c_t* i2c, uint16_t reg_addr, uint8_t* buffer, uint16_t buffer_size)
{
    int nRead, nTotal = 0;

    // write i2c reg address (1 byte)
    uint8_t reg = reg_addr & 0xFF;
    if (write(i2c->fd, &reg, 1) != 1)
        return E_BUS_OPERATION;

    while (nTotal < buffer_size) {
        nRead = read(i2c->fd, buffer, buffer_size);
        if (nRead < 0 && errno == EINTR)
            continue;
        else if (nRead > 0)
            nTotal += nRead;
        else
            break;
    }
    if (nTotal != buffer_size)
        return E_BUS_OPERATION;
    return E_OK;
}

int linux_i2c_write(linux_i2c_t* i2c, uint16_t reg_addr, const uint8_t* buffer, uint16_t buffer_size)
{
    int nWritten, nTotal = 0;

    // we have to write the data to the bus at one shot starting with reg addr
    if (buffer_size > sizeof(i2c->i2c_buf) - 1)
        return E_BUFFER_MEM_SIZE;

    // prepare the buffer
    i2c->i2c_buf[0] = reg_addr & 0xffU;
    memcpy(i2c->i2c_buf + 1, buffer, buffer_size);

    while (nTotal < buffer_size)
    {
        nWritten = write(i2c->fd, buffer, buffer_size);
        if (nWritten < 0 && errno == EINTR)
            continue;
        else if (nWritten > 0)
            nTotal += nWritten;
        else
            break;
    }
    if (nTotal != buffer_size)
        return E_BUS_OPERATION;
    return E_OK;
}

int linux_i2c_close(linux_i2c_t* i2c)
{
    return close(i2c->fd);
}


