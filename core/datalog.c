#include "smartsensor.h"
#include "log.h"
#include "datalog.h"

/*
 * Tom notes
 * Erase either end
 * Begin to T
 * Start = T, end = Max time
 * T to latest
 * Start = 0, end = T
 */

int sensor_log_enable(sensor_t* sensor, system_control_t log_bits)
{
    return system_control_set_bits(sensor, log_bits);
}

int sensor_log_disable(sensor_t* sensor, system_control_t log_bits)
{
    return system_control_clear_bits(sensor, log_bits);
}

int sensor_log_search(sensor_t* sensor, uint32_t* start_time, uint32_t* end_time)
{
    int ret;
    uint16_t trigger;
    if ((ret = sensor_write(sensor, EXTRACT_START_TIME, start_time, sizeof(*start_time))) != E_OK)
        return ret;
    if ((ret = sensor_write(sensor, EXTRACT_END_TIME, end_time, sizeof(*end_time))) != E_OK)
        return ret;
    trigger = TRIGGER_EXTRACT;
    if ((ret = sensor_write(sensor, TRIGGER_REQUESTS, &trigger, sizeof(trigger))) != E_OK)
        return ret;
    if ((ret = sensor_read(sensor, EXTRACT_START_TIME, start_time, sizeof(*start_time))) != E_OK)
        return ret;
    if ((ret = sensor_read(sensor, EXTRACT_END_TIME, end_time, sizeof(*end_time))) != E_OK)
        return ret;
    return ret;
}

static rec_type_t sensor_log_record_type(const sensor_log_record_t* record)
{

}

static int log_record_parse(sensor_log_record_t* record)
{
    rec_framer_t * framer = &record->data.frame;
    if (framer->timestamp & 0x80000000) { // last bit is set
        if (framer->timestamp == LOG_TIME_EOF) {
            record->type = REC_EOF;
        }
        else if (framer->value[0] == _RECORD_HEALTH) {
            record->type = REC_HEALTH;
        }
        else if (framer->value[0] == _RECORD_TIME_CHANGE) {
            record->type = REC_TIME_CHANGE;
        }
        else if (framer->value[0] == _RECORD_POWER_UP) {
            record->type = REC_POWER_UP;
        }
        else if (framer->value[0] == _RECORD_POWER_DOWN) {
            record->type = REC_POWER_DOWN;
        }
        else if (framer->value[0] == _RECORD_SENSOR_CHANGE) {
            record->type = REC_SENSOR_CHANGE;
        }
        else if (framer->value[0] == _RECORD_DEVICE_RESET) {
            record->type = REC_DEVICE_RESET;
        }
        else if (framer->value[0] > _RECORD_DEVICE_RESET && framer->value[0] < 0xffff) {
            record->type = REC_USER_RECORD;
        }
        else {
            record->type = REC_UNKNOWN;
        }
    }
    else {
        record->type = REC_SENSOR_DATA;
    }
    return 0;
}

/// TODO: race condition ? log search -> log extract
int sensor_log_extract(sensor_t* sensor, sensor_log_record_t* record)
{
    int ret;
    rec_framer_t * frame = &record->data.frame;
    if ((ret = sensor_read(sensor, EXTRACTED_TIME_STAMP, &frame->timestamp, sizeof(frame->timestamp))) != E_OK)
        return ret;
    for (int i = 0; i < MAX_SENSOR_COUNT; i++) {
        if ((ret = sensor_indexed_read(sensor, EXTRACTED_DATA, i, &frame->value[i], sizeof(frame->value[i]))) != E_OK)
            return ret;
    }
    return log_record_parse(record);
}

int sensor_log_extract_next(sensor_t* sensor)
{
    uint16_t trigger = TRIGGER_EXTRACT_NEXT;
    return sensor_write(sensor, TRIGGER_REQUESTS, &trigger, sizeof(trigger));
}

int sensor_log_erase(sensor_t* sensor, uint32_t start_time, uint32_t end_time)
{
    int ret;
    uint16_t trigger;
    if ((ret = sensor_write(sensor, EXTRACT_START_TIME, &start_time, sizeof(start_time))) != E_OK)
        return ret;
    if ((ret = sensor_write(sensor, EXTRACT_END_TIME, &end_time, sizeof(end_time))) != E_OK)
        return ret;
    trigger = TRIGGER_CLEAR_LOG;
    if ((ret = sensor_write(sensor, TRIGGER_REQUESTS, &trigger, sizeof(trigger))) != E_OK)
        return ret;
    return ret;
}

