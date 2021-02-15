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
#include "log.h"
#include "errno.h"
#include "port/linux/port_linux.h"

#define USE_PLATFORM_THREAD     1
#define USE_I2C_SENSOR          1

static int do_exit = 0;
static sensor_t g_sensor = SENSOR_INIT;

static void signal_handler(int sig)
{
    do_exit = 1;
}

static void get_readings(sensor_t* sensor)
{
    int ret;
    sensor_time_t time;
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


static void example_callback(api_event_t event, void* ctx)
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
    api_event_t event;
    uint8_t data[32];
    uint16_t u16_data;
    uint32_t u32_data;

    linuxConfig_t config = {
#if I2C_SENSOR && USE_I2C_SENSOR
        .bus_res = "/dev/i2c-1",
        .bus_type = SENSOR_BUS_I2C,
#elif MODBUS_SENSOR
        .bus_res = "/dev/ttyACM0",
        .bus_type = SENSOR_BUS_MODBUS,
#endif
        .interrupt_pin = -1,
#if USE_PLATFORM_THREAD
        .event_callback = example_callback,
#endif
        .event_callback_ctx = sensor,
    };
    sensor->platform = get_platform(&config);

    ret = sensor_open(sensor);
    perror("got error");
    s_log("%d %d\n", ret, errno);
    assert(ret == E_OK);

    device_name_t device_name;
    ret = get_device_name(sensor, device_name);
    assert(ret == E_OK);
    s_log("Device name: %s\n", device_name);

    strcpy(device_name, "testing123");
    ret = set_device_name(sensor, device_name);
    assert(ret == E_OK);

    ret = get_device_name(sensor, device_name);
    assert(ret == E_OK);
    s_log("Device name: %s\n", device_name);

    goto EXIT;

    while (!do_exit)
    {
        ret = sensor_read(sensor, FIRMARE_VERSION, data, sizeof(data));
        assert(ret == E_OK);
        s_log("Firmware: 0x%08X\n", *(uint32_t*)data);

        u16_data = 2;
        ret = set_sample_time(sensor, u16_data);
        assert(ret == E_OK);

        device_name_t device_name;
        ret = get_device_name(sensor, device_name);
        assert(ret == E_OK);
        s_log("Device name: %s\n", device_name);

        sensor_date_t calendar;
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

//        ret = sensor_heartbeat_enable(sensor, 2000);
//        printf("heart beat %d\n", ret);
//        assert(ret == E_OK);

//        ret = probe_default_init(sensor);
//        assert(ret == E_OK);
    }

    while (!do_exit)
    {
#if USE_PLATFORM_THREAD
        sleep(1);
#else
        if ((ret = sensor_poll_event(sensor, &event)) == E_OK) {
            example_callback(event, sensor);
        }
#endif
    }

EXIT:
    // close the device
    s_log("Sensor closing");
    ret = sensor_close(sensor);
    assert(ret == E_OK);
    s_log("Device closed with status %d\n", ret);
}
