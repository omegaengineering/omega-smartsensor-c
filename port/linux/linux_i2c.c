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

int linux_i2c_read(linux_i2c_t* i2c, uint8_t* buffer, uint16_t buffer_size)
{
    int nRead, nTotal = 0;

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

int linux_i2c_write(linux_i2c_t* i2c, const uint8_t* buffer, uint16_t buffer_size)
{
    int nWritten, nTotal = 0;

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

///// standard i2c "1 byte wide register"
///// bus data returned: MSB
///// bus will write Least sig byte of reg_addr, then read up to buffer_len into buffer
//int port_bus_read(int bus_id, uint16_t reg_addr, data_buffer_t *buffer)
//{
//    int ret;
//    bus_i2c_t* bus = get_bus(bus_id);
//    if (bus) {
//        uint8_t addr_buf[2];
//
//        addr_buf[0] = reg_addr & 0xffU;  // register is 1 byte only
//        ret = write(bus->fd, addr_buf, 1);
//        if (ret != 1)
//            return E_BUS_OPERATION;
//        ret = port_bus_read_ll(bus, buffer);
//        return ret;
//    }
//    return E_UNAVAILABLE;
//}
//
//int port_bus_write(int bus_id, uint16_t reg_addr, data_buffer_t *buffer)
//{
//    int ret;
//    bus_i2c_t* bus = get_bus(bus_id);
//    if (bus) {
//        uint8_t temp[48];
//        data_buffer_t temp_buffer = {.data = temp, .data_len = buffer->data_len + 1};
//        if (buffer->data_len > sizeof(temp) - 1)
//            return E_BUFFER_MEM_SIZE;
//
//        // we have to write the data to the bus at one shot starting with reg addr
//        temp[0] = reg_addr & 0xffU;
//        memcpy(temp + 1, buffer->data, buffer->data_len);
//
//        ret = port_bus_write_ll(bus, &temp_buffer);
//        return ret;
//    }
//    return E_UNAVAILABLE;
//}


