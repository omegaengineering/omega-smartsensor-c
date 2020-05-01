#ifndef SMARTSENSOR_ALARM_H
#define SMARTSENSOR_ALARM_H

#include <stdint.h>
#include "function_block.h"

typedef enum alarm_mode {
    ALARM_HI    = 0x00,
    ALARM_LO    = 0x01,
    ALARM_HI_LO = 0x10,
    ALARM_BAND  = 0x11,
} alarm_mode_t;

typedef struct {
    uint8_t     block_enable: 1;
    uint8_t     block_type  : 4;
    uint8_t     alarm_mode  : 2;
    uint8_t     status      : 1;
    uint8_t     source_index: 5;
    uint8_t     bpa         : 1;
    uint8_t     event       : 1;
    uint8_t     latching    : 1;
    uint8_t     on_count    : 5;
    uint8_t     on_action   : 3;
    uint8_t     off_count   : 5;
    uint8_t     off_action  : 3;
} alarm_ctrl_w1_t;

typedef struct {
    uint8_t         opt_b1 : 1;
    union {
        uint8_t     percent_increase : 7;
        struct {
            uint8_t     index : 5;
            uint8_t     bpa : 1;
            uint8_t     fba : 1;
            uint8_t     opt : 1;
        } low_threshold;
    };

    uint8_t         opt_b2 : 1;
    union {
        uint8_t     percent_decrease : 7;
        struct {
            uint8_t     index : 5;
            uint8_t     bpa : 1;
            uint8_t     fba : 1;
            uint8_t     opt : 1;
        } high_threshold;
    };

    uint8_t     on_time_value : 7;
    uint8_t     on_time_value_opt : 1;
    uint8_t     output : 7;
    uint8_t     output_opt : 1;
} alarm_ctrl_w2_t;

typedef struct {
    uint8_t             block_idx;
    alarm_ctrl_w1_t     w1;         /*! private */
    alarm_ctrl_w2_t     w2;         /*! private */
} fb_alarm_t;

/*! alarm function block configuration API */
void fb_alarm_init(fb_alarm_t* alarm);
void fb_alarm_set_block_index(fb_alarm_t* alarm, uint8_t index);
void fb_alarm_set_mode(fb_alarm_t* alarm, alarm_mode_t mode);
void fb_alarm_set_latching(fb_alarm_t* alarm, uint8_t latch);
void fb_alarm_set_sensor(fb_alarm_t* alarm, uint8_t sensor_index);
void fb_alarm_enable(fb_alarm_t* alarm, uint8_t enable);
void fb_alarm_ctrl_set_percent_increase_rate(fb_alarm_t* alarm, uint8_t rate);
void fb_alarm_ctrl_set_percent_decrease_rate(fb_alarm_t* alarm, uint8_t rate);

int  fb_alarm_assign_user_param(sensor_t* sensor, uint8_t param_index, float value);
void fb_alarm_ctrl_set_low_threshold_value(fb_alarm_t* alarm, uint8_t param_index);
void fb_alarm_ctrl_set_high_threshold_value(fb_alarm_t* alarm, uint8_t param_index);

int fb_alarm_commit(sensor_t* sensor, fb_alarm_t* alarm);
int fb_alarm_get_status(sensor_t* sensor, fb_alarm_t* alarm, uint8_t* status);

#endif //SMARTSENSOR_ALARM_H
