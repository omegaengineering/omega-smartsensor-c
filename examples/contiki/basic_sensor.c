
#include "contiki.h"
#include "core/smartsensor.h"

#include <stdio.h> /* For printf() */
#include <assert.h>

sensor_init_t init = {
     .bus_type = SENSOR_BUS_I2C
};

sensor_t *sensor;

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Basic Sensor");
AUTOSTART_PROCESSES(&hello_world_process);



/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  static struct etimer timer;
  int ret;
  char buffer[32];
  data_buffer_t data32 = {.data = buffer, .data_len = 32};
  float reading;

  PROCESS_BEGIN();

  /* Setup a periodic timer that expires after 10 seconds. */
  etimer_set(&timer, CLOCK_SECOND * 3);

  ret = sensor_new(&sensor, &init);
  if (ret != E_OK)
      printf("Failed to create new sensor\n");
  ret = sensor_open(sensor);
  if (ret != E_OK)
      printf("Failed to open sensor\n");

  ret = sensor_read(sensor, FIRMARE_VERSION, &data32);
   if (ret == E_OK)
       printf("FIRMWARE VERSION = %08X\n", *(uint32_t*)&data32.data);

  while(sensor != NULL)
  {
    ret = get_sensor_reading(sensor, 0, &reading);

    if (ret == E_OK)
        printf("Sensor 1 = %d\n", (uint32_t)(reading*100));

    /* Wait for the periodic timer to expire and then restart the timer. */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

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
/*--------------------------------------------------------------------------
