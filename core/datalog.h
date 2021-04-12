#ifndef SMARTSENSOR_DATALOG_H
#define SMARTSENSOR_DATALOG_H
#include "smartsensor.h"

#define LOG_TIME_FIRST_FOUND        0
#define LOG_TIME_LAST_FOUND         0x7fffffff
#define LOG_TIME_EOF                0xffffffff

#define _RECORD_HEALTH              0x0000
#define _RECORD_TIME_CHANGE         0x0001
#define _RECORD_POWER_UP            0x0002
#define _RECORD_POWER_DOWN          0x0003
#define _RECORD_SENSOR_CHANGE       0x0004
#define _RECORD_DEVICE_RESET        0x0005

typedef enum {
    REC_UNKNOWN,
    REC_SENSOR_DATA,
    REC_HEALTH,
    REC_TIME_CHANGE,
    REC_POWER_UP,
    REC_POWER_DOWN,
    REC_SENSOR_CHANGE,
    REC_DEVICE_RESET,
    REC_USER_RECORD,
    REC_EOF,
} rec_type_t;

typedef struct {
    uint32_t    timestamp;
    float       value[MAX_SENSOR_COUNT];
} rec_framer_t;

typedef struct {
    uint32_t    timestamp;
    float       value[MAX_SENSOR_COUNT];
} rec_sensor_t;

typedef struct {
    uint32_t    timestamp;
    uint32_t    magic;
    uint8_t     temperature;
    uint8_t     voltage;
} rec_health_t;

typedef struct {
    uint32_t    timestamp;
    uint32_t    magic;
    uint32_t    old_time;
} rec_time_change_t;

typedef struct {
    uint32_t    timestamp;
    uint32_t    magic;
    uint8_t     temperature;
    uint8_t     voltage;
} rec_power_up_t;

typedef struct {
    uint32_t    timestamp;
    uint32_t    magic;
    uint8_t     temperature;
    uint8_t     voltage;
} rec_power_down_t;

typedef struct {
    uint32_t    timestamp;
    uint32_t    magic;
    uint8_t     num_sensors;
    uint8_t     num_outputs;
} rec_sensor_change_t;

typedef struct {
    uint32_t    timestamp;
    uint32_t    magic;
    uint8_t     reset_type;
    uint16_t    reset_source;
} rec_device_reset_t;

typedef struct {
    uint32_t    timestamp;
    uint32_t    magic;
    uint8_t     data[12];
} rec_user_data_t;

typedef struct {
    uint32_t    marker;
    uint32_t    start_time;
    uint32_t    end_time;
} rec_eof_t;

typedef struct {
    rec_type_t type;
    union {
        rec_framer_t        frame;
        rec_sensor_t        sensor;
        rec_health_t        health;
        rec_time_change_t   time_change;
        rec_power_up_t      power_up;
        rec_power_down_t    power_down;
        rec_sensor_change_t sensor_change;
        rec_device_reset_t  device_reset;
        rec_user_data_t     user_data;
        rec_eof_t           eof;
    } data;
} sensor_log_record_t;

int sensor_log_enable(sensor_t* sensor, system_control_t log_bits);
int sensor_log_disable(sensor_t* sensor, system_control_t log_bits);

int sensor_log_erase(sensor_t* sensor, uint32_t start_time, uint32_t end_time);
int sensor_log_erase_all(sensor_t* sensor);

/*! start_time & end_time are input/output.
 * may use LOG_TIME_FIRST_FOUND, LOG_TIME_LAST_FOUND to do blind search */
int sensor_log_search(sensor_t* sensor, uint32_t* start_time, uint32_t* end_time);
int sensor_log_extract(sensor_t* sensor, sensor_log_record_t* record);
int sensor_log_extract_next(sensor_t* sensor);

int sensor_log_record_count(sensor_t* sensor, uint32_t* result, uint32_t start_time, uint32_t end_time);
void sensor_log_print_record(const sensor_log_record_t *record, const uint32_t* rec_num);

#endif //SMARTSENSOR_DATALOG_H
