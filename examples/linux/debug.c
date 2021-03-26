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
            .bus_type = SENSOR_BUS_MODBUS,
            .comm.modbus.bus = "/dev/ttyUSB0",
            .comm.modbus.modbus_addr = 0x01,
            .comm.modbus.baud_rate = 115200,
            .comm.modbus.data_bit = 8,
            .comm.modbus.parity = 'E',
            .comm.modbus.stop_bit = 1,
#if USE_PLATFORM_THREAD
            .event_callback = example_callback,
#endif
            .event_callback_ctx = sensor,
    };
    sensor->platform = get_platform(&config);

    ret = sensor_open(sensor);
    assert(ret == E_OK);

    uint32_t deviceId;
    ret = sensor_read(sensor, DEVICE_ID, &deviceId, sizeof(deviceId));
    assert(ret == 0);
    s_log("device id = %08X\n", deviceId);

//    probe_status_t probestatus;
//    ret = sensor_read(sensor, PROBE_STATUS, &probestatus, sizeof(probestatus));
//    assert(ret == 0);
//    printf("probe status = %d %d \n", probestatus.p0_active, probestatus.p1_active);

    measurement_type_t type;
    ret = get_sensor_type(sensor, 0, &type);
    assert(ret == E_OK);
    s_log("type %d\n", type);

    for (int i = 0; i < 4; i++)
    {
        sensor_unit_t unit;
        ret = get_sensor_unit(sensor, i, unit);
        assert(ret == 0);
        s_log("unit %s\n", unit);
    }



    sensor_descriptor_t desc;


//    ret = get_sensor_descriptor(sensor, 0, &desc);
//    assert(ret == E_OK);
//    s_log("meas %d tc type %d\n",
//          desc.e_Measurement_Type,
//          desc.st_Config.u8_Sensor_Range);
//    s_log("%02x %02x %02x %02x\n", ((uint8_t*)&desc)[0],
//          ((uint8_t*)&desc)[1], ((uint8_t*)&desc)[2], ((uint8_t*)&desc)[3]);
//
//    ret = get_sensor_descriptor(sensor, 1, &desc);
//    assert(ret == E_OK);
//    s_log("meas %d tc type %d\n",
//          desc.e_Measurement_Type,
//          desc.st_Config.u8_Sensor_Range);
//    s_log("%02x %02x %02x %02x\n", ((uint8_t*)&desc)[0],
//          ((uint8_t*)&desc)[1], ((uint8_t*)&desc)[2], ((uint8_t*)&desc)[3]);
//
//    memset(&desc, 0, sizeof(desc));
//    ret = get_sensor_descriptor(sensor, 2, &desc);
//    assert(ret == E_OK);
//    s_log("meas %d tc type %d\n",
//          desc.e_Measurement_Type,
//          desc.st_Config.u8_Sensor_Range);
//    s_log("%02x %02x %02x %02x\n", ((uint8_t*)&desc)[0],
//          ((uint8_t*)&desc)[1], ((uint8_t*)&desc)[2], ((uint8_t*)&desc)[3]);
//
//    ret = get_sensor_descriptor(sensor, 3, &desc);
//    assert(ret == E_OK);
//    s_log("meas %d tc type %d\n",
//          desc.e_Measurement_Type,
//          desc.st_Config.u8_Sensor_Range);
//    s_log("%02x %02x %02x %02x\n", ((uint8_t*)&desc)[0],
//          ((uint8_t*)&desc)[1], ((uint8_t*)&desc)[2], ((uint8_t*)&desc)[3]);

//    measurement_type_t type;
//    ret = get_sensor_type(sensor, 0, &type);
//    assert(ret == 0);
//    printf("%d\n", type);
//    ret = get_sensor_type(sensor, 1, &type);
//    assert(ret == 0);
//    printf("%d\n", type);
//    ret = get_sensor_type(sensor, 2, &type);
//    assert(ret == 0);
//    printf("%d\n", type);
//    ret = get_sensor_type(sensor, 3, &type);
//    assert(ret == 0);
//    printf("%d\n", type);

//    s_log("%02x %02x %02x %02x\n",
//          ((uint8_t*)&desc)[0],
//          ((uint8_t*)&desc)[1],
//          ((uint8_t*)&desc)[2],
//          ((uint8_t*)&desc)[3]);

//    desc.st_Config.u8_Sensor_Range = 2;
//    s_log("%02x %02x %02x %02x\n",
//          ((uint8_t*)&desc)[0],
//          ((uint8_t*)&desc)[1],
//          ((uint8_t*)&desc)[2],
//          ((uint8_t*)&desc)[3]);

//    ret = set_sensor_descriptor(sensor, 0, &desc);
//    assert(ret == E_OK);



//    s_log("changed type to 2, resetting\n");
//    soft_reset(sensor, 0);
//    sleep(1);


    EXIT:
    // close the device
    s_log("Sensor closing");
    ret = sensor_close(sensor);
    assert(ret == E_OK);
    s_log("Device closed with status %d\n", ret);
}
