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

#if 1
#include <i2c/smbus.h>
#endif


int linux_i2c_open(linux_i2c_t* i2c, const char* resource, uint8_t i2c_address)
{
    if ((i2c->fd = open(resource, O_RDWR)) < 0) {
        return SET_PORT_ERR(errno);
    }
    if ((ioctl(i2c->fd, I2C_SLAVE, i2c_address)) < 0) {
        return SET_PORT_ERR(errno);
    }
    return E_OK;
}

#if 0
int linux_i2c_read(linux_i2c_t* i2c, uint16_t reg_addr, uint8_t* buffer, uint16_t buffer_size)
{
    int nRead, nTotal = 0;

    // write i2c reg address (1 byte)
    uint8_t reg = reg_addr & 0xFF;
    if (write(i2c->fd, &reg, 1) != 1) {
        perror("yyy");
        return SET_PORT_ERR(errno);
    }

    while (nTotal < buffer_size) {
        nRead = read(i2c->fd, buffer, buffer_size);
        if (nRead < 0 && errno == EINTR)
            continue;
        else if (nRead > 0)
            nTotal += nRead;
        else {
            perror("read 0");
            printf("error %d\n", errno);
            break;
        }
    }
    if (nTotal != buffer_size) {
        printf("xxx\n");
        perror("xxx");
        return E_BUS_OPERATION;
    }
    return E_OK;
}
#endif

#if 1
int linux_i2c_read(linux_i2c_t* i2c, uint16_t reg_addr, uint8_t* buffer, uint16_t buffer_size)
{
    int nRead, nTotal = 0;

    // write i2c reg address (1 byte)
    uint8_t reg = reg_addr & 0xFF;

    while (nTotal < buffer_size) {
        nRead = i2c_smbus_read_i2c_block_data(i2c->fd, reg, buffer_size, buffer);
        if (nRead < 0 && errno == EINTR)
            continue;
        else if (nRead > 0)
            nTotal += nRead;
        else {
            perror("read 0");
            printf("error %d\n", errno);
            break;
        }
    }
    if (nTotal != buffer_size) {
        printf("xxx\n");
        perror("xxx");
        return E_BUS_OPERATION;
    }
    return E_OK;
}
#endif

#if 0
int linux_i2c_write(linux_i2c_t* i2c, uint16_t reg_addr, const uint8_t* buffer, uint16_t buffer_size)
{
    int nWritten, nTotal = 0;

    // we have to write the data to the bus at one shot starting with reg addr
    if (buffer_size > sizeof(i2c->i2c_buf) - 1)
        return E_BUFFER_MEM_SIZE;

    // prepare the buffer
    i2c->i2c_buf[0] = reg_addr & 0xffU;
    memcpy(i2c->i2c_buf + 1, buffer, buffer_size);
    buffer_size += 1;   // add 1 for the first byte

    while (nTotal < buffer_size)
    {
        nWritten = write(i2c->fd, i2c->i2c_buf, buffer_size);
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
#endif

#if 1
int linux_i2c_write(linux_i2c_t* i2c, uint16_t reg_addr, const uint8_t* buffer, uint16_t buffer_size)
{
    int ret;
    reg_addr &= 0xff;

    // this func does not return nWritten, we cannot check for completeness
    // the driver code is responsible for completeness
    ret = i2c_smbus_write_i2c_block_data(i2c->fd, reg_addr, buffer_size, buffer);
    if (ret) {
        ret = SET_PORT_ERR(errno);
    }
    return ret;
}
#endif

int linux_i2c_close(linux_i2c_t* i2c)
{
    return close(i2c->fd);
}


