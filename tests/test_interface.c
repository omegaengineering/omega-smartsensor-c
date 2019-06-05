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
            .interrupt_pin = 16,
            .event_callback = NULL
    };

    ret = sensor_new(&sensor, &init);
    assert(ret == E_OK);
    ret = sensor_open(sensor);
    assert(ret == E_OK);

    uint8_t data64[64];
    data_buffer_t buffer64 = {.data = data64, .data_len = 64};
    uint8_t data32[32];
    data_buffer_t buffer32 = {.data = data32, .data_len = 32};
    uint8_t data16[16];
    data_buffer_t buffer16 = {.data = data16, .data_len = 16};
    uint8_t data8[8];
    data_buffer_t buffer8 = {.data = data8, .data_len = 8};


    /// test read uint16
    operating_stat_t stat;
    ret = get_operating_stat(sensor, &stat);
    assert(ret == E_OK);
    assert((int)(stat.operating_voltage/ 10) == 3);
    assert((int)(stat.operating_temp/10) == 2);

    /// test read/write float
    f_value = 123.45;
    memcpy(buffer64.data, &f_value, sizeof(f_value));
    ret = sensor_write(sensor, SENSOR_0_OFFSET, &buffer64);
    assert(ret == E_OK);
    memset(buffer64.data, 0, buffer64.data_len);
    ret = sensor_read(sensor, SENSOR_0_OFFSET, &buffer64);
    assert(ret == E_OK);
    assert((int)(f_value * 100) == (int)(*(float*)(buffer64.data) * 100));

    /// test read/write string
#define TEST_STRING "1234567890987654321"
    strcpy(buffer64.data, TEST_STRING);
    ret = sensor_write(sensor, DEVICE_NAME, &buffer64);
    assert(ret == E_OK);
    sleep(1);   // corruption ?
    memset(buffer64.data, 0, buffer64.data_len);
    ret = sensor_read(sensor, DEVICE_NAME, &buffer64);
    assert(ret == E_OK);
    assert(strcmp(TEST_STRING, buffer64.data) == 0);

#define TEST_STRING "helloworld"
    strcpy(buffer64.data, TEST_STRING);
    ret = sensor_write(sensor, DEVICE_NAME, &buffer64);
    assert(ret == E_OK);
    sleep(1);   // corruption ?
    memset(buffer64.data, 0, buffer64.data_len);
    ret = sensor_read(sensor, DEVICE_NAME, &buffer64);
    assert(ret == E_OK);
    assert(strcmp(TEST_STRING, buffer64.data) == 0);


    /// test read uint32
    data_time_t mark;
    ret = get_current_time(sensor, &mark);
    assert(ret == E_OK);
    sleep(2);
    data_time_t check;
    ret = get_current_time(sensor, &check);
    assert(ret == E_OK);
    assert(mark.days == check.days && mark.hours == check.hours &&
           mark.mins == check.mins && mark.secs < check.secs);
    /// test write uint32
    mark.days = 12;
    mark.hours = 34;
    mark.mins = 10;
    mark.secs = 20;
    ret = set_current_time(sensor, &mark);
    assert(ret == E_OK);
    sleep(2);
    ret = get_current_time(sensor, &check);
    assert(ret == E_OK);
    assert(mark.days == check.days && mark.hours == check.hours &&
            mark.mins == check.mins && mark.secs < check.secs);

    /// test some struct
    sensor_descriptor_t desc;
    ret = get_sensor_descriptor(sensor, 0, &desc);
    assert(ret == E_OK);
    assert(desc.e_Measurement_Type == SENSOR_TEMPERATURE_TYPE);
    assert(desc.st_Format.u8_Data_Type == SENSOR_DATA_TYPE_FLOAT);

    ret = get_sensor_descriptor(sensor, 1, &desc);
    assert(ret == E_OK);
    assert(desc.e_Measurement_Type == SENSOR_HUMIDITY_TYPE);
    assert(desc.st_Format.u8_Data_Type == SENSOR_DATA_TYPE_FLOAT);



    // close the device
    ret = sensor_close(sensor);
    printf("Device closed with status %d\n", ret);
}