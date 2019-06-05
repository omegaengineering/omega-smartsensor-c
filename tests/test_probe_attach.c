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

void fault_handler(int sig)
{
    printf("Oops\n");
    fflush(stdout);
}

int api_event_probe_attached = 0;
int api_event_probe_detached = 0;


void example_callback(api_event_t event, void* ctx)
{
    sensor_t *sensor = *(sensor_t**) ctx;
    int ret;

    if (event & API_EVENT_SENSOR_ATTACHED)
    {
        printf("API_EVENT_SENSOR_ATTACHED\n");
        api_event_probe_attached++;
    }
    if (event & API_EVENT_SENSOR_DETACHED)
    {
        printf("API_EVENT_SENSOR_DETACHED\n");
        api_event_probe_detached++;
    }
}


int main()
{

    int ret;
    sensor_t *sensor;
    signal(SIGINT, signal_handler);
    signal(SIGSEGV, fault_handler);

    sensor_init_t init = {
//            .bus_id = "/dev/ttyACM0",
//            .bus_type = SENSOR_BUS_MODBUS,
            .bus_id = "/dev/i2c-3",
            .bus_type = SENSOR_BUS_I2C,
            .interrupt_pin = 16,
            .event_callback = example_callback,
            .event_callback_ctx = &sensor,
            .heartbeat_period = 1
    };

    for (int i = 0; i < 100000 && !do_exit; i++)
    {
        api_event_probe_attached = 0;
        api_event_probe_detached = 0;

        ret = sensor_new(&sensor, &init);
        assert(ret == E_OK);
        ret = sensor_open(sensor);
        assert(ret == E_OK);

        sleep(1);
        ret = sensor_close(sensor);
        assert(ret == E_OK);
        ret = sensor_free(sensor);
        assert(ret == E_OK);

       // sleep(1);   // pickup pending callbacks

        if (api_event_probe_attached == api_event_probe_detached)
            printf("PASS!\n");
        else
            printf("FAILED!. Attached = %d. Detached = %d\n", api_event_probe_attached, api_event_probe_detached);
        printf("------------------------\n"); fflush(stdout);
    }
    printf("Device closed with status %d\n", ret);
}
