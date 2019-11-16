/*!********************************************************************************************
  @file     smartsensor.c

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
            Contains implementation for the public interface of smartsensor api.

***********************************************************************************************/

#include <string.h>
#include "smartsensor_private.h"


int get_sensor_reading(sensor_t *ctx, int sensor_num, const float *reading)
{
    data_buffer_t buffer = {.data = (uint8_t *) reading, .data_len = sizeof(reading)};
    return sensor_indexed_read(ctx, SENSOR_0_DATA, sensor_num, &buffer);
}

int get_sensor_gain(sensor_t *ctx, int sensor_num, const float *gain)
{
    data_buffer_t buffer = {.data = (uint8_t *) gain, .data_len = sizeof(gain)};
    return sensor_indexed_read(ctx, SENSOR_0_GAIN, sensor_num, &buffer);
}

int get_sensor_offset(sensor_t *ctx, int sensor_num, const float *gain)
{
    data_buffer_t buffer = {.data = (uint8_t *) gain, .data_len = sizeof(gain)};
    return sensor_indexed_read(ctx, SENSOR_0_OFFSET, sensor_num, &buffer);
}

int get_device_name(sensor_t *ctx, device_name_t name)
{
    data_buffer_t buffer = {.data = (uint8_t *) name, .data_len = sizeof(device_name_t)-1};
    memset(name, 0, sizeof(device_name_t));
    return sensor_read(ctx, DEVICE_NAME, &buffer);
}

int get_sensor_unit(sensor_t *ctx, int sensor_num, sensor_unit_t unit)
{
    data_buffer_t buffer = {.data = (uint8_t *) unit, .data_len = sizeof(sensor_unit_t)-1};
    memset(unit, 0, sizeof(sensor_unit_t));
    return sensor_indexed_read(ctx, SENSOR_0_UNIT, sensor_num, &buffer);
}

int get_sensor_descriptor(sensor_t *ctx, int sensor_num, sensor_descriptor_t *descriptor)
{
    data_buffer_t buffer = {.data = (uint8_t *) descriptor, .data_len = sizeof(sensor_descriptor_t)};
    return sensor_indexed_read(ctx, SENSOR_0_DESCRIPTOR, sensor_num, &buffer);
}

int get_io_count(sensor_t *ctx, io_count_t *io_count)
{
    data_buffer_t buffer = {.data = (uint8_t *) io_count, .data_len = sizeof(io_count_t)};
    return sensor_read(ctx, NUMBER_OF_SENSORS, &buffer);
}

int get_operating_stat(sensor_t *ctx, operating_stat_t *stat)
{
    data_buffer_t buffer = {.data = (uint8_t *) stat, .data_len = sizeof(operating_stat_t)};
    return sensor_read(ctx, OPERATING_TEMP, &buffer);
}

int get_sensor_type(sensor_t *ctx, int sensor_num, measurement_type_t *sensor_type)
{
    sensor_descriptor_t descriptor;
    int ret = get_sensor_descriptor(ctx, sensor_num, &descriptor);
    if (ret == E_OK)
        *sensor_type = descriptor.e_Measurement_Type;
    return ret;
}

int get_system_status(sensor_t *ctx, system_status_t *status)
{
    data_buffer_t buffer = {.data = (uint8_t *) status, .data_len = sizeof(system_status_t)};
    return sensor_read(ctx, SYSTEM_STATUS, &buffer);
}

int get_interrupt_status(sensor_t *ctx, interrupt_status_t *status)
{
    uint16_t interrupt_status;
    data_buffer_t buffer = {.data = (uint8_t *) &interrupt_status, .data_len = sizeof(interrupt_status)};
    int ret = sensor_read(ctx, INTERRUPT_STATUS, &buffer);
    if (ret == E_OK)
        *status = (interrupt_status_t) interrupt_status;
    return ret;
}


int get_current_time(sensor_t *ctx, data_time_t *time)
{
    data_buffer_t buffer = {.data = (uint8_t *) time, .data_len = sizeof(uint32_t)};
    int ret = sensor_read(ctx, CURRENT_TIME, &buffer);
    if (ret != E_OK)
        return ret;
    uint32_t u32 = *(uint32_t*)buffer.data;
    time->secs = u32 % 60;
    u32 /= 60;
    time->mins = u32 % 60;
    u32 /= 60;
    time->hours = u32 % 24;
    time->days = u32 / 24;
    return ret;
}

int set_current_time(sensor_t *ctx, data_time_t *time)
{
    uint32_t data = time->days * 24 * 3600 + time->hours * 3600 + time->mins * 60 + time->secs;
    data_buffer_t buffer = {.data = (uint8_t *) &data, .data_len = sizeof(uint32_t)};
    return sensor_write(ctx, CURRENT_TIME, &buffer);
}

#define		YEARSHIFT		9
#define 	MONTHSHIFT		5
#define		DAYSHIFT		0
static void convert_to_calendar(uint16_t u16_date, calendar_t * calendar)
{
    calendar->year = (u16_date >> YEARSHIFT) +  2000;
    calendar->month = (u16_date & 0x01e0) >> MONTHSHIFT;
    calendar->day = (u16_date & 0x001f);
}

