/*!********************************************************************************************
  @file     basic_sensor.c

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
            Example for using smartsensor device in interrupt mode

***********************************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include "smartsensor.h"


static int do_exit = 0;

void signal_handler(int sig)
{
    do_exit = 1;
}

void get_readings(int instance)
{
    int ret;
    data_time_t time;
    ret = get_current_time(instance, &time);
    assert(ret == E_OK);
    printf("Current time %d days %d hours %d mins %d secs\n", time.days, time.hours, time.mins, time.secs);

    printf("Sensor Reading = ");
    for (int idx = 0; idx < MAX_SENSOR_COUNT; idx++)
    {
        float reading;
        ret = get_sensor_reading(instance, idx, &reading);
        if (ret != E_OK)
            return;
        sensor_unit_t unit;
        ret = get_sensor_unit(instance, idx, unit);
        if (ret != E_OK)
            return;
        printf("%0.2f %s \t", reading, unit);
    }
    printf("\n");
}


void example_callback(api_event_t event, void* ctx)
{
    int instance = *(int*) ctx;

    if (event & API_DATA_READY)
    {
        get_readings(SMARTSENSOR_0);
    }
    if (event & API_EVENT_SENSOR_ATTACHED)
    {
        printf("API_EVENT_SENSOR_ATTACHED\n");
    }
    if (event & API_EVENT_SENSOR_DETACHED)
    {
        printf("API_EVENT_SENSOR_DETACHED\n");
    }
}


int main()
{
    int ret;
    signal(SIGINT, signal_handler);

    sensor_init_t init = {
        .bus_type = SENSOR_BUS_I2C,
        .event_callback = example_callback,
        .event_callback_ctx = NULL,
    };

    ret = sensor_init(SMARTSENSOR_0, &init);
    assert(ret == E_OK);

    uint8_t data[32];
    data_buffer_t buffer = {.data = data, .data_len = 32};

    ret = sensor_read(SMARTSENSOR_0, DEVICE_ID, &buffer);
    assert(ret == E_OK);
    printf("Device Id: 0x%08X\n", *(uint32_t*)buffer.data );

    ret = sensor_read(SMARTSENSOR_0, FIRMARE_VERSION, &buffer);
    assert(ret == E_OK);
    printf("Firmware: 0x%08X\n", *(uint32_t*)buffer.data);

    device_name_t device_name;
    ret = get_device_name(SMARTSENSOR_0, device_name);
    assert(ret == E_OK);
    printf("Device name: %s\n", device_name);

    calendar_t calendar;
    ret = get_calibration_date(SMARTSENSOR_0, &calendar);
    assert(ret == E_OK);
    printf("Calibration date: %02d/%02d/%04d\n", calendar.month, calendar.day, calendar.year);
    ret = get_manufacturing_date(SMARTSENSOR_0, &calendar);
    assert(ret == E_OK);
    printf("MFR date: %02d/%02d/%04d\n", calendar.month, calendar.day, calendar.year);

    operating_stat_t stat;
    ret = get_operating_stat(SMARTSENSOR_0, &stat);
    assert(ret == E_OK);
    printf("Sensor Temperature = %d.\n"
           "Sensor Voltage = %d.\n", stat.operating_temp, stat.operating_voltage);
    io_count_t io_count;
    ret = get_io_count(SMARTSENSOR_0, &io_count);
    assert(ret == E_OK);
    printf("On-board %d sensors.\n", io_count.sensor_count);
    printf("On-board %d outputs.\n", io_count.output_count);

    while (!do_exit)
    {
        sleep(1);
        float value;
        if (get_sensor_reading(SMARTSENSOR_0, 0, &value) == 0)
            printf("Sensor value: %.2f\n", value);
    }

    // close the device
    ret = sensor_deinit(SMARTSENSOR_0);
    assert(ret == E_OK);
    printf("Device closed with status %d\n", ret);
}