int sensor_log_erase_all(sensor_t* sensor)
{
    // write both LOG_TIME_LAST_FOUND to trigger complete erase
    return sensor_log_erase(sensor, LOG_TIME_LAST_FOUND, LOG_TIME_LAST_FOUND);
}

int sensor_log_record_count(sensor_t* sensor, uint32_t* result, uint32_t start_time, uint32_t end_time)
{
    int ret;
    uint32_t cnt = 0;
    sensor_log_record_t record;
    if ((ret = sensor_log_search(sensor, &start_time, &end_time)) != E_OK)
        return ret;
    if ((ret = sensor_log_extract_next(sensor)) != E_OK)
        return ret;
    if ((ret = sensor_log_extract(sensor, &record)) != E_OK)
        return ret;
    while (record.type != REC_EOF) {
        cnt++;
        sensor_log_print_record(&record, NULL);
        if ((ret = sensor_log_extract_next(sensor)) != E_OK)
            return ret;
        if ((ret = sensor_log_extract(sensor, &record)) != E_OK)
            return ret;
    }
    *result = cnt;
    return ret;
}

void sensor_log_print_record(const sensor_log_record_t *record, const uint32_t* rec_num)
{
    char prefix[8] = "";
    if (rec_num)
        snprintf(&prefix[0], sizeof(prefix), "%-4d", *rec_num);
    switch (record->type)
    {
        case REC_UNKNOWN:
            s_log("%s REC_UNKNOWN: %08X %08X %08X %08X %08X\n",
                    prefix,
                    record->data.frame.timestamp,
                    (uint32_t)record->data.frame.value[0],
                    (uint32_t)record->data.frame.value[1],
                    (uint32_t)record->data.frame.value[2],
                    (uint32_t)record->data.frame.value[3]);
            break;
        case REC_SENSOR_DATA:
            s_log("%s SENSOR DATA: Time %d, %f, %f, %f, %f\n",
                    prefix,
                    record->data.sensor.timestamp,
                    record->data.sensor.value[0],
                    record->data.sensor.value[1],
                    record->data.sensor.value[2],
                    record->data.sensor.value[3]);
            break;
        case REC_HEALTH:
            s_log("%s HEALTH: Time %d, Temperature %d, Voltage %d\n",
                    prefix,
                    record->data.health.timestamp,
                    record->data.health.temperature,
                    record->data.health.voltage);
            break;
        case REC_TIME_CHANGE:
            s_log("%s TIME CHANGE: Time %d, Old time %d\n",
                    prefix,
                    record->data.time_change.timestamp,
                    record->data.time_change.old_time);
            break;
        case REC_POWER_UP:
            s_log("%s POWER UP: Time %d, Temperature %d, Voltage %d\n",
                    prefix,
                    record->data.power_up.timestamp,
                    record->data.power_up.temperature,
                    record->data.power_up.voltage);
            break;
        case REC_POWER_DOWN:
            s_log("%s POWER DOWN: Time %d, Temperature %d, Voltage %d\n",
                  prefix,
                  record->data.power_down.timestamp,
                  record->data.power_down.temperature,
                  record->data.power_down.voltage);
            break;
        case REC_SENSOR_CHANGE:
            s_log("%s SENSOR CHANGE: Time %d, nSensor %d, nOutput %d\n",
                    prefix,
                    record->data.sensor_change.timestamp,
                    record->data.sensor_change.num_sensors,
                    record->data.sensor_change.num_outputs);
            break;
        case REC_DEVICE_RESET:
            s_log("%s DEVICE RESET: Time %d, Reset type %d, Reset source %d\n",
                    prefix,
                    record->data.device_reset.timestamp,
                    record->data.device_reset.reset_type,
                    record->data.device_reset.reset_source);
            break;
        case REC_USER_RECORD:
            s_log("%s USER RECORD:\n",prefix);
            break;
        case REC_EOF:
            s_log("%s END OF FILE: Start time %d, End time %d\n",
                    prefix,
                    record->data.eof.start_time,
                    record->data.eof.end_time);
            break;
        default:
            break;
    }
}