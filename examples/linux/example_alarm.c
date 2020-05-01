#include <signal.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "smartsensor.h"
#include "alarm.h"
#include "log.h"
#include "port/platform/linux.h"

static int do_exit = 0;
static sensor_t g_sensor;


static port_cfg_t port_cfg = {
        .bus_res = "/dev/i2c-2",
        .bus_type = SENSOR_BUS_I2C,
        .pin_intr = 60,
        .pin_intr_enable = 1,
};

void signal_handler(int sig)
{
    do_exit = 1;
}


int main()
{
    int ret;
    float f1;
    uint32_t u32;
    uint16_t u16;
    signal(SIGINT, signal_handler);
    sensor_t* sensor = &g_sensor;

    sensor_init_t init = {
            .bus_type = SENSOR_BUS_I2C,
            .event_callback = NULL,
            .event_callback_ctx = sensor,
    };

    s_log("sensor init\n");
    ret = sensor_init(sensor, &init);
    assert(ret == E_OK);

    s_log("sensor open\n");
    ret = sensor_open(sensor, &port_cfg, sizeof(port_cfg));
    assert(ret == E_OK);

    ret = fb_disable(sensor);
    assert(ret == 0);

    ret = fb_program_erase_all(sensor);
    assert(ret == 0);

    // configuring alarm func block
    fb_alarm_t alarm1;
    fb_alarm_init(&alarm1);
    fb_alarm_set_mode(&alarm1, ALARM_HI);
    fb_alarm_set_latching(&alarm1, 0);
    fb_alarm_set_sensor(&alarm1, SENSOR_0);
    fb_alarm_enable(&alarm1, 1);
    ret = fb_alarm_assign_user_param(sensor, 0, 25.6f);
    assert(ret == 0);
    fb_alarm_ctrl_set_high_threshold_value(&alarm1, 0);

    s_log("alarm1 word 1 %08X\n", alarm1.w1.block_data);
    s_log("alarm1 word 2 %08X\n", alarm1.w2.block_data);

    // make alarm1 the first block
    fb_init_head((fb_block_t*) &alarm1, 0);
    // done with adding blocks
    fb_block_t eob;
    fb_terminate((fb_block_t*) &alarm1, &eob);

    fb_print(&alarm1.w1);

    // write to sensor
    fb_commit(sensor, (fb_block_t*) &alarm1);

    // enable func block processing
    ret = fb_enable(sensor);
    assert(ret == 0);

    while (!do_exit)
    {
        uint8_t status;
        float reading;
        ret = get_sensor_reading(sensor, 0, &reading);
        assert(ret == 0);
        ret = fb_alarm_get_status(sensor, &alarm1, &status);
        assert(ret == 0);
        s_log("reading = %0.2f, status = %d\n", reading, status);
        sleep(1);
    }

    s_log("sensor close\n");
    ret = sensor_close(sensor);
    assert(ret == E_OK);


}