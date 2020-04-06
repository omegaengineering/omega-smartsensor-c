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
#include <assert.h>
#include "smartsensor_private.h"
#include "log.h"

static void run_user_callback(sensor_t* sensor, api_event_t event);



int get_max_instance(ss_register_t ss_register)
{
    const _register_t* reg = get_register_entry(ss_register);
    if (reg == NULL)
        return 0;
    return reg->nInstance;
}


int sensor_init (sensor_t* sensor, sensor_init_t* init)
{
    memset(sensor, 0, sizeof(*sensor));
    if (init->bus_type != SENSOR_BUS_I2C && init->bus_type != SENSOR_BUS_MODBUS)
        return E_BUS_TYPE;  // not supported
    sensor->bus_type = init->bus_type;
    sensor->event_callback = init->event_callback;
    if (sensor->event_callback)
        sensor->event_callback_ctx = init->event_callback_ctx;
    return E_OK;
}

int sensor_open (sensor_t* sensor, const port_cfg_t* config, uint16_t config_sz)
{
    int ret;
    ret = port_platform_init(&sensor->platform, config, config_sz, sensor);
    if (ret == E_OK)
        sensor->opened = 1;
    return ret;
}

int sensor_close(sensor_t* sensor)
{
    int ret;
    ret = port_platform_deinit(sensor->platform, sensor);
    sensor->opened = 0;
    return ret;
}


int sensor_heartbeat_enable (sensor_t* sensor, int period_ms)
{
    int ret;
    if ((ret = port_heartbeat_start(sensor->platform, period_ms)))
        return ret;
    return E_OK;
}

int sensor_heartbeat_disable(sensor_t* sensor)
{
    int ret;
    if ((ret = port_heartbeat_stop(sensor->platform)))
        return ret;
    return E_OK;
}


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
        if ((ret = port_comm_write(sensor->platform, data.data, data.data_len) != E_OK))
            return ret;
        *u16_Register_out = ((u16_Register_in >> 10) + R_FACTORY_ACCESS);
    }
    return (E_OK);
}

static int sensor_bus_read(sensor_t* sensor, uint16_t reg_addr, data_buffer_t * buffer)
{
    int ret;
    uint8_t addr_buf[2];

    addr_buf[0] = reg_addr & 0xffU;  // register is 1 byte only
    data_buffer_t temp = {.data = addr_buf, .data_len = 1};
#if 1
    if ((ret = port_comm_write(sensor->platform, temp.data, temp.data_len)) != E_OK)
        return ret;
    ret = port_comm_read(sensor->platform, buffer->data, buffer->data_len);

#else
    //write register addr
    ret = op_push(OP_SEND, temp.data, temp.data_len);
    assert(ret == 0);
    // read data into buffer
    ret = op_push(OP_RECV, buffer->data, buffer->data_len);
    assert(ret == 0);
#endif
    return ret;
}

static int sensor_bus_write(sensor_t* sensor, uint16_t reg_addr, data_buffer_t * buffer)
{
    int ret;

    uint8_t temp[48];
    data_buffer_t temp_buffer = {.data = temp, .data_len = buffer->data_len + 1};
    if (buffer->data_len > sizeof(temp) - 1)
        return E_BUFFER_MEM_SIZE;

    // we have to write the data to the bus at one shot starting with reg addr
    temp[0] = reg_addr & 0xffU;
    memcpy(temp + 1, buffer->data, buffer->data_len);

    ret = port_comm_write(sensor->platform, temp_buffer.data, temp_buffer.data_len);
    return ret;
}

int sensor_read(sensor_t* sensor, ss_register_t ss_register, data_buffer_t *data_buffer)
{
    int ret;
    uint16_t reg_addr;
    const _register_t *reg;

    if (!(reg = get_register_entry(ss_register)))
        return E_INVALID_PARAM;
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
    ret = sensor_bus_read(sensor, reg_addr, &buf);
    if (ret == E_OK && !(reg->access & BYTES))    // reverse I2C data (in MSB) to LSB format
        reverse_bytes(&buf);

    port_EXIT_CRITICAL_SECTION();
    return ret;

ERROR:
    port_EXIT_CRITICAL_SECTION();
    return ret;
}

static int handle_event_data_intr(sensor_t* sensor)
{
    int ret;
    interrupt_status_t intr_status;
    if ((ret = get_interrupt_status(sensor, &intr_status)) == E_OK) {
        if (sensor->data.sensor_attached) {
            // normal operation, attached and receiving interrupts
            run_user_callback(sensor, (api_event_t) intr_status);
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
                run_user_callback(sensor, (api_event_t) API_EVENT_SENSOR_ATTACHED);
            }
        }
    }
    return ret;
}

static int handle_event_heartbeat(sensor_t* sensor)
{
    int ret;
    system_status_t status;
    if ((ret = get_system_status(sensor, &status) != E_OK)) {
        sensor->data.heartbeat_misses++;
        if (sensor->data.sensor_attached && sensor->data.heartbeat_misses > HEARTBEAT_MAX_MISS) {
            sensor->data.sensor_attached = false;
            s_log("Probe detached\n");
            run_user_callback(sensor, API_EVENT_SENSOR_DETACHED);
        }
    }
    return ret;
}

