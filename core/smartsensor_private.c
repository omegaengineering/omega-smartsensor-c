/*!********************************************************************************************
  @file     smartsensor_private.c

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
            Contains implementation of smartsensor api

***********************************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "registers_i2c.h"
#include "log.h"
#include "port.h"

#define INDEX_0     0

int get_register_instance_cnt(ss_register_t ss_register)
{
    const _register_t* reg;
    if ((reg = get_register_entry(ss_register)))
        return reg->nInstance;
    return 0;
}


//int sensor_init (sensor_t* sensor, sensor_init_t* init)
//{
//    memset(sensor, 0, sizeof(*sensor));
//    if (init->bus_type != SENSOR_BUS_I2C && init->bus_type != SENSOR_BUS_MODBUS)
//        return E_BUS_TYPE;  // not supported
//    sensor->bus_type = init->bus_type;
//    sensor->event_callback = init->event_callback;
//    if (sensor->event_callback)
//        sensor->event_callback_ctx = init->event_callback_ctx;
//    sensor->platform = init->platform;
//    // open with configuration
//    return ((port_t*)(sensor->platform))->init(&sensor->platform, init->platform_config);
//}


int sensor_open (sensor_t* sensor)
{
    int ret;
    port_t * p = sensor->platform;
    p->sensor = sensor;
    return p->init(p);
}

int sensor_close(sensor_t* sensor)
{
    int ret;
//    sensor->do_exit = 1;
    port_t * p = sensor->platform;
    return p->deinit(sensor->platform);
}


int sensor_heartbeat_enable (sensor_t* sensor, int period_ms)
{
    int ret;
//    if ((ret = port_heartbeat_start(sensor->platform, period_ms)))
//        return ret;
    return E_OK;
}

int sensor_heartbeat_disable(sensor_t* sensor)
{
    int ret;
//    if ((ret = port_heartbeat_stop(sensor->platform)))
//        return ret;
    return E_OK;
}


static void reverse_bytes(uint8_t* buffer, uint16_t buffer_sz)
{
    for (int i = 0; i < buffer_sz/2; i++)
    {
        uint8_t temp = buffer[i];
        buffer[i] = buffer[buffer_sz - i-1];
        buffer[buffer_sz - i-1] = temp;
    }
}

/*!*******************************************************************
static uint16_t    u16_SmartSensor_SetIndex(uint16_t u16_Register);

 @brief sets up indexed register based on selected u16_Register

 @param     u16_Register        Smart Sensor register to be accessed
 @returns   u16_Register        Adjusted register or error condition

 If u16_Register is < 0xff it is returned unmodified.
 if u16_Register is 0x100 .. 0x0fff it used to set the INDEX REGISTER and
 the correct ACCESS register is returned.

 If no error occurs the returned value will be in the range 0x0000 .. 0x00ff and
 may be used for subsequent read or write functions

 If u16_Register is > 0x0fff an E_REGISTER error is returned.

 Errors

 E_REGISTER     invalid register
 E_WRITE        Write request error during INDEX REGISTER write

*********************************************************************/


