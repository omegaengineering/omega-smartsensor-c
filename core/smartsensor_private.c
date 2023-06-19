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

int sensor_open (sensor_t* sensor)
{
    int ret;
    port_t * p = sensor->platform;
    if (!p->init || !p->deinit
        || !p->read || !p->write
        || !p->bus_type || !p->delay
        || !p->event_get) {
        return E_PORT_UNAVAILABLE;
    }
    p->sensor = sensor;
    return p->init(p);
}

int sensor_close(sensor_t* sensor)
{
    int ret;
    port_t * p = sensor->platform;
    return p->deinit(sensor->platform);
}


int sensor_heartbeat_enable (sensor_t* sensor, int period_ms)
{
    port_t * p = sensor->platform;
    return p->timer_start ? p->timer_start(p, period_ms) : E_PORT_UNAVAILABLE;
}

int sensor_heartbeat_disable(sensor_t* sensor)
{
    port_t * p = sensor->platform;
    return p->timer_stop ? p->timer_stop(p) : E_PORT_UNAVAILABLE;
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
    }
    else if(u16_Register_in > 0xff && u16_Register_in < 0x8000)
    {
		uint8_t buffer[2];
		buffer[0] = u16_Register_in >> 8u;
		buffer[1] = u16_Register_in & 0xff;

		if (((ret = p->write(p, R_REGISTER_ACCESS, &buffer[0], 2)) == E_OK) )
		{
			*u16_Register_out = ((u16_Register_in >> 10u) + R_ACCESS_FACTORY_INDEX);
		}
    }
	else
	{
		ret = E_INVALID_ADDR;
	}
    return ret;
}

int sensor_indexed_read(sensor_t* sensor, ss_register_t base_reg, uint8_t index, void* buffer, uint16_t buffer_sz)
{
    int ret;
    uint16_t reg_addr;
    const _register_t *reg;
    port_t* p = sensor->platform;

    if (!(reg = get_register_entry(base_reg)))
        return E_INVALID_PARAM;
    if (!(reg->access & BYTES) && buffer_sz != reg->size
        || (reg->access & BYTES) && buffer_sz > reg->size)
        return E_BUFFER_MEM_SIZE;
    if (index >= reg->nInstance)
        return E_INVALID_PARAM;

    port_ENTER_CRITICAL_SECTION();

    if (p->bus_type(p) == SENSOR_BUS_I2C)
    {
        reg_addr = reg->i2c_addr + index * (reg->size + reg->offset);
        if (reg_addr >= 0x00 && reg_addr <= 0xff)
        {
            // only read up to the actual data size
            ret = p->read(p, reg_addr, buffer, reg->size);
        }
        else if(reg_addr > 0xff && reg_addr < 0x8000)
        {//this is an indirect register read
    		uint8_t indirect_address[2];
    		indirect_address[0] = reg_addr >> 8u;
    		indirect_address[1] = reg_addr & 0xff;

    		reg_addr = ((reg_addr >> 10u) + R_ACCESS_FACTORY_INDEX);
    		ret = p->indirect_read(p, reg_addr, buffer, reg->size, R_REGISTER_ACCESS, &indirect_address[0], 2);

        }
    	else
    	{
    		ret = E_INVALID_ADDR;
    	}
    }
    else
    {
        reg_addr = reg->modbus_addr + (index * (reg->size + reg->offset))/2;
        // only read up to the actual data size
        ret = p->read(p, reg_addr, buffer, reg->size);
    }


    if(ret == E_OK)
    {
    	if(reg->access & STR)
    	{
    		char * stringbuf = buffer;
    		if((stringbuf[0]== 0) && (stringbuf[reg->size-1] !=0))
    		{
    			reverse_bytes(buffer, buffer_sz);
    		}
    	}
    	else if (!(reg->access & BYTES))
    	{// reverse I2C data (in MSB) to LSB format
    		reverse_bytes(buffer, buffer_sz);
    	}
    }


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
            *event = API_EVENT_SENSOR_DETACHED;
            ret = 0; // event was handled
        }
    }
    return ret;
}