int get_calibration_date(sensor_t *ctx, calendar_t *calendar)
{
    uint16_t data;
    data_buffer_t buffer = {.data = (uint8_t *) &data, .data_len = sizeof(data)};
    int ret = sensor_read(ctx, CALIBRATION_DATE, &buffer);
    if (ret == E_OK)
        convert_to_calendar(data, calendar);
    return ret;
}

int get_manufacturing_date(sensor_t *ctx, calendar_t *calendar)
{
    uint16_t data;
    data_buffer_t buffer = {.data = (uint8_t *) &data, .data_len = sizeof(data)};
    int ret = sensor_read(ctx, MANUFACTURED_DATE, &buffer);
    if (ret == E_OK)
        convert_to_calendar(data, calendar);
    return ret;
}

int set_device_name(sensor_t *ctx, device_name_t name)
{
    data_buffer_t buffer = {.data = (uint8_t *) name, .data_len = sizeof(device_name_t)};
    return sensor_write(ctx, DEVICE_NAME, &buffer);
}

int wait_for_device_ready(sensor_t *ctx, int max_wait_msec)
{
    int ret = E_OK;
    system_status_t s;
    while (max_wait_msec > 0)
    {
        ret = get_system_status(ctx, &s);
        if (ret == E_OK && s.device_ready)
            break;
        s19_sleep_msec(200);
        max_wait_msec -= 200;
    }
   return ret;
}

int soft_reset(sensor_t *ctx)
{
    uint32_t trigger = TRIGGER_DEVICE_RESET;
    data_buffer_t buffer = {.data = (uint8_t *) &trigger, .data_len = sizeof(trigger)};
    int ret = sensor_write(ctx, TRIGGER_REQUESTS, &buffer);
    if (ret == E_OK)
        wait_for_device_ready(ctx, 1000);
    return ret;
}

int preset_config(sensor_t *ctx)
{
    int ret;
    uint16_t u16 = 1;
    data_buffer_t buffer16 = {.data = (uint8_t *) &u16, .data_len = sizeof(u16)};
    if ((ret = sensor_write(ctx, EVENT_0_TIME_BASE, &buffer16)) != E_OK)
        return ret;

    uint32_t u32 = ENABLE_DATA_READY_INTR;
    data_buffer_t buffer32 = {.data = (uint8_t *) &u32, .data_len = sizeof(u32)};
    if ((ret = sensor_write(ctx, INTERRUPT_CONTROL, &buffer32)) != E_OK)
        return ret;

    u16 = ENABLE_EVENT_1_READ;
    ret = sensor_write(ctx, SYSTEM_CONTROL, &buffer16);
    if (ret == E_OK)
        wait_for_device_ready(ctx, 1000);
    return ret;
}

int set_interrupt_control(sensor_t *ctx, interrupt_control_t control)
{
    uint16_t data = (uint16_t) control;
    data_buffer_t buffer = {.data = (uint8_t*) &data, .data_len = sizeof(data)};
    return sensor_write(ctx, INTERRUPT_CONTROL, &buffer);
}

unsigned int sdk_version()
{
    return SMARTSENSOR_SDK_VERSION;
}

const char* measurement_str(measurement_type_t meas)
{
    switch (meas)
    {
        case SENSOR_NULL_TYPE           :   return "null";
        case SENSOR_TEMPERATURE_TYPE    :   return "temperature";
        case SENSOR_HUMIDITY_TYPE       :   return "humidity";
        case SENSOR_BAROMETRIC_TYPE     :   return "barometric";
        case SENSOR_LIGHT_TYPE          :   return "light";
        case SENSOR_VOLT_TYPE           :   return "volt";
        case SENSOR_MILLIVOLT_TYPE      :   return "milivolt";
        case SENSOR_AMP_TYPE            :   return "ampere";
        case SENSOR_MILLIAMP_TYPE       :   return "miliampere";
        case SENSOR_DIGITAL_TYPE        :   return "digital";
        case SENSOR_DIGITAL_DIN_TYPE    :   return "digital";
        case SENSOR_RATE_TYPE           :   return "rate";
        case SENSOR_PULSE_WIDTH_TYPE    :   return "pulse width";
        case SENSOR_PULSE_DELAY_TYPE    :   return "pulse delay";
        case SENSOR_DUTY_CYCLE_TYPE     :   return "duty cycle";
        case SENSOR_COUNT_TYPE          :   return "count";
        case SENSOR_UP_DOWN_COUNT_TYPE  :   return "up down count";
        case SENSOR_QUAD_COUNT_TYPE     :   return "quad count";
        case SENSOR_TC_TYPE             :   return "temperature";
        case SENSOR_RTD_TYPE            :   return "rtd";
        case SENSOR_THERMISTOR_TYPE     :   return "thermistor";
        case SENSOR_IR_TYPE             :   return "infrared";
        case SENSOR_CJC_TYPE            :   return "cold junction";
        case SENSOR_PH_TYPE             :   return "ph";
        case SENSOR_CO2_TYPE            :   return "co2";
        case SENSOR_PRESSURE_TYPE       :   return "pressure";
        case SENSOR_ACCELERATION_TYPE   :   return "acceleration";
        case SENSOR_INCLINATION_TYPE    :   return "inclination";
        case SENSOR_SERIAL_TYPE         :   return "serial";
        case SENSOR_ADC_COUNTS_TYPE     :   return "adc count";

        case SENSOR_DIGITAL_MASK        :
        default: return "unknown";
    }
}