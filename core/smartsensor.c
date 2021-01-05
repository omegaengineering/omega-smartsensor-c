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


int get_sensor_reading(sensor_t* sensor, int sensor_num, float *reading)
{
    return sensor_indexed_read(sensor, SENSOR_0_DATA, sensor_num, reading, sizeof(*reading));
}

int get_sensor_gain(sensor_t* sensor, int sensor_num, float *gain)
{
    return sensor_indexed_read(sensor, SENSOR_GAIN, sensor_num, gain, sizeof(*gain));
}

int get_sensor_offset(sensor_t* sensor, int sensor_num, float *offset)
{
    return sensor_indexed_read(sensor, SENSOR_OFFSET, sensor_num, offset, sizeof(*offset));
}

int get_device_name(sensor_t* sensor, device_name_t name)
{
    memset(name, 0, sizeof(device_name_t));
    return sensor_read(sensor, DEVICE_NAME, name, sizeof(device_name_t)-1);
}

int get_sensor_unit(sensor_t* sensor, int sensor_num, sensor_unit_t unit)
{
    memset(unit, 0, sizeof(sensor_unit_t));
    return sensor_indexed_read(sensor, SENSOR_UNIT, sensor_num, unit, sizeof(sensor_unit_t)-1);
}

int get_sensor_descriptor(sensor_t* sensor, int sensor_num, sensor_descriptor_t *descriptor)
{
    return sensor_indexed_read(sensor, SENSOR_0_DESCRIPTOR, sensor_num, descriptor, sizeof(sensor_descriptor_t));
}

int get_io_count(sensor_t* sensor, io_count_t *io_count)
{
    return sensor_read(sensor, NUMBER_OF_SENSORS, io_count, sizeof(*io_count));
}

int get_operating_stat(sensor_t* sensor, operating_stat_t *stat)
{
    return sensor_read(sensor, OPERATING_TEMP, stat, sizeof(*stat));
}

int get_sensor_type(sensor_t* sensor, int sensor_num, measurement_type_t *sensor_type)
{
    sensor_descriptor_t descriptor;
    int ret = get_sensor_descriptor(sensor, sensor_num, &descriptor);
    if (ret == E_OK)
        *sensor_type = descriptor.e_Measurement_Type;
    return ret;
}

int get_system_status(sensor_t* sensor, system_status_t *status)
{
    return sensor_read(sensor, SYSTEM_STATUS, status, sizeof(*status));
}

int get_interrupt_status(sensor_t* sensor, interrupt_status_t *status)
{
    uint16_t interrupt_status;
    int ret = sensor_read(sensor, INTERRUPT_STATUS, &interrupt_status, sizeof(interrupt_status));
    if (ret == E_OK)
        *status = (interrupt_status_t) interrupt_status;
    return ret;
}


int get_current_time(sensor_t* sensor, data_time_t *time)
{
    uint32_t u32_value;
    int ret = sensor_read(sensor, CURRENT_TIME, &u32_value, sizeof(u32_value));
    if (ret != E_OK)
        return ret;
    time->secs = u32_value % 60;
    u32_value /= 60;
    time->mins = u32_value % 60;
    u32_value /= 60;
    time->hours = u32_value % 24;
    time->days = u32_value / 24;
    return ret;
}

int set_current_time(sensor_t* sensor, data_time_t *time)
{
    uint32_t data = time->days * 24 * 3600 + time->hours * 3600 + time->mins * 60 + time->secs;
    return sensor_write(sensor, CURRENT_TIME, &data, sizeof(data));
}

#define		YEARSHIFT		9
#define 	MONTHSHIFT		5
#define		DAYSHIFT		0
static void convert_to_calendar(uint16_t u16_date, data_date_t * calendar)
{
    calendar->year = (u16_date >> YEARSHIFT) +  2000;
    calendar->month = (u16_date & 0x01e0) >> MONTHSHIFT;
    calendar->day = (u16_date & 0x001f);
}

int get_calibration_date(sensor_t* sensor, data_date_t *date)
{
    uint16_t data;
    int ret = sensor_read(sensor, CALIBRATION_DATE, &data, sizeof(data));
    if (ret == E_OK)
        convert_to_calendar(data, date);
    return ret;
}

int get_manufacturing_date(sensor_t* sensor, data_date_t *date)
{
    uint16_t data;
    int ret = sensor_read(sensor, MANUFACTURED_DATE, &data, sizeof(data));
    if (ret == E_OK)
        convert_to_calendar(data, date);
    return ret;
}

int set_device_name(sensor_t* sensor, device_name_t name)
{
    return sensor_write(sensor, DEVICE_NAME, name, sizeof(device_name_t));
}

int wait_for_device_ready(sensor_t* sensor, int max_wait_msec)
{
    int ret = E_OK;
    system_status_t s;
    while (max_wait_msec > 0)
    {
        ret = get_system_status(sensor, &s);
        if (ret == E_OK && s.device_ready)
            break;
        port_sleep_ms(200);
        max_wait_msec -= 200;
    }
   return ret;
}

int soft_reset(sensor_t* sensor)
{
    uint32_t trigger = TRIGGER_DEVICE_RESET;
    int ret = sensor_write(sensor, TRIGGER_REQUESTS, &trigger, sizeof(trigger));
    if (ret == E_OK)
        wait_for_device_ready(sensor, 1000);
    return ret;
}