int sensor_poll(sensor_t* sensor)
{
    int ret;
    port_event_t port_event;
    api_event_t api_event = API_EVENT_CONTINUE;
    ret = port_event_get(sensor->platform, &port_event);
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
                ret = handle_event_heartbeat(sensor);
                break;
            case EV_DAT_INTR:
                ret = handle_event_data_intr(sensor);
                break;
            default:
                break;
        }
    }
    return ret;
}

int sensor_indexed_read(sensor_t* sensor, ss_register_t ss_register, uint8_t index, data_buffer_t * buffer)
{
    ss_register_t indexed_register = ss_register + index;
    if ( index >= get_max_instance(ss_register))
        return E_INVALID_PARAM;
    return sensor_read(sensor, indexed_register, buffer);
}

int sensor_write(sensor_t* sensor, ss_register_t ss_register, data_buffer_t *data_buffer)
{
    int ret;
    uint16_t reg_addr;
    const _register_t *reg;

    if (!(reg = get_register_entry(ss_register)))
        return E_INVALID_PARAM;

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
    ret = sensor_bus_write(sensor, reg_addr, &buf);

    port_EXIT_CRITICAL_SECTION();
    return ret;

ERROR:
    port_EXIT_CRITICAL_SECTION();
    return ret;
}

int sensor_indexed_write(sensor_t* sensor, ss_register_t ss_register, uint8_t index, data_buffer_t * buffer)
{
    ss_register_t indexed_register = ss_register + index;
    if ( index >= get_max_instance(ss_register))
        return E_INVALID_PARAM;
    return sensor_write(sensor, indexed_register, buffer);
}

int sensor_is_opened(sensor_t* sensor)
{
    return sensor->opened;
}

static void run_user_callback(sensor_t* sensor, api_event_t event)
{
    if (sensor && sensor->event_callback)
        sensor->event_callback(event, sensor->event_callback_ctx);
}

//void miss_heartbeat(int instance)
//{
//    sensor_t* sensor = get_sensor(instance);
//    if (sensor) {
//        sensor->data.heartbeat_misses++;
//        if (sensor->data.heartbeat_misses > HEARTBEAT_MAX_MISS)
//        {
//            sensor->data.sensor_attached = false;
//            run_user_callback(instance, API_EVENT_SENSOR_DETACHED);
//            s19_log_info("Probe detached\n");
//        }
//    }
//}


//static void resolve_intr_event(int instance)
//{
//    int ret;
//    interrupt_status_t intr_status;
//    ret = get_interrupt_status(instance, &intr_status);
//    if (ret == E_OK)
//    {
//        run_user_callback(instance, (api_event_t) intr_status);
//    }
//    else
//    {
//        miss_heartbeat(instance);
//    }
//}

//static int do_probe_attach(sensor_t * sensor)
//{
//    int ret;
//    sensor->data.stat_attach_counter++;
//    // probe init
//    s19_log_info("Probe attached\n");
//    // okay, new probe attached
//    wait_for_device_ready(sensor, 1000);
//    sensor->data.sensor_attached = true;
//    sensor->data.heartbeat_misses = 0;
////    run_user_callback(sensor->sensor_id, API_EVENT_SENSOR_ATTACHED);
//}

/// level 2 handling
//static void event_handler_low_level(int instance, event_type_t intr)
//{
//    int ret;
//    sensor_t* sensor = get_sensor(instance);
//    if (!sensor)
//        return;
//    if (intr == SENSOR_EVT_HARD_INTR) {
//        if (sensor->data.sensor_attached)
//        {   // normal operation, attached and receiving interrupts
//            resolve_intr_event(instance);
//        }
//        else
//        {
//            system_status_t status;
//            ret = get_system_status(instance, &status);
//            if (ret == E_OK && status.device_ready) // device ready cannot be false
//            {
//                do_probe_attach(sensor);
//            }
//            else
//                s19_log_dbg("System Status %d %d\n", ret, status.device_ready);
//        }
//    }
//    else if (intr == SENSOR_EVT_TIMEOUT)
//    {
//        if (sensor->data.sensor_attached)
//        {   // was attached, then check if still attached
//            system_status_t status;
//            ret = get_system_status(instance, &status);
//            if (ret != E_OK || !status.device_ready) {  // looks like no response
//                miss_heartbeat(sensor->sensor_id);
//            }
//        } else {
//            // wasn't attached, keep checking for device ready
//            system_status_t status;
//            ret = get_system_status(instance, &status);
//            if (ret == E_OK && status.device_ready) // device ready cannot be false
//            {
//                do_probe_attach(sensor);
//            }
//        }
//    }
//}


/// level 1 signal
//void sensor_interrupt_triggered(int instance)
//{
////    if (ctx->ready)
//    {
////        s19_log_dbg("Hard interrupt\n");
//        event_handler_low_level(instance, SENSOR_EVT_HARD_INTR);
//    }
//}
//
/// level 1 signal
//void sensor_timeout_triggered(void* x)
//{
////    s19_log_dbg("Heartbeat timeout\n");
////    event_handler_low_level(instance, SENSOR_EVT_TIMEOUT);
//}

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
