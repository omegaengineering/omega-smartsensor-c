#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "smartsensor.h"
#include "port_linux.h"

typedef struct _bus_i2c {
    uint8_t     opened;
    int         fd;
} bus_i2c_t;

typedef struct _bus_i2c_cfg {
    uint8_t     bus_id;
} bus_i2c_cfg_t;

static bus_i2c_t bus_i2c[SMARTSENSOR_MAX];

static bus_i2c_cfg_t bus_i2c_cfg[SMARTSENSOR_MAX] = {
    {
        .bus_id = SMARTSENSOR_0_I2C
    }
};

static bus_i2c_t* get_bus(int instance)
{
    if (instance >= SMARTSENSOR_MAX)
        return NULL;
    return &bus_i2c[instance];
}

static bus_i2c_cfg_t* get_bus_cfg(int instance)
{
    if (instance >= SMARTSENSOR_MAX)
        return NULL;
    return &bus_i2c_cfg[instance];
}

int port_bus_open(int instance)
{
    bus_i2c_t* bus = get_bus(instance);
    bus_i2c_cfg_t* bus_cfg = get_bus_cfg(instance);

    if (bus && bus_cfg) {
        if (bus->opened)
            return E_UNAVAILABLE;
        char path[32];
        sprintf(path, "/dev/i2c-%d", (int)bus_cfg->bus_id);
        if ((bus->fd = open(path, O_RDWR)) < 0)
            return E_UNAVAILABLE;
        if ((ioctl(bus->fd, I2C_SLAVE, SMARTSENSOR_I2C_ADDR)) < 0)
            return E_BUS_OPERATION;
        bus->opened = 1;
        return E_OK;
    }
    return E_UNAVAILABLE;
}

int port_bus_read(int bus_id, data_buffer_t *buffer)
{
    int nRead, nTotal = 0;
    bus_i2c_t* bus = get_bus(bus_id);
    if (bus) {
        while (nTotal < buffer->data_len) {
            nRead = read(bus->fd, buffer->data, buffer->data_len);
            if (nRead < 0 && errno == EINTR)
                continue;
            else if (nRead > 0)
                nTotal += nRead;
            else
                break;
        }
        if (nTotal != buffer->data_len)
            return E_BUS_OPERATION;
        return E_OK;
    }
    return E_UNAVAILABLE;
}

int port_bus_write(int bus_id, data_buffer_t *buffer)
{
    int nWritten, nTotal = 0;
    bus_i2c_t* bus = get_bus(bus_id);
    if (bus) {
        while (nTotal < buffer->data_len)
        {
            nWritten = write(bus->fd, buffer->data, buffer->data_len);
            if (nWritten < 0 && errno == EINTR)
                continue;
            else if (nWritten > 0)
                nTotal += nWritten;
            else
                break;
        }
        if (nTotal != buffer->data_len)
            return E_BUS_OPERATION;
        return E_OK;
    }
    return E_UNAVAILABLE;
}

int port_bus_close(int instance)
{
    bus_i2c_t* bus = get_bus(instance);
    if (bus) {
        if (bus->opened)
            close(bus->fd);
        bus->opened = 0;
    }
    return E_UNAVAILABLE;
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

int port_interrupt_open(int instance)
{

}

int port_interrupt_close(int instance)
{

}


