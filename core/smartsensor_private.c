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
#include <stdio.h>
#include "smartsensor_private.h"
#include "port/port.h"

static sensor_t sensors[SMARTSENSOR_MAX];


static void run_user_callback(int instance, api_event_t event);



int get_max_instance(ss_register_t ss_register)
{
    const _register_t* reg = get_register_entry(ss_register);
    if (reg == NULL)
        return 0;
    return reg->nInstance;
}

//int sensor_new(sensor_t **ctx, const sensor_init_t *init)
//{
//    int ret;
//    sensor_t *new_sensor = s19_mem_malloc(sizeof(sensor_t));
//    if (new_sensor == NULL)
//        return E_NO_MEM;
//
//    memset(new_sensor, 0, sizeof(sensor_t));
//    new_sensor->bus_type = init->bus_type;
//    new_sensor->gpio_pin = init->interrupt_pin;
//    new_sensor->event_callback = init->event_callback;
//    new_sensor->event_callback_ctx = init->event_callback_ctx;
//    new_sensor->heartbeat_period = init->heartbeat_period;
//
//    if (new_sensor->bus_type == SENSOR_BUS_I2C) {
//        if ((ret = port_bus_i2c_new(&new_sensor->bus)) != E_OK)
//            goto ERROR;
//        new_sensor->bus_cfg = (void*) port_bus_i2c_cfg;
//    }
//    else if (new_sensor->bus_type == SENSOR_BUS_MODBUS) {
//        if ((ret = port_bus_modbus_new(&new_sensor->bus)) != E_OK)
//            goto ERROR;
//        new_sensor->bus_cfg = (void*) port_bus_modbus_cfg;
//    }
//    else {
//        ret = E_NOT_SUPPORTED;
//        goto ERROR;
//    }
//    strcpy(new_sensor->bus->bus_id, init->bus_id);  // do this after bus create !
//
//    if ((ret = s19_mutex_create(&new_sensor->bus_lock)) != E_OK)
//        goto ERROR;
//    if ((ret = s19_mutex_create(&new_sensor->data_lock)) != E_OK)
//        goto ERROR;
//
//#ifdef HEARTBEAT
//    if (new_sensor->heartbeat_period)
//        if ((ret = s19_timer_create(&new_sensor->timer) != E_OK))
//            goto ERROR;
//#endif
//    *ctx = new_sensor;
//    return E_OK;
//
//ERROR:
//    if (new_sensor) {
//        if (new_sensor->bus)
//            bus_free(new_sensor->bus);
//#ifdef HEARTBEAT
//        if (new_sensor->timer)
//            s19_timer_destroy(new_sensor->timer);
//#endif
//        if (new_sensor->bus_lock)
//            s19_mutex_destroy(new_sensor->bus_lock);
//        if (new_sensor->data_lock)
//            s19_mutex_destroy(new_sensor->data_lock);
//        s19_mem_free(new_sensor);
//    }
//    *ctx = NULL;
//    return ret;
//}

static sensor_t* get_sensor(int instance)
{
    if (instance < SMARTSENSOR_MAX)
        return &sensors[instance];
    else
        return NULL;
}

int sensor_init (int instance, const sensor_init_t *init)
{
    int ret;
    sensor_t* sensor = get_sensor(instance);
    if (sensor) {
        if (sensor->init)   // already taken
            return E_UNAVAILABLE;
        if (init->bus_type != SENSOR_BUS_I2C && init->bus_type != SENSOR_BUS_MODBUS)
            return E_API_ERROR;
        sensor->sensor_id = instance;
        sensor->bus_id = instance;
        sensor->bus_type = init->bus_type;
        sensor->event_callback = init->event_callback;
        sensor->event_callback_ctx = init->event_callback_ctx;
        if (port_platform_init(instance) == 0) {
            sensor->init = true;
            return E_OK;
        }
    }
    return E_UNAVAILABLE;
}

int sensor_deinit(int instance)
{
    sensor_t* sensor = get_sensor(instance);
    if (sensor) {
        if (sensor->init) {
            port_platform_deinit(instance);
            sensor->init = false;
        }
    }
    return E_OK;
}

int sensor_heartbeat_enable (int instance, int period)
{
    int ret;
    if ((ret = port_timer_register(instance, period, sensor_timeout_triggered)))
        return ret;
    return E_OK;
}

int sensor_heartbeat_disable(int instance)
{
    int ret;
    if ((ret = port_timer_deregister(instance, sensor_timeout_triggered)))
        return ret;
    return E_OK;
}

