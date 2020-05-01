#include <string.h>
#include "smartsensor.h"
#include "log.h"
#include "alarm.h"

void fb_alarm_init(fb_alarm_t* alarm)
{
    memset(alarm, 0, sizeof(*alarm));
    alarm->w1.block_type = FUNCTION_BLOCK_ALARM;
}
void fb_alarm_set_block_index(fb_alarm_t* alarm, uint8_t index)
{
    alarm->block_idx = index;
}

void fb_alarm_set_mode(fb_alarm_t* alarm, alarm_mode_t mode)
{
    alarm->w1.alarm_mode = mode;
}

void fb_alarm_set_latching(fb_alarm_t* alarm, uint8_t latch)
{
    alarm->w1.latching = latch != 0;
}

void fb_alarm_set_sensor(fb_alarm_t* alarm, uint8_t sensor_index)
{
    alarm->w1.source_index = sensor_index;
}

void fb_alarm_enable(fb_alarm_t* alarm, uint8_t enable)
{
    alarm->w1.block_enable = enable;
}

void fb_alarm_ctrl_set_percent_increase_rate(fb_alarm_t* alarm, uint8_t rate)
{
    alarm->w2.opt_b1 = 0;
    alarm->w2.percent_increase = rate;
}

void fb_alarm_ctrl_set_percent_decrease_rate(fb_alarm_t* alarm, uint8_t rate)
{
    alarm->w2.opt_b2 = 0;
    alarm->w2.percent_decrease = rate;
}

void fb_alarm_ctrl_set_low_threshold_value(fb_alarm_t* alarm, uint8_t param_index)
{
    alarm->w2.opt_b1 = 1;
    alarm->w2.low_threshold.index = param_index;
    alarm->w2.low_threshold.fba = 0;
    alarm->w2.low_threshold.bpa = 1;
}

void fb_alarm_ctrl_set_high_threshold_value(fb_alarm_t* alarm, uint8_t param_index)
{
    alarm->w2.opt_b2 = 1;
    alarm->w2.high_threshold.index = param_index;
    alarm->w2.high_threshold.fba = 0;
    alarm->w2.high_threshold.bpa = 1;
}

int fb_alarm_commit(sensor_t* sensor, fb_alarm_t* alarm)
{
    int ret;
    if ((ret = fb_program_write(sensor, alarm->block_idx, &alarm->w1, sizeof(alarm->w1))) != E_OK)
        return ret;
    if ((ret = fb_program_write(sensor, alarm->block_idx+1, &alarm->w2, sizeof(alarm->w2))) != E_OK)
        return ret;
    return ret;
}

int fb_alarm_get_status(sensor_t* sensor, fb_alarm_t* alarm, uint8_t* status)
{
    int ret;
    fb_alarm_t fb;
    if ((ret = fb_program_read(sensor, alarm->block_idx, &fb.w1, sizeof(fb.w1))) != E_OK)
        return ret;
    *status = fb.w1.status;
    return E_OK;
}

int fb_alarm_assign_user_param(sensor_t* sensor, uint8_t param_index, float value)
{
    return fb_param_write(sensor, param_index, &value, sizeof(value));
}