static int i2c_set_index(sensor_t* sensor, uint16_t u16_Register_in, uint16_t *u16_Register_out)
{
    int ret;
    port_t* p = sensor->platform;
    if (u16_Register_in >= 0x00 && u16_Register_in <= 0xff)
    {
        // single byte address
        *u16_Register_out = u16_Register_in & 0xffu;
        ret = E_OK;
    } else if (1) {
    //} else if (u16_Register_in >= R_ACCESS_FACTORY_START && u16_Register_in <= R_ACCESS_FACTORY_END) {
        uint8_t buffer[3];
        buffer[0] = R_REGISTER_ACCESS;
        buffer[1] = u16_Register_in >> 8u;
        buffer[2] = u16_Register_in;

        if ((ret = p->write(sensor->platform, buffer, 3)) == E_OK) {
            *u16_Register_out = ((u16_Register_in >> 10u) + R_ACCESS_FACTORY_INDEX);  //R_FACTORY_ACCESS
        }
    } else if (u16_Register_in >= R_ACCESS_BLOCK_START && u16_Register_in <= R_ACCESS_BLOCK_END) {
        uint8_t buffer[3];
        buffer[0] = R_REGISTER_ACCESS;
        buffer[1] = u16_Register_in >> 8u;
        buffer[2] = u16_Register_in;

        if ((ret = p->write(sensor->platform, buffer, 3)) == E_OK) {
            *u16_Register_out = ((u16_Register_in >> 10u) + R_ACCESS_BLOCK_INDEX);
        }
    } else if (u16_Register_in >= R_ACCESS_SENSOR_START && u16_Register_in <= R_ACCESS_SENSOR_END) {
        ret = E_INVALID_ADDR;
    } else if (u16_Register_in >= R_ACCESS_EXTENSION_START && u16_Register_in <= R_ACCESS_EXTENSION_END) {
        ret = E_INVALID_ADDR;
    } else {
        ret = E_INVALID_ADDR;
    }
    return ret;
}

static int sensor_bus_read(sensor_t* sensor, uint16_t reg_addr, uint8_t* buffer, uint16_t buffer_sz)
{
    int ret;
    uint8_t addr_buf[2];
    port_t* p = sensor->platform;

    addr_buf[0] = reg_addr & 0xffU;  // register is 1 byte only

    if (sensor->bus_type == SENSOR_BUS_I2C) {
        if ((ret = p->read(sensor->platform, addr_buf, 1)) != E_OK)
            return ret;
    }
    ret = p->read(sensor->platform, buffer, buffer_sz);
    if (ret != 0)
        printf("%d\n", ret);
    return ret;
}

static int sensor_bus_write(sensor_t* sensor, uint16_t reg_addr, uint8_t* buffer, uint16_t buffer_sz)
{
    int ret;
    port_t* p = sensor->platform;
    uint8_t temp[48];

    if (buffer_sz > sizeof(temp) - 1)
        return E_BUFFER_MEM_SIZE;

    // we have to write the data to the bus at one shot starting with reg addr
    temp[0] = reg_addr & 0xffU;
    memcpy(temp + 1, buffer, buffer_sz);

    ret = p->write(sensor->platform, temp, buffer_sz + 1);
    return ret;
}

int sensor_indexed_read(sensor_t* sensor, ss_register_t base_reg, uint8_t index, void* buffer, uint16_t buffer_sz)
{
    int ret;
    uint16_t reg_addr;
    const _register_t *reg;

    if (!(reg = get_register_entry(base_reg)))
        return E_INVALID_PARAM;
    if (buffer_sz < reg->size)
        return E_BUFFER_MEM_SIZE;
    if (index >= reg->nInstance)
        return E_INVALID_PARAM;

    port_ENTER_CRITICAL_SECTION();

    if (sensor->bus_type == SENSOR_BUS_I2C) {
        reg_addr = reg->i2c_addr + index * (reg->size + reg->offset);
        if ((ret = i2c_set_index(sensor, reg_addr, &reg_addr) != E_OK))
            goto ERROR;
    }
    else {
        reg_addr = reg->modbus_addr + (index * reg->size)/2;
    }

    // only read up to the actual data size
    ret = sensor_bus_read(sensor, reg_addr, buffer, reg->size);
    if (ret == E_OK && !(reg->access & BYTES))    // reverse I2C data (in MSB) to LSB format
        reverse_bytes(buffer, reg->size);

    port_EXIT_CRITICAL_SECTION();
    return ret;

ERROR:
    port_EXIT_CRITICAL_SECTION();
    return ret;
}

int sensor_read(sensor_t* sensor, ss_register_t ss_register, void* buffer, uint16_t buffer_sz)
{
    return sensor_indexed_read(sensor, ss_register, INDEX_0, buffer, buffer_sz);
}

