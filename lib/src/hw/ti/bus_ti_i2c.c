/*!********************************************************************************************
  @file     bus_ti_i2c.c

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

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "platform/memory.h"
#include "common/errors.h"
#include "common/common.h"
#include "common/log.h"
#include "hw/ti/bus_ti_i2c.h"

typedef struct {
    bus_t           base;
    I2C_Handle      i2c_handle;
    I2C_Transaction i2c_trans;                  /**< transaction info */
    uint8_t         buffer[I2C_BUFFER_SIZE];    /**< internal buffer */
} bus_ti_i2c_t;


static bus_ti_i2c_t instances[I2C_INSTANCE_CNT];

static const bus_driver_t bus_ti_i2c_driver = {
        .bus_open = bus_ti_i2c_open,
        .bus_read = bus_ti_i2c_read,
        .bus_write = bus_ti_i2c_write,
        .bus_close = bus_ti_i2c_close,
        .bus_read_raw = bus_ti_i2c_read_raw,
        .bus_write_raw = bus_ti_i2c_write_raw,
        .bus_free = bus_ti_i2c_free
};

int bus_ti_i2c_new(bus_t **ctx)
{
    for (int i = 0; i < I2C_INSTANCE_CNT; i++)
    {
        if (instances[i].base.driver == NULL)
        {
            instances[i].base.driver = &bus_ti_i2c_driver;
            *ctx = (bus_t*) &instances[i];
            return E_OK;
        }
    }
    return E_NO_MEM;
}

int bus_ti_i2c_free(bus_t *ctx)
{
    bus_ti_i2c_t * pbus        = (bus_ti_i2c_t*) ctx;
    memset(pbus, 0, sizeof(bus_ti_i2c_t));
    return E_OK;
}

/// this driver will setup blocking I2C mode
int bus_ti_i2c_open(bus_t *ctx, void *cfg)
{
    I2C_Params          i2cParams;
    I2CCC26XX_I2CPinCfg st_I2C_Option;

    bus_ti_i2c_t * pbus        = (bus_ti_i2c_t*) ctx;
    bus_ti_i2c_cfg_t * i2c_cfg = (bus_ti_i2c_cfg_t*) cfg;

    (void) I2C_init();

    (void) I2C_Params_init(&i2cParams);
    i2cParams.bitRate = i2c_cfg->bit_rate;
    i2cParams.transferMode = I2C_MODE_BLOCKING;
    i2cParams.transferCallbackFxn = NULL;

    st_I2C_Option.pinSCL = i2c_cfg->clk_pin;
    st_I2C_Option.pinSDA = i2c_cfg->sda_pin;
    i2cParams.custom = &st_I2C_Option;
    pbus->i2c_handle = I2C_open(i2c_cfg->periph_idx, &i2cParams);
    pbus->i2c_trans.slaveAddress = i2c_cfg->slave_addr;

    return pbus->i2c_handle ? E_OK : E_BUS_OPERATION;
}

int bus_ti_i2c_read_raw(bus_t *ctx, data_buffer_t *buffer)
{
    bool ret;
    bus_ti_i2c_t * pbus        = (bus_ti_i2c_t*) ctx;
    pbus->i2c_trans.readBuf = (uint8_t *) buffer->data;
    pbus->i2c_trans.readCount = buffer->data_len;
    pbus->i2c_trans.writeBuf = NULL;
    pbus->i2c_trans.writeCount = 0;

    ret = I2C_transfer(pbus->i2c_handle, &pbus->i2c_trans);
    return ret ? E_OK : E_BUS_OPERATION;
}

/// 1 byte reg addr only
int bus_ti_i2c_read(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer)
{
    bool ret;
    bus_ti_i2c_t * pbus        = (bus_ti_i2c_t*) ctx;
    pbus->i2c_trans.writeBuf = (uint8_t *) &reg_addr;
    pbus->i2c_trans.writeCount = 1;
    pbus->i2c_trans.readBuf = (uint8_t *) buffer->data;
    pbus->i2c_trans.readCount = buffer->data_len;

    ret = I2C_transfer(pbus->i2c_handle, &pbus->i2c_trans);
    return ret ? E_OK : E_BUS_OPERATION;
}

int bus_ti_i2c_write_raw(bus_t *ctx, data_buffer_t *buffer)
{
    bool ret;
    bus_ti_i2c_t * pbus        = (bus_ti_i2c_t*) ctx;
    pbus->i2c_trans.writeBuf = (uint8_t *) buffer->data;
    pbus->i2c_trans.writeCount = buffer->data_len;
    pbus->i2c_trans.readBuf = NULL;
    pbus->i2c_trans.readCount = 0;

    ret = I2C_transfer(pbus->i2c_handle, &pbus->i2c_trans);
    return ret ? E_OK : E_BUS_OPERATION;
}

/// 1 byte addr only
int bus_ti_i2c_write(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer)
{
    bool ret;
    bus_ti_i2c_t * pbus        = (bus_ti_i2c_t*) ctx;

    if (sizeof(pbus->buffer) < buffer->data_len + 1)
        return E_API_ERROR;
    pbus->buffer[0] = reg_addr & 0xff;
    memcpy(pbus->buffer + 1, buffer->data, buffer->data_len);
    data_buffer_t temp = {.data = pbus->buffer, .data_len = buffer->data_len + 1};

    ret = bus_ti_i2c_write_raw(ctx, &temp);
    return ret;
}

int bus_ti_i2c_close(bus_t *ctx)
{
    bus_ti_i2c_t * pbus        = (bus_ti_i2c_t*) ctx;
    if (pbus->i2c_handle)
        I2C_close(pbus->i2c_handle);
    return E_OK;
}
