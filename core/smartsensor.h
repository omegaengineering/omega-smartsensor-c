#ifndef OMEGA_SMARTSENSOR_C_SMARTSENSOR_H
#define OMEGA_SMARTSENSOR_C_SMARTSENSOR_H

#include "common/common.h"
#include "common/errors.h"
#include "registers.h"


/************************************************************************************************
 * Public interface for interacting with smart sensor.
 * The interface is the wrapper for the smartsensor device,
 * plus additional easy to use access functions to retrieve/write data.
 *************************************************************************************************/

#define SMARTSENSOR_I2C_ADDR            0x68
#define SMARTSENSOR_MODBUS_ADDR         0x01
#define SMARTSENSOR_MODBUS_BAUDRATE     38400
#define DEFAULT_SAMPLE_TIME             1
#define HEARTBEAT                       1
#define HEARTBEAT_MAX_MISS              3

struct _sensor;

typedef struct _sensor sensor_t;

typedef enum {
    API_SENSOR_CHANGE           = SENSOR_CHANGE_INTR,
    API_POWER_CHANGE            = POWER_CHANGE_INTR,
    API_HEALTH_CHANGE           = HEALTH_CHANGE_INTR,
    API_EVENT_0                 = EVENT_0_INTR,
    API_EVENT_1                 = EVENT_1_INTR,
    API_DATA_READY              = DATA_READY_INTR,
    API_FUNCTION_BLOCK          = FUNCTION_BLOCK_INTR,
    API_LOG_DATA_READY          = LOG_DATA_READY_INTR,
    API_EVENT_SENSOR_ATTACHED   = 0x00010000,
    API_EVENT_SENSOR_DETACHED   = 0x00020000
} api_event_t;

typedef void (*event_callback_t)(api_event_t event, void *);

typedef enum {
    SENSOR_BUS_I2C,
    SENSOR_BUS_MODBUS
} sensor_bus_type_t;

typedef struct {
    char                bus_id[128];
    sensor_bus_type_t   bus_type;
    int                 gpio_id;
    event_callback_t    event_callback;
    void*               event_callback_ctx;
    int                 heartbeat_period;
} sensor_init_t;


int sensor_new              (sensor_t **ctx, const sensor_init_t *init);
int sensor_open             (sensor_t *ctx);
int sensor_read             (sensor_t *ctx, ss_register_t ss_register, data_buffer_t * data_buffer);
int sensor_write            (sensor_t *ctx, ss_register_t ss_register, data_buffer_t * data_buffer);
int sensor_close            (sensor_t *ctx);
int sensor_free             (sensor_t *ctx);

int get_sensor_reading      (sensor_t *ctx, int sensor_num, const float *reading);
int get_device_name         (sensor_t *ctx, device_name_t name);
int get_sensor_unit         (sensor_t *ctx, int sensor_num, sensor_unit_t unit);
int get_sensor_descriptor   (sensor_t *ctx, int sensor_num, sensor_descriptor_t *descriptor);
int get_sensor_type         (sensor_t *ctx, int sensor_num, measurement_type_t *sensor_type);
int get_system_status       (sensor_t *ctx, system_status_t *status);
int get_interrupt_status    (sensor_t *ctx, interrupt_status_t *status);
int get_current_time        (sensor_t *ctx, data_time_t *time);
int get_io_count            (sensor_t *ctx, io_count_t *io_count);
int get_operating_stat      (sensor_t *ctx, operating_stat_t *stat);
int get_calibration_date    (sensor_t *ctx, calendar_t *calendar);
int get_manufacturing_date  (sensor_t *ctx, calendar_t *calendar);

int set_device_name         (sensor_t *ctx, device_name_t name);
int set_current_time        (sensor_t *ctx, data_time_t *time);
int set_interrupt_control   (sensor_t *ctx, interrupt_control_t control);

int wait_for_device_ready   (sensor_t *ctx, int max_wait_msec);
int soft_reset              (sensor_t *ctx);
int preset_config           (sensor_t *ctx);

#endif //OMEGA_SMARTSENSOR_C_SMARTSENSOR_H