static int handle_event_data_intr(sensor_t* sensor, api_event_t* event)
{
    int ret;
    interrupt_status_t intr_status;
    if ((ret = get_interrupt_status(sensor, &intr_status)) == E_OK) {
        if (sensor->data.sensor_attached) {
            // normal operation, attached and receiving interrupts
            *event = (api_event_t) intr_status;
        }
        else {
            system_status_t status;
            ret = get_system_status(sensor, &status);
            if (ret == E_OK && status.device_ready) // device ready cannot be false
            {
                sensor->data.stat_attach_counter++;
                sensor->data.sensor_attached = true;
                sensor->data.heartbeat_misses = 0;
                s_log("Probe attached\n");
                *event = API_EVENT_SENSOR_ATTACHED;
            }
        }
    }
    return ret;
}

static int handle_event_heartbeat(sensor_t* sensor, api_event_t* event)
{
    int ret;
    system_status_t status;
    if ((ret = get_system_status(sensor, &status) != E_OK)) {
        sensor->data.heartbeat_misses++;
        if (sensor->data.sensor_attached && sensor->data.heartbeat_misses > HEARTBEAT_MAX_MISS) {
            sensor->data.sensor_attached = false;
            s_log("Probe detached\n");
            *event = API_EVENT_SENSOR_DETACHED;
        }
    }
    return ret;
}

int sensor_poll_event(sensor_t* sensor, api_event_t* event)
{
    int ret;
    port_event_t port_event;
    api_event_t api_event = API_EVENT_CONTINUE;
    port_t* p = sensor->platform;

    ret = p->event_get(p, &port_event);
    if (ret == E_OK)
    {
        switch (port_event)
        {
            case EV_COMM_SEND_DONE:
                break;
            case EV_COMM_RECV_DONE:
                break;
            case EV_COMM_TIMEOUT:
                break;
            case EV_HEARTBEAT:
                ret = handle_event_heartbeat(sensor, &api_event);
                break;
            case EV_DAT_INTR:
                ret = handle_event_data_intr(sensor, &api_event);
                break;
            default:
                break;
        }
    }
    return ret;
}

//void* sensor_thread(void* args)
//{
//    if (!args)
//        return NULL;
//
//    sensor_t* sensor = args;
//
//    while (!sensor->do_exit)
//    {
//        sensor_poll_event(sensor);
//    }
//    return NULL;
//}

int sensor_indexed_write(sensor_t* sensor, ss_register_t base_register, uint8_t index, void* buffer, uint16_t buffer_sz)
{
    int ret = 0;
    uint16_t reg_addr;
    const _register_t *reg;

    if (!(reg = get_register_entry(base_register)))
        return E_INVALID_PARAM;
    if (buffer_sz < reg->size) //TODO
        return E_BUFFER_MEM_SIZE;
    if (index >= reg->nInstance)
        return E_INVALID_PARAM;

    port_ENTER_CRITICAL_SECTION();

    if (sensor->bus_type == SENSOR_BUS_I2C) {
        reg_addr = reg->i2c_addr + index * (reg->size + reg->offset);
        if ((ret = i2c_set_index(sensor, reg_addr, &reg_addr) != E_OK))
            goto ERROR;
    }
    else {
        reg_addr = reg->modbus_addr + (index * reg->size)/2;
    }

    // only write up to the actual data size
    if (ret == E_OK && !(reg->access & BYTES))    // reverse I2C data (in MSB) to LSB format
        reverse_bytes(buffer, reg->size);
    ret = sensor_bus_write(sensor, reg_addr, buffer, buffer_sz);

    port_EXIT_CRITICAL_SECTION();
    return ret;

ERROR:
    port_EXIT_CRITICAL_SECTION();
    return ret;
}

int sensor_write(sensor_t* sensor, ss_register_t ss_register, void* buffer, uint16_t buffer_sz)
{
    return sensor_indexed_write(sensor, ss_register, INDEX_0, buffer, buffer_sz);
}

