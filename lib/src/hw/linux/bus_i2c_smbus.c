#include <linux/i2c-dev.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "platform/memory.h"
#include "common/errors.h"
#include "common/common.h"
#include "common/log.h"
#include "hw/linux/bus_i2c_smbus.h"

s19_log_create("Smbus");

static const bus_driver_t bus_i2c_driver = {
        .bus_open = bus_smbus_open,
        .bus_read = bus_smbus_read,
        .bus_write = bus_smbus_write,
        .bus_close = bus_smbus_close,
        .bus_read_raw = bus_smbus_read_raw,
        .bus_write_raw = bus_smbus_write_raw,
        .bus_free = bus_smbus_free
};

int bus_smbus_new(bus_t **ctx)
{
    bus_smbus_t *new_bus = s19_mem_malloc(sizeof(bus_smbus_t));
    if (new_bus == NULL)
        return E_NO_MEM;
    memset(new_bus, 0, sizeof(bus_smbus_t));
    new_bus->base.driver = &bus_i2c_driver;
    *ctx = (bus_t*) new_bus;
    return E_OK;
}

int bus_smbus_free(bus_t *ctx)
{
    if (ctx)
        s19_mem_free(ctx);
    return E_OK;
}


int bus_smbus_open(bus_t *ctx, void *cfg)
{
    int ret;
    bus_smbus_t     *p_bus = (bus_smbus_t*) ctx;
    bus_smbus_cfg_t *smbus_cfg = (bus_smbus_cfg_t*) cfg;

    ret = open(p_bus->base.bus_id, O_RDWR);
    if (ret < 0)
        return E_BUS_UNAVAILABLE;
    p_bus->fd = ret;
    ret = ioctl(p_bus->fd, I2C_SLAVE, smbus_cfg->dev_addr);
    if (ret < 0)
        return E_BUS_OPERATION;
    return E_OK;
}

int bus_smbus_read_raw(bus_t *ctx, data_buffer_t *buffer)
{
    int ret;
    bus_smbus_t *p_bus = (bus_smbus_t*) ctx;
    ret = read(p_bus->fd, buffer->data, buffer->data_len);
//    s19_log_dbg("Smbus Read: status %d\n", ret);
//    print_buffer(buffer);
    if (ret != buffer->data_len)
        return E_BUS_OPERATION;
    return E_OK;
}

/// standard i2c "1 byte wide register"
/// bus data returned: MSB
/// bus will write Least sig byte of reg_addr, then read up to buffer_len into buffer
int bus_smbus_read(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer)
{
    int ret;
    bus_smbus_t *p_bus = (bus_smbus_t*) ctx;
    uint8_t addr_buf[2];

    addr_buf[0] = reg_addr & 0xff;  // register is 1 byte only
    ret = write(p_bus->fd, addr_buf, 1);
    if (ret != 1)
        return E_BUS_OPERATION;
    ret = bus_smbus_read_raw(ctx, buffer);
    return ret;
}

int bus_smbus_write_raw(bus_t *ctx, data_buffer_t *buffer)
{
    int ret;
    bus_smbus_t *p_bus = (bus_smbus_t*) ctx;
//    s19_log_dbg("SMBUS Write:\n");
//    print_buffer(buffer);
    ret = write(p_bus->fd, buffer->data, buffer->data_len);
    if (ret != buffer->data_len)
        return E_BUS_OPERATION;
    return E_OK;
}

int bus_smbus_write(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer)
{
    int ret;
    bus_smbus_t *p_bus = (bus_smbus_t*) ctx;
    uint8_t temp[48];
    data_buffer_t temp_buffer = {.data = temp, .data_len = buffer->data_len + 1};
    if (buffer->data_len > sizeof(temp) - 1)
        return E_BUFFER_MEM_SIZE;

    // we have to write the data to the bus at one shot starting with reg addr
    temp[0] = reg_addr & 0xff;
    memcpy(temp + 1, buffer->data, buffer->data_len);

    ret = bus_smbus_write_raw(ctx, &temp_buffer);
    return ret;
}

int bus_smbus_close(bus_t *ctx)
{
    bus_smbus_t *p_bus = (bus_smbus_t*) ctx;
    close(p_bus->fd);
    return E_OK;
}
