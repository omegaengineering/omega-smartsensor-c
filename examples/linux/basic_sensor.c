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
#include <pthread.h>
#include "smartsensor.h"
#include "log.h"
#include "port/platform/linux.h"


static int do_exit = 0;
static sensor_t g_sensor;

#if 1
static port_cfg_t port_cfg = {
        .bus_res = "/dev/i2c-10",
        .bus_type = SENSOR_BUS_I2C,
        .pin_intr = 4,
        .pin_intr_enable = 1,
};
#endif
#if 0
static port_cfg_t port_cfg = {
        .bus_res = "/dev/i2c-2",
        .bus_type = SENSOR_BUS_I2C,
        .pin_intr = 60,
        .pin_intr_enable = 1,
};
#endif
#if 0
static port_cfg_t port_cfg = {
        .bus_res = "/dev/ttyACM0",
        .bus_type = SENSOR_BUS_MODBUS,
        .pin_intr = 0,
        .pin_intr_enable = 0,
};
#endif

void signal_handler(int sig)
{
    do_exit = 1;
}

void get_readings(sensor_t* sensor)
{
    int ret;
    data_time_t time;
    ret = get_current_time(sensor, &time);
    assert(ret == E_OK);
    s_log("Current time %d days %d hours %d mins %d secs\n", time.days, time.hours, time.mins, time.secs);

    s_log("Sensor Reading = ");
    for (int idx = 0; idx < MAX_SENSOR_COUNT; idx++)
    {
        float reading;
        ret = get_sensor_reading(sensor, idx, &reading);
        if (ret != E_OK)
            return;
        sensor_unit_t unit;
        ret = get_sensor_unit(sensor, idx, unit);
        if (ret != E_OK)
            return;
        s_log("%0.2f %s \t", reading, unit);
    }
    s_log("\n");
}


void example_callback(api_event_t event, void* ctx)
{
    if (!ctx)
        return;
    sensor_t* sensor = (sensor_t*) ctx;
    if (event & API_DATA_READY)
    {
        get_readings(sensor);
    }
    if (event & API_EVENT_SENSOR_ATTACHED)
    {
        s_log("API_EVENT_SENSOR_ATTACHED\n");
    }
    if (event & API_EVENT_SENSOR_DETACHED)
    {
        s_log("API_EVENT_SENSOR_DETACHED\n");
    }
}


int main()
{
    int ret;
    signal(SIGINT, signal_handler);
    sensor_t* sensor = &g_sensor;

    sensor_init_t init = {
        .bus_type = SENSOR_BUS_I2C,
        .event_callback = example_callback,
        .event_callback_ctx = sensor,
    };

    s_log("sensor init\n");
    ret = sensor_init(sensor, &init);
    assert(ret == E_OK);

//    pthread_create(&sensor_thread, NULL, sensor_handler, NULL);

    s_log("sensor open\n");
    ret = sensor_open(sensor, &port_cfg, sizeof(port_cfg));
    assert(ret == E_OK);

    uint8_t data[32];

    ret = sensor_read(sensor, DEVICE_ID, data, sizeof(data));
    assert(ret == E_OK);
    s_log("Device Id: 0x%08X\n", *(uint32_t*)data );

    ret = sensor_read(sensor, FIRMARE_VERSION, data, sizeof(data));
    assert(ret == E_OK);
    s_log("Firmware: 0x%08X\n", *(uint32_t*)data);

    device_name_t device_name;
    ret = get_device_name(sensor, device_name);
    assert(ret == E_OK);
    s_log("Device name: %s\n", device_name);

    data_date_t calendar;
    ret = get_calibration_date(sensor, &calendar);
    assert(ret == E_OK);
    s_log("Calibration date: %02d/%02d/%04d\n", calendar.month, calendar.day, calendar.year);
    ret = get_manufacturing_date(sensor, &calendar);
    assert(ret == E_OK);
    s_log("MFR date: %02d/%02d/%04d\n", calendar.month, calendar.day, calendar.year);

    operating_stat_t stat;
    ret = get_operating_stat(sensor, &stat);
    assert(ret == E_OK);
    s_log("Sensor Temperature = %d.\n"
           "Sensor Voltage = %d.\n", stat.operating_temp, stat.operating_voltage);
    io_count_t io_count;
    ret = get_io_count(sensor, &io_count);
    assert(ret == E_OK);
    s_log("On-board %d sensors.\n", io_count.sensor_count);
    s_log("On-board %d outputs.\n", io_count.output_count);

    ret = sensor_heartbeat_enable(sensor, 2000);
    assert(ret == E_OK);

    probe_default_init(sensor);

    while (!do_exit)
    {
        sleep(1);
    }

    // close the device
    s_log("Sensor closing");
    ret = sensor_close(sensor);
    assert(ret == E_OK);
    s_log("Device closed with status %d\n", ret);
}