//int sensor_open(int instance)
//{
//    int ret;
//    if (ctx == NULL)
//        return E_INVALID_PARAM;
//    if ((ret = bus_open(ctx->bus, ctx->bus_cfg)) != E_OK)
//        return ret;
//
//    if (ctx->bus_type == SENSOR_BUS_I2C)
//    {
//        if ((ret = port_intr_init(ctx)) != E_OK)
//            return ret;
//        if ((ret = port_platform_init(ctx)) != E_OK)
//            return ret;
//    }
//#ifdef HEARTBEAT
//    if (ctx->heartbeat_period)
//    {
//        if ((ret = s19_timer_entry_add(ctx->timer, &ctx->heartbeat) != E_OK))
//            return ret;
//        if ((ret = s19_timer_entry_set_period(ctx->heartbeat, ctx->heartbeat_period)) != E_OK)
//            return ret;
//        if ((ret = s19_timer_entry_set_ctx(ctx->heartbeat, ctx) != E_OK))
//            return ret;
//        if ((ret = s19_timer_entry_set_callback(ctx->heartbeat, timeout_triggered)) != E_OK)
//            return ret;
//    }
//#endif
//    ctx->ready = true;
//    return E_OK;
//}

static void reverse_bytes(data_buffer_t * data_buffer)
{
    for (int i = 0; i < data_buffer->data_len/2; i++)
    {
        uint8_t temp = data_buffer->data[i];
        data_buffer->data[i] = data_buffer->data[data_buffer->data_len - i-1];
        data_buffer->data[data_buffer->data_len - i-1] = temp;
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

#define R_ACCESS_REGISTER           0x0030
#define R_FACTORY_ACCESS            0x0032
static int i2c_set_index(sensor_t* sensor, uint16_t u16_Register_in, uint16_t *u16_Register_out)
{
    int ret;
    if (u16_Register_in > 0x0fff)
        return E_INVALID_PARAM;

    *u16_Register_out = u16_Register_in & 0xff;
    if (u16_Register_in > 0xff)
    {
        uint8_t buffer[3];
        buffer[0] = R_ACCESS_REGISTER;
        buffer[1] = u16_Register_in >> 8;
        buffer[2] = u16_Register_in;

        data_buffer_t data = {.data = buffer, .data_len = 3};
        if ((ret = port_bus_write(sensor->bus_id, &data) != E_OK))
            return ret;
        *u16_Register_out = ((u16_Register_in >> 10) + R_FACTORY_ACCESS);
    }
    return (E_OK);
}

static int sensor_bus_read(int bus_id, uint16_t reg_addr, data_buffer_t * buffer)
{
    int ret;
    uint8_t addr_buf[2];

    addr_buf[0] = reg_addr & 0xffU;  // register is 1 byte only
    data_buffer_t temp = {.data = addr_buf, .data_len = 1};
    if ((ret = port_bus_write(bus_id, &temp)) != E_OK)
        return ret;
    ret = port_bus_read(bus_id, buffer);
    return ret;
}

static int sensor_bus_write(int bus_id, uint16_t reg_addr, data_buffer_t * buffer)
{
    int ret;

    uint8_t temp[48];
    data_buffer_t temp_buffer = {.data = temp, .data_len = buffer->data_len + 1};
    if (buffer->data_len > sizeof(temp) - 1)
        return E_BUFFER_MEM_SIZE;

    // we have to write the data to the bus at one shot starting with reg addr
    temp[0] = reg_addr & 0xffU;
    memcpy(temp + 1, buffer->data, buffer->data_len);

    ret = port_bus_write(bus_id, &temp_buffer);
    return ret;
}

int sensor_read(int instance, ss_register_t ss_register, data_buffer_t *data_buffer)
{
    int ret;
    uint16_t reg_addr;
    const _register_t *reg;
    sensor_t* sensor;

    if (!(reg = get_register_entry(ss_register)))
        return E_INVALID_PARAM;
    if (!(sensor = get_sensor(instance)))
        return E_UNAVAILABLE;
    if (data_buffer->data_len < reg->size)
        return E_BUFFER_MEM_SIZE;

    port_ENTER_CRITICAL_SECTION();

    reg_addr = reg->modbus_addr;    // assume modbus
    if (sensor->bus_type == SENSOR_BUS_I2C) {
        reg_addr = reg->i2c_addr;
        if ((ret = i2c_set_index(sensor, reg_addr, &reg_addr) != E_OK))
            goto ERROR;
    }

    // only read up to the actual data size
    data_buffer_t buf = {data_buffer->data, reg->size};
    ret = sensor_bus_read(sensor->bus_id, reg_addr, &buf);
    if (ret == E_OK && !(reg->access & BYTES))    // reverse I2C data (in MSB) to LSB format
        reverse_bytes(&buf);

    port_EXIT_CRITICAL_SECTION();
    return ret;

ERROR:
    port_EXIT_CRITICAL_SECTION();
    return ret;
}

int sensor_indexed_read(int instance, ss_register_t ss_register, uint8_t index, data_buffer_t * buffer)
{
    ss_register_t indexed_register = ss_register + index;
    if ( index >= get_max_instance(ss_register))
        return E_INVALID_PARAM;
    return sensor_read(instance, indexed_register, buffer);
}

int sensor_write(int instance, ss_register_t ss_register, data_buffer_t *data_buffer)
{
    int ret;
    uint16_t reg_addr;
    sensor_t* sensor;
        const _register_t *reg;

    if (!(reg = get_register_entry(ss_register)))
        return E_INVALID_PARAM;
    if (!(sensor = get_sensor(instance)))
        return E_UNAVAILABLE;
    if (data_buffer->data_len < reg->size)
        return E_BUFFER_MEM_SIZE;

    port_ENTER_CRITICAL_SECTION();

    reg_addr = reg->modbus_addr;    // assume modbus
    if (sensor->bus_type == SENSOR_BUS_I2C) {
        reg_addr = reg->i2c_addr;
        if ((ret = i2c_set_index(sensor, reg_addr, &reg_addr) != E_OK))
            goto ERROR;
    }

    // only write up to the actual data size
    data_buffer_t buf = {data_buffer->data, reg->size};
    if (ret == E_OK && !(reg->access & BYTES))    // reverse I2C data (in MSB) to LSB format
        reverse_bytes(&buf);
    ret = sensor_bus_write(sensor->bus_id, reg_addr, &buf);

    port_EXIT_CRITICAL_SECTION();
    return ret;

ERROR:
    port_EXIT_CRITICAL_SECTION();
    return ret;
}

int sensor_indexed_write(int instance, ss_register_t ss_register, uint8_t index, data_buffer_t * buffer)
{
    ss_register_t indexed_register = ss_register + index;
    if ( index >= get_max_instance(ss_register))
        return E_INVALID_PARAM;
    return sensor_write(instance, indexed_register, buffer);
}

static void run_user_callback(int instance, api_event_t event)
{
    sensor_t* sensor = get_sensor(instance);
    if (sensor && sensor->event_callback)
        sensor->event_callback(event, sensor->event_callback_ctx);
}

void miss_heartbeat(int instance)
{
    sensor_t* sensor = get_sensor(instance);
    if (sensor) {
        sensor->data.heartbeat_misses++;
        if (sensor->data.heartbeat_misses > HEARTBEAT_MAX_MISS)
        {
            sensor->data.sensor_attached = false;
            run_user_callback(instance, API_EVENT_SENSOR_DETACHED);
            s19_log_info("Probe detached\n");
        }
    }
}


static void resolve_intr_event(int instance)
{
    int ret;
    interrupt_status_t intr_status;
    ret = get_interrupt_status(instance, &intr_status);
    if (ret == E_OK)
    {
        run_user_callback(instance, (api_event_t) intr_status);
    }
    else
    {
        miss_heartbeat(instance);
    }
}

static void do_probe_attach(sensor_t * sensor)
{
    int ret;
    sensor->data.stat_attach_counter++;
    // probe init
    s19_log_info("Probe attached\n");
    // okay, new probe attached
    wait_for_device_ready(sensor->sensor_id, 1000);
    sensor->data.sensor_attached = true;
    sensor->data.heartbeat_misses = 0;
    run_user_callback(sensor->sensor_id, API_EVENT_SENSOR_ATTACHED);
}

/// level 2 handling
static void event_handler_low_level(int instance, event_type_t intr)
{
    int ret;
    sensor_t* sensor = get_sensor(instance);
    if (!sensor)
        return;
    if (intr == SENSOR_EVT_HARD_INTR) {
        if (sensor->data.sensor_attached)
        {   // normal operation, attached and receiving interrupts
            resolve_intr_event(instance);
        }
        else
        {
            system_status_t status;
            ret = get_system_status(instance, &status);
            if (ret == E_OK && status.device_ready) // device ready cannot be false
            {
                do_probe_attach(sensor);
            }
            else
                s19_log_dbg("System Status %d %d\n", ret, status.device_ready);
        }
    }
    else if (intr == SENSOR_EVT_TIMEOUT)
    {
        if (sensor->data.sensor_attached)
        {   // was attached, then check if still attached
            system_status_t status;
            ret = get_system_status(instance, &status);
            if (ret != E_OK || !status.device_ready) {  // looks like no response
                miss_heartbeat(sensor->sensor_id);
            }
        } else {
            // wasn't attached, keep checking for device ready
            system_status_t status;
            ret = get_system_status(instance, &status);
            if (ret == E_OK && status.device_ready) // device ready cannot be false
            {
                do_probe_attach(sensor);
            }
        }
    }
}


/// level 1 signal
void sensor_interrupt_triggered(int instance)
{
//    if (ctx->ready)
    {
//        s19_log_dbg("Hard interrupt\n");
        event_handler_low_level(instance, SENSOR_EVT_HARD_INTR);
    }
}

/// level 1 signal
void sensor_timeout_triggered(int instance)
{
//    s19_log_dbg("Heartbeat timeout\n");
    event_handler_low_level(instance, SENSOR_EVT_TIMEOUT);
}

#if ENABLE_UNIT_TEST
int get_def_table_size()
{
    return sizeof(_def);
}

_register_t* get_def_table()
{
    return (_register_t*) &_def[0];
}
#endif
