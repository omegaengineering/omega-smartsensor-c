#include <signal.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "smartsensor.h"
#include "datalog.h"
#include "log.h"
#include "port/linux/port_linux.h"

static int do_exit = 0;
static sensor_t g_sensor;

static void signal_handler(int sig)
{
    do_exit = 1;
}


int main()
{
    int ret;
    float f1;
    uint32_t u32;
    uint16_t u16;
    uint32_t start_time, end_time;
    sensor_log_record_t record;
    signal(SIGINT, signal_handler);
    sensor_t* sensor = &g_sensor;

    linuxConfig_t config = {
        .comm.i2c.bus = "/dev/i2c-1",
        .comm.i2c.i2c_addr = 0x68,
        .comm.i2c.interrupt_pin = -1,
        .bus_type = SENSOR_BUS_I2C,
        .event_callback = NULL,
        .event_callback_ctx = sensor,
    };
    sensor->platform = get_platform(&config);

    s_log("sensor open\n");
    ret = sensor_open(sensor);
    assert(ret == E_OK);

#if INIT || 0
    u32 = time(NULL);
    ret = sensor_write(sensor, CURRENT_TIME, &u32, sizeof(u32));
    assert(ret == 0);

    u16 = 5;
    ret = sensor_write(sensor, EVENT_0_TIME_BASE, &u16, sizeof(u16));
    assert(ret == 0);

    ret = system_control_write_bits(sensor,
                                    ENABLE_EVENT_1_LOG
                                    | ENABLE_EVENT_1_READ
                                    | ENABLE_TIME_CHANGE_LOG
                                    | ENABLE_RTC);
    assert(ret == 0);

    // Extract full span of data available
    u32 = 0;
    if ((ret = sensor_write(sensor, EXTRACT_START_TIME, &u32, sizeof(u32))))
        return ret;

    u32 = 0xffffffff;
    if ((ret = sensor_write(sensor, EXTRACT_END_TIME, &u32, sizeof(u32))))
        return ret;
#endif
//
//    ret = sensor_read(sensor, CURRENT_TIME, &u32, sizeof(u32));
//    assert(ret == 0);
//    s_log("<<< sensor time: %d\n", u32);




#if FACTORY_RESET
    ret = factory_reset(sensor);
    assert(ret == 0);
    sleep(1);
#endif

#if ERASE || 0
    s_log("erase\n");
    ret = sensor_log_erase_all(sensor);
    assert(ret == 0);
    sleep(1);
    start_time = 0;
    if ((ret = sensor_write(sensor, EXTRACT_START_TIME, &start_time, sizeof(start_time))) != E_OK)
        return ret;
    end_time = LOG_TIME_LAST_FOUND;
    if ((ret = sensor_write(sensor, EXTRACT_END_TIME, &end_time, sizeof(end_time))) != E_OK)
        return ret;
    uint16_t trigger = TRIGGER_EXTRACT;
    if ((ret = sensor_write(sensor, TRIGGER_REQUESTS, &trigger, sizeof(trigger))) != E_OK)
        return ret;

#endif

#if WAIT
    // log enable
    ret = log_enable(sensor, ENABLE_EVENT_1_LOG);
    assert(ret == 0);

    uint32_t cur_time;
    uint32_t log_start = 5539987;
    uint32_t log_end = log_start + 10;
    s_log("wait 5s...\n");
    for (int i = 0; i < 5; i++)
    {
        ret = sensor_read(sensor, CURRENT_TIME, &cur_time, sizeof(cur_time));
        assert(ret == 0);
        s_log("Time %d\n", cur_time);
        sleep(1);
    }
#endif

#if LOG_SEARCH || 1
    // search log
    start_time = LOG_TIME_FIRST_FOUND;
    end_time = LOG_TIME_LAST_FOUND;
    ret = sensor_log_search(sensor, &start_time, &end_time);
    assert(ret == 0);
    s_log("log search finished. Found from %d to %d. Delta of %d\n", start_time, end_time, end_time - start_time);

    uint32_t count = 0;
    do {
        count++;

        // extract first or extract next first ?
        ret = sensor_log_extract(sensor, &record);
        assert(ret == 0);

        sensor_log_print_record(&record, &count);

    } while (sensor_log_get_record_type(&record) != REC_EOF && sensor_log_extract_next(sensor) == E_OK);

    s_log("total %d record\n", count);
#endif


    s_log("sensor close\n");
    ret = sensor_close(sensor);
    assert(ret == E_OK);


}

