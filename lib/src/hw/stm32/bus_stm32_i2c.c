/*!********************************************************************************************
  @file     bus_stm32_i2c.c

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
  @date     August 2nd, 2019
  @details


***********************************************************************************************/

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stm32l0xx_hal.h>
#include <stm32l0xx_hal.h>
#include "platform/memory.h"
#include "common/errors.h"
#include "common/common.h"
#include "common/log.h"
#include "hw/stm32/bus_stm32_i2c.h"

typedef struct {
    bus_t             base;
    I2C_HandleTypeDef hi2c;
    uint8_t           addr;
    uint32_t          timeout;
    uint8_t           buffer[I2C_BUFFER_SIZE];    /**< internal buffer */
} bus_stm32_i2c_t;


static bus_stm32_i2c_t instances[I2C_INSTANCE_CNT];

static const bus_driver_t bus_stm32_i2c_driver = {
        .bus_open = bus_stm32_i2c_open,
        .bus_read = bus_stm32_i2c_read,
        .bus_write = bus_stm32_i2c_write,
        .bus_close = bus_stm32_i2c_close,
        .bus_read_raw = bus_stm32_i2c_read_raw,
        .bus_write_raw = bus_stm32_i2c_write_raw,
        .bus_free = bus_stm32_i2c_free
};

int bus_stm32_i2c_new(bus_t **ctx)
{
    for (int i = 0; i < I2C_INSTANCE_CNT; i++)
    {
        if (instances[i].base.driver == NULL)
        {
            instances[i].base.driver = &bus_stm32_i2c_driver;
            *ctx = (bus_t*) &instances[i];
            return E_OK;
        }
    }
    return E_NO_MEM;
}

int bus_stm32_i2c_free(bus_t *ctx)
{
    bus_stm32_i2c_t * pbus        = (bus_stm32_i2c_t*) ctx;
    memset(pbus, 0, sizeof(bus_stm32_i2c_t));
    return E_OK;
}

/// this driver will setup blocking I2C mode
int bus_stm32_i2c_open(bus_t *ctx, void *cfg)
{
    bus_stm32_i2c_t * pbus        = (bus_stm32_i2c_t*) ctx;
    bus_stm32_i2c_cfg_t * i2c_cfg = (bus_stm32_i2c_cfg_t*) cfg;

    pbus->addr = i2c_cfg->slave_addr << 1; // this is how stm32 driver recognizes slave i2c addr
    pbus->timeout = i2c_cfg->timeout;

    pbus->hi2c.Instance = I2C1;
    pbus->hi2c.Init.Timing = 0x00707CBB;
    pbus->hi2c.Init.OwnAddress1 = 0;
    pbus->hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    pbus->hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    pbus->hi2c.Init.OwnAddress2 = 0;
    pbus->hi2c.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    pbus->hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    pbus->hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&pbus->hi2c) != HAL_OK)
        return E_BUS_OPERATION;
    /** Configure Analogue filter
    */
    if (HAL_I2CEx_ConfigAnalogFilter(&pbus->hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
        return E_BUS_OPERATION;
    /** Configure Digital filter
    */
    if (HAL_I2CEx_ConfigDigitalFilter(&pbus->hi2c, 0) != HAL_OK)
        return E_BUS_OPERATION;
	return E_OK;
}

int bus_stm32_i2c_read_raw(bus_t *ctx, data_buffer_t *buffer)
{
    bus_stm32_i2c_t * pbus        = (bus_stm32_i2c_t*) ctx;
    if (HAL_I2C_Master_Receive(&pbus->hi2c, pbus->addr, buffer->data, buffer->data_len, pbus->timeout) != HAL_OK)
        return E_BUS_OPERATION;
	return E_OK;
}

/// 1 byte reg addr only
int bus_stm32_i2c_read(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer)
{
    error_t status;
    bus_stm32_i2c_t * pbus        = (bus_stm32_i2c_t*) ctx;
    reg_addr = reg_addr & 0xff;
    data_buffer_t addr_byte = {.data = (uint8_t *) &reg_addr, .data_len = 1};
    if ((status = bus_stm32_i2c_write_raw(ctx, &addr_byte)) != E_OK)
        return status;
    if ((status = bus_stm32_i2c_read_raw(ctx, buffer)) != E_OK)
        return status;
	return E_OK;
}

int bus_stm32_i2c_write_raw(bus_t *ctx, data_buffer_t *buffer)
{
    bus_stm32_i2c_t * pbus        = (bus_stm32_i2c_t*) ctx;
    if (HAL_I2C_Master_Transmit(&pbus->hi2c, pbus->addr, buffer->data, buffer->data_len, pbus->timeout) != HAL_OK)
        return E_BUS_OPERATION;
	return E_OK;
}

/// 1 byte addr only
int bus_stm32_i2c_write(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer)
{
    bus_stm32_i2c_t * pbus        = (bus_stm32_i2c_t*) ctx;
    error_t status;
    reg_addr = reg_addr & 0xff;
    data_buffer_t addr_byte = {.data = (uint8_t *) &reg_addr, .data_len = 1};
    if ((status = bus_stm32_i2c_write_raw(ctx, &addr_byte)) != E_OK)
        return status;
    if ((status = bus_stm32_i2c_write_raw(ctx, buffer)) != E_OK)
        return status;
	return E_OK;
}

int bus_stm32_i2c_close(bus_t *ctx)
{
    bus_stm32_i2c_t * pbus        = (bus_stm32_i2c_t*) ctx;
    if (HAL_I2C_DeInit(&pbus->hi2c) != HAL_OK)
        return E_BUS_OPERATION;
	return E_OK;
}
