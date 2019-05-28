#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include "hw/bus.h"
#include "smartsensor.h"

int main()
{
    int ret;
    float f_value;
    uint32_t u32_value;
    uint16_t u16_value;
    sensor_t *sensor;

    sensor_init_t init = {
//            .bus_id = "/dev/ttyACM0",
//            .bus_type = SENSOR_BUS_MODBUS,
            .bus_id = "/dev/i2c-3",
            .bus_type = SENSOR_BUS_I2C,
            .gpio_id = 16,
            .event_callback = NULL,
            .heartbeat_period = 0
    };

    ret = sensor_new(&sensor, &init);
    assert(ret == E_OK);
    ret = sensor_open(sensor);
    assert(ret == E_OK);

    preset_config(sensor);

    uint8_t data64[64];
    data_buffer_t buffer64 = {.data = data64, .data_len = 64};
    uint8_t data32[32];
    data_buffer_t buffer32 = {.data = data32, .data_len = 32};
    uint8_t data16[16];
    data_buffer_t buffer16 = {.data = data16, .data_len = 16};
    uint8_t data8[8];
    data_buffer_t buffer8 = {.data = data8, .data_len = 8};

    while(1)
    {

        /// test read/write string
#define TEST_STRING "1234567890987654321"
        strcpy(buffer64.data, TEST_STRING);
//    ret = sensor_write(sensor, DEVICE_NAME, &buffer64);
//    assert(ret == E_OK);
////    sleep(1);   // corruption ?
    memset(buffer64.data, 0, buffer64.data_len);
        ret = sensor_read(sensor, DEVICE_NAME, &buffer64);
        assert(ret == E_OK);
        for (int i = 0; i < 32; i++)
            printf("%02x ", TEST_STRING[i]);
        printf("\n");
        for (int i = 0; i < 32; i++)
            printf("%02x ", buffer64.data[i]);
        printf("\n");
        if(strcmp(TEST_STRING, buffer64.data) != 0)
        {
            assert(0);
            printf("Failed\n");fflush(stdout);
        }
        else
            printf("Passed\n"); fflush(stdout);

    }
    // close the device
    ret = sensor_close(sensor);
    printf("Device closed with status %d\n", ret);
}