int sensor_poll_event(sensor_t* sensor, api_event_t* event)
{
    int ret;
    port_event_t port_event;
    *event = API_EVENT_CONTINUE;
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
                ret = handle_event_heartbeat(sensor, event);
                break;
            case EV_DAT_INTR:
                ret = handle_event_data_intr(sensor, event);
                break;
            default:
                break;
        }
    }
    return ret;
}

int sensor_indexed_write(sensor_t* sensor, ss_register_t base_register, uint8_t index, void* buffer, uint16_t buffer_sz)
{
    int ret = 0;
    uint16_t reg_addr;
    const _register_t *reg;
    port_t* p = sensor->platform;

    if (!(reg = get_register_entry(base_register)))
        return E_INVALID_PARAM;
    if (!(reg->access & BYTES) && buffer_sz != reg->size
        || (reg->access & BYTES) && buffer_sz > reg->size)
        return E_BUFFER_MEM_SIZE;
    if (index >= reg->nInstance)
        return E_INVALID_PARAM;

    if (!(reg->access & BYTES))    // reverse I2C data (in MSB) to LSB format
        reverse_bytes(buffer, buffer_sz);

    port_ENTER_CRITICAL_SECTION();

    if (p->bus_type(p) == SENSOR_BUS_I2C) {
        reg_addr = reg->i2c_addr + index * (reg->size + reg->offset);
        if (reg_addr >= 0x00 && reg_addr <= 0xff)
        {
            // only write up to the actual data size
            ret = p->write(p, reg_addr, buffer, buffer_sz);
        }
        else if(reg_addr > 0xff && reg_addr < 0x8000)
        {//this is an indirect register read
    		uint8_t indirect_address[2];
    		indirect_address[0] = reg_addr >> 8u;
    		indirect_address[1] = reg_addr & 0xff;

    		reg_addr = ((reg_addr >> 10u) + R_ACCESS_FACTORY_INDEX);
    		ret = p->indirect_write(p, reg_addr, buffer, buffer_sz, R_REGISTER_ACCESS, &indirect_address[0], 2);

        }
    }
    else {
        reg_addr = reg->modbus_addr + (index * (reg->size + reg->offset))/2;
        // only write up to the actual data size
        ret = p->write(p, reg_addr, buffer, buffer_sz);
    }
    //read the system status to hold the i2c line so the write cane get to the probe
    uint8_t sys_stat[2];
    p->read(p, R_SYSTEM_STATUS, &sys_stat[0], 2);

    port_EXIT_CRITICAL_SECTION();
    return ret;

}

int sensor_write(sensor_t* sensor, ss_register_t ss_register, void* buffer, uint16_t buffer_sz)
{
    return sensor_indexed_write(sensor, ss_register, INDEX_0, buffer, buffer_sz);
}

const char* sensor_strerror (sensor_t* sensor, int err)
{
    port_t* p = sensor->platform;
    int sensor_err = GET_SENSOR_ERR(err);
    int port_err = GET_PORT_ERR(err);
    switch (sensor_err)
    {
        case E_UNAVAILABLE:         return "E_UNAVAILABLE";
        case E_OK:                  return "E_OK";
        case E_BUS_OPERATION:       return "E_BUS_OPERATION";
        case E_BUS_TYPE:            return "E_BUS_TYPE";
        case E_BUFFER_MEM_SIZE:     return "E_BUFFER_MEM_SIZE";
        case E_INVALID_PARAM:       return "E_INVALID_PARAM";
        case E_INVALID_ADDR:        return "E_INVALID_ADDR";
        case E_NO_MEM:              return "E_NO_MEM";
        case E_NOT_SUPPORTED:       return "E_NOT_SUPPORTED";
        case E_TRY_AGAIN:           return "E_TRY_AGAIN";
        case E_FULL:                return "E_FULL";
        case E_EMPTY:               return "E_EMPTY";
        case E_CONTINUE:            return "E_CONTINUE";
        case E_PORT_UNAVAILABLE:    return "E_PORT_UNAVAILABLE";
        case E_PORT_ERR:            return p->strerror ? p->strerror(port_err): "E_PORT_ERR";
        default:                    return "Unknown";
    }
}
