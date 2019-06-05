/*!********************************************************************************************
  @file     bus_ti_i2c.h

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
#ifndef OMEGA_SMARTSENSOR_C_BUS_TI_I2C_H
#define OMEGA_SMARTSENSOR_C_BUS_TI_I2C_H

#include <ti/drivers/I2C.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/i2c/I2CCC26XX.h>       // needed to support I2C SCL/SDA Swap
#include "common/common.h"
#include "hw/bus.h"

#define I2C_INSTANCE_CNT    1
#define I2C_BUFFER_SIZE     64

typedef struct {
    uint8_t         periph_idx;
    I2C_BitRate     bit_rate;
    uint8_t         sda_pin;
    uint8_t         clk_pin;
    uint8_t         slave_addr;
} bus_ti_i2c_cfg_t;

int bus_ti_i2c_new(bus_t **ctx);
int bus_ti_i2c_free(bus_t *ctx);
int bus_ti_i2c_open(bus_t *ctx, void *cfg);
int bus_ti_i2c_read_raw(bus_t *ctx, data_buffer_t *buffer);
int bus_ti_i2c_read(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer);
int bus_ti_i2c_write_raw(bus_t *ctx, data_buffer_t *buffer);
int bus_ti_i2c_write(bus_t *ctx, uint16_t reg_addr, data_buffer_t *buffer);
int bus_ti_i2c_close(bus_t *ctx);

#endif //OMEGA_SMARTSENSOR_C_BUS_TI_I2C_H
