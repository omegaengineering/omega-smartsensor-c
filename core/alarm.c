#include <string.h>
#include <assert.h>
#include "smartsensor.h"
#include "log.h"
#include "alarm.h"

void fb_alarm_init(fb_alarm_t* alarm)
{
    assert(sizeof(alarm_ctrl_w1_t) == sizeof(uint32_t));
    assert(sizeof(alarm_ctrl_w2_t) == sizeof(uint32_t));
    memset(alarm, 0, sizeof(*alarm));
    ((alarm_ctrl_w1_t*)(&alarm->w1.block_data))->block_type = FUNCTION_BLOCK_ALARM;
    alarm->w1.next = &alarm->w2;
    alarm->w2.next = NULL;
}

void fb_alarm_set_mode(fb_alarm_t* alarm, alarm_mode_t mode)
{
    ((alarm_ctrl_w1_t*)(&alarm->w1.block_data))->alarm_mode = mode;
}

void fb_alarm_set_latching(fb_alarm_t* alarm, uint8_t latch)
{
    ((alarm_ctrl_w1_t*)(&alarm->w1.block_data))->latching = latch != 0;
}

void fb_alarm_set_sensor(fb_alarm_t* alarm, uint8_t sensor_index)
{
    ((alarm_ctrl_w1_t*)(&alarm->w1.block_data))->source_index = sensor_index;
}

void fb_alarm_enable(fb_alarm_t* alarm, uint8_t enable)
{
    ((alarm_ctrl_w1_t*)(&alarm->w1.block_data))->block_enable = enable;
}

void fb_alarm_ctrl_set_percent_increase_rate(fb_alarm_t* alarm, uint8_t rate)
{
    ((alarm_ctrl_w2_t*)(&alarm->w2.block_data))->percent_increase.opt = 0;
    ((alarm_ctrl_w2_t*)(&alarm->w2.block_data))->percent_increase.value = rate;
}

void fb_alarm_ctrl_set_percent_decrease_rate(fb_alarm_t* alarm, uint8_t rate)
{
    ((alarm_ctrl_w2_t*)(&alarm->w2.block_data))->percent_decrease.opt = 0;
    ((alarm_ctrl_w2_t*)(&alarm->w2.block_data))->percent_decrease.value = rate;
}

void fb_alarm_ctrl_set_low_threshold_value(fb_alarm_t* alarm, uint8_t param_index)
{
    ((alarm_ctrl_w2_t*)(&alarm->w2.block_data))->low_threshold.opt = 1;
    ((alarm_ctrl_w2_t*)(&alarm->w2.block_data))->low_threshold.index = param_index;
    ((alarm_ctrl_w2_t*)(&alarm->w2.block_data))->low_threshold.fba = 0;
    ((alarm_ctrl_w2_t*)(&alarm->w2.block_data))->low_threshold.bpa = 1;
}

void fb_alarm_ctrl_set_high_threshold_value(fb_alarm_t* alarm, uint8_t param_index)
{
    ((alarm_ctrl_w2_t*)(&alarm->w2.block_data))->high_threshold.opt = 1;
    ((alarm_ctrl_w2_t*)(&alarm->w2.block_data))->high_threshold.index = param_index;
    ((alarm_ctrl_w2_t*)(&alarm->w2.block_data))->high_threshold.fba = 0;
    ((alarm_ctrl_w2_t*)(&alarm->w2.block_data))->high_threshold.bpa = 1;
}

int fb_alarm_get_status(sensor_t* sensor, fb_alarm_t* alarm, uint8_t* status)
{
    int ret;
    alarm_ctrl_w1_t w1;
    if ((ret = fb_program_read(sensor, alarm->w1.block_idx, &w1, sizeof(w1))) != E_OK)
        return ret;
    *status = w1.status;
    return E_OK;
}

int fb_alarm_assign_user_param(sensor_t* sensor, uint8_t param_index, float value)
{
    return fb_param_write(sensor, param_index, &value, sizeof(value));
}