int factory_reset(sensor_t* sensor)
{
    uint32_t trigger = TRIGGER_FACTORY_RESET;
    int ret = sensor_write(sensor, TRIGGER_REQUESTS, &trigger, sizeof(trigger));
    if (ret == E_OK)
        wait_for_device_ready(sensor, 1000);
    return ret;
}

int preset_config(sensor_t* sensor)
{
    int ret;
    uint16_t u16 = 1;
    if ((ret = sensor_write(sensor, EVENT_0_TIME_BASE, &u16, sizeof(u16))) != E_OK)
        return ret;

    uint32_t u32 = ENABLE_DATA_READY_INTR;;
    if ((ret = sensor_write(sensor, INTERRUPT_CONTROL, &u32, sizeof(u32))) != E_OK)
        return ret;

    u16 = ENABLE_EVENT_1_READ;
    ret = sensor_write(sensor, SYSTEM_CONTROL, &u16, sizeof(u16));
    if (ret == E_OK)
        wait_for_device_ready(sensor, 1000);
    return ret;
}

int set_interrupt_control(sensor_t* sensor, interrupt_control_t control)
{
    uint16_t data = (uint16_t) control;
    return sensor_write(sensor, INTERRUPT_CONTROL, &data, sizeof(data));
}

int set_sample_time (sensor_t* sensor, uint16_t sample_time)
{
    return sensor_write(sensor, EVENT_0_TIME_BASE, &sample_time, sizeof(sample_time));
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

 int probe_default_init(sensor_t* sensor)
{
    int ret;
    uint16_t    data16;
    uint32_t    data32;

    // Ensure Interrupt Control register set correctly
    if ((ret = sensor_read(sensor, INTERRUPT_CONTROL, &data16, sizeof(data16))))
        return ret;

    data16 |=  (ENABLE_SENSOR_CHANGE_INTR |
                ENABLE_POWER_CHANGE_INTR |
                ENABLE_HEALTH_CHANGE_INTR |
                ENABLE_DATA_READY_INTR |
                ENABLE_FUNCTION_BLOCK_INTR |
                ENABLE_LOG_DATA_READY_INTR);

    if ((ret = sensor_write(sensor, INTERRUPT_CONTROL, &data16, sizeof(data16))))
        return ret;

    if ((ret = sensor_read(sensor, SYSTEM_CONTROL, &data16, sizeof(data16))))
        return ret;

    // set the CONTROL for the Device
    data16 |= ( ENABLE_SENSOR_CHANGE_LOG |
                ENABLE_POWER_CHANGE_LOG |
                ENABLE_HEALTH_FAULT_LOG |
                ENABLE_TIME_CHANGE_LOG |
                ENABLE_EVENT_1_READ |
                ENABLE_EVENT_1_LOG |
                ENABLE_FUNCTION_BLOCK |
                ENABLE_HEALTH_MONITOR |
                ENABLE_LOG_OVERWRITE |
                ENABLE_RTC);

    if ((ret = sensor_write(sensor, SYSTEM_CONTROL, &data16, sizeof(data16))))
        return ret;

    // Extract full span of data available
    data32 = 0;
    if ((ret = sensor_write(sensor, EXTRACT_START_TIME, &data32, sizeof(data32))))
        return ret;

    data32 = 0xffffffff;
    if ((ret = sensor_write(sensor, EXTRACT_END_TIME, &data32, sizeof(data32))))
        return ret;

    // crashes smartsensor ?
//    data16 = TRIGGER_CLEAR_LOG;
//    if ((ret = sensor_write(sensor->sensor_id, TRIGGER_REQUESTS, &buffer16)))
//        return ret;

#ifdef FORCE_SAMPLE_TIME
    data16 = DEFAULT_SAMPLE_TIME;
    if ((ret = sensor_write(sensor, EVENT_0_TIME_BASE, &data16, sizeof(data16))))
        return ret;
    if ((ret = sensor_write(sensor, EVENT_1_TIME_BASE, &data16, sizeof(data16))))
        return ret;

#else
    if ((ret = sensor_read(sensor, EVENT_0_TIME_BASE, &buffer16)))
        return ret;

    if (data16 < DEFAULT_SAMPLE_TIME)
    {
        data16 = DEFAULT_SAMPLE_TIME;
        if ((ret = sensor_write(sensor, EVENT_0_TIME_BASE, &buffer16)))
            return ret;
    }
#endif
    return E_OK;
}

int system_control_set_bits(sensor_t* sensor, system_control_t bits)
{
    int ret;
    uint16_t control;
    if ((ret = sensor_read(sensor, SYSTEM_CONTROL, &control, sizeof(control))) != 0)
        return ret;
    control |= bits;
    if ((ret = sensor_write(sensor, SYSTEM_CONTROL, &control, sizeof(control))) != 0)
        return ret;
    return ret;
}

int system_control_clear_bits(sensor_t* sensor, system_control_t bits)
{
    int ret;
    uint16_t control;
    if ((ret = sensor_read(sensor, SYSTEM_CONTROL, &control, sizeof(control))) != 0)
        return ret;
    control &= ~bits;
    if ((ret = sensor_write(sensor, SYSTEM_CONTROL, &control, sizeof(control))) != 0)
        return ret;
    return ret;
}

int system_control_write_bits(sensor_t* sensor, system_control_t bits)
{
    uint16_t control = bits;
    return sensor_write(sensor, SYSTEM_CONTROL, &control, sizeof(control));
}