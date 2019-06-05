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


#include "contiki.h"
#include "core/smartsensor.h"

#include <stdio.h> /* For printf() */
#include <assert.h>

void get_readings(sensor_t *sensor)
{
    int ret;
    printf("Sensor Reading = ");
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
        printf("%d %s \t", (int)(reading*100), unit);
    }
    printf("\n");
}

void my_callback(api_event_t event, void * data)
{
    sensor_t *sensor = *(sensor_t**) data;

    if (event & API_EVENT_SENSOR_ATTACHED)
        printf("Sensor attached\n");
    if (event & API_EVENT_SENSOR_DETACHED)
        printf("Sensor detached\n");
    if (event & API_DATA_READY)
    {
        get_readings(sensor);
    }
}

sensor_t *sensor;

sensor_init_t init = {
        .bus_type = SENSOR_BUS_I2C,
        .interrupt_pin = 22,
        .event_callback = my_callback,
        .event_callback_ctx = &sensor
};

/*---------------------------------------------------------------------------*/
PROCESS(basic_sensor, "Basic Sensor");

AUTOSTART_PROCESSES(&basic_sensor);

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(basic_sensor, ev, data)
{
  static struct etimer timer;
  static int ret;
  static uint8_t data32[32];
  static data_buffer_t buffer32 = {.data = data32, .data_len = 32};

  PROCESS_BEGIN();

  ret = sensor_new(&sensor, &init);
  if (ret != E_OK)
      printf("Failed to create new sensor\n");
  ret = sensor_open(sensor);
  if (ret != E_OK)
      printf("Failed to open sensor\n");

  ret = sensor_read(sensor, DEVICE_ID, &buffer32);
  assert(ret == E_OK);
  printf("Device Id: 0x%08X\n", *(unsigned int*)buffer32.data );

  ret = sensor_read(sensor, FIRMARE_VERSION, &buffer32);
  assert(ret == E_OK);
  printf("Firmware: 0x%08X\n", *(unsigned int*)buffer32.data);

  device_name_t device_name;
  ret = get_device_name(sensor, device_name);
  assert(ret == E_OK);
  printf("Device name: %s\n", device_name);

  calendar_t calendar;
  ret = get_calibration_date(sensor, &calendar);
  assert(ret == E_OK);
  printf("Calibration date: %02d/%02d/%04d\n", calendar.month, calendar.day, calendar.year);
  ret = get_manufacturing_date(sensor, &calendar);
  assert(ret == E_OK);
  printf("MFR date: %02d/%02d/%04d\n", calendar.month, calendar.day, calendar.year);

  operating_stat_t stat;
  ret = get_operating_stat(sensor, &stat);
  assert(ret == E_OK);
  printf("Sensor Temperature = %d.\n"
         "Sensor Voltage = %d.\n", stat.operating_temp, stat.operating_voltage);

  io_count_t io_count;
  ret = get_io_count(sensor, &io_count);
  assert(ret == E_OK);
  printf("On-board %d sensors.\n", io_count.sensor_count);
  printf("On-board %d outputs.\n", io_count.output_count);

  etimer_set(&timer, CLOCK_SECOND * 1);

  while(sensor != NULL)
  {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    //get_readings(sensor);

    etimer_reset(&timer);
  }

  PROCESS_END();

  ret = sensor_close(sensor);
  if (ret != E_OK)
      printf("Failed to close sensor\n");

  ret = sensor_free(sensor);
  if (ret != E_OK)
      printf("Failed to free sensor\n");
}
/*--------------------------------------------------------------------------*/
