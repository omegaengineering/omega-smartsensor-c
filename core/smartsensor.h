/*!********************************************************************************************
  @file     smartsensor.h

  @copyright
            Copyright (c) 2019, Omega Engineering Inc.

            Permission is hereby granted, free of charge, to any person obtaining
            a copy of this software and associated documentation files (the
            'Software'), to deal in the Software without restriction, including
            without limitation the rights to use, copy, modify, merge, publish,
            distribute, sublicense, and/or sell copies of the Software, and to
            permit persons to whom the Software is furnished to do so, subject to
            the following conditions:

            The above copyright notice and this permission notice shall be
            included in all copies or substantial portions of the Software.

            THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
            EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
            MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
            IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
            CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
            TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
            SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  @author   Binh Dinh
  @date     June 5th, 2019
  @details
            Contains public interface for the smartsensor API.
            This is the header file to be included by user application.

***********************************************************************************************/

#ifndef OMEGA_SMARTSENSOR_C_SMARTSENSOR_H
#define OMEGA_SMARTSENSOR_C_SMARTSENSOR_H

#include "registers.h"
#include "smartsensor_errors.h"

#define SMARTSENSOR_SDK_VERSION         0x00010005
#define SMARTSENSOR_I2C_ADDR            0x68
#define SMARTSENSOR_MODBUS_ADDR         0x01
#define SMARTSENSOR_MODBUS_BAUDRATE     38400

#ifndef DEFAULT_SAMPLE_TIME
#define DEFAULT_SAMPLE_TIME             1
#endif
#ifndef HEARTBEAT
#define HEARTBEAT                       1
#endif
#ifndef HEARTBEAT_MAX_MISS
#define HEARTBEAT_MAX_MISS              1
#endif
#ifndef FORCE_SAMPLE_TIME
#define FORCE_SAMPLE_TIME               1
#endif


typedef enum {
    API_SENSOR_CHANGE           = SENSOR_CHANGE_INTR,
    API_POWER_CHANGE            = POWER_CHANGE_INTR,
    API_HEALTH_CHANGE           = HEALTH_CHANGE_INTR,
    API_EVENT_0                 = EVENT_0_INTR,
    API_EVENT_1                 = EVENT_1_INTR,
    API_DATA_READY              = DATA_READY_INTR,
    API_FUNCTION_BLOCK          = FUNCTION_BLOCK_INTR,
    API_LOG_DATA_READY          = LOG_DATA_READY_INTR,
    API_EVENT_CONTINUE          = 0x00010000,           // no event
    API_EVENT_SENSOR_ATTACHED   = 0x00020000,
    API_EVENT_SENSOR_DETACHED   = 0x00040000,
} api_event_t;

typedef void (*event_callback_t)(api_event_t event, void *);

typedef enum {
    SENSOR_BUS_I2C,                             /**< type of bus_id is I2C bus */
    SENSOR_BUS_MODBUS                           /**< type of bus_id is Uart Modbus */
} sensor_bus_type_t;

typedef struct {
    uint8_t     sensor_attached;
    uint32_t    stat_attach_counter;
    uint32_t    heartbeat_misses;
} data_t;

typedef struct _sensor {
    sensor_bus_type_t   bus_type;           /**< type of bus that is being used */
    data_t              data;               /**< internal data */
    uint8_t             ready;              /**< flag to prevent interrupts from using the SDK if sensor has not been opened */
    void*               platform;
} sensor_t;

#define SENSOR_INIT     (sensor_t){0}

/**
 * Open the communicating bus and initialize the device
 * @param ctx sensor instance
 * @return see @error_t
 */
int sensor_open             (sensor_t* sensor);

int sensor_close            (sensor_t* sensor);

int sensor_poll_event       (sensor_t* sensor, api_event_t* event);

int sensor_heartbeat_enable (sensor_t* sensor, int period_ms);

int sensor_heartbeat_disable(sensor_t* sensor);

/**
 * Read register into provided data buffer
 *      - data buffer must be large enough to hold data size of the register, otherwise
 *          E_BUFFER_MEM_SIZE is returned.
 *      - Only actual data size of the register are read into the provided buffer.
 * @param ctx sensor instance
 * @param ss_register register to read
 * @param buffer data to read into
 * @return see @error_t
 */
int sensor_read             (sensor_t* sensor, ss_register_t ss_register, void* buffer, uint16_t buffer_sz);

/**
 * Read register with the specified instance index into provided data buffer
 * Eg:  SENSOR_0_DATA can have 4 indexed instances which are SENSOR_(0 1 2 3)_DATA
 *      SENSOR_1_DATA can have 3 indexed instances which are SENSOR(1 2 3)_DATA
 *
 *      - data buffer must be large enough to hold data size of the register, otherwise
 *          E_BUFFER_MEM_SIZE is returned.
 *      - Only actual data size of the register are read into the provided buffer.
 * @param ctx sensor instance
 * @param base_reg register base to read
 * @param index instance index
 * @param buffer buffer data to read into
 * @return see @error_t
 */
int sensor_indexed_read     (sensor_t* sensor, ss_register_t base_reg, uint8_t index, void* buffer, uint16_t buffer_sz);

/**
 * Write data provided from data buffer to register
 *
 *      - data buffer must be large enough to hold data size of the register, otherwise
 *          E_BUFFER_MEM_SIZE is returned.
 *      - Only actual data size of the register are read from the buffer and written to sensor.
 * @param ctx sensor instance
 * @param ss_register register to write
 * @param buffer buffer to write from
 * @return see @error_t
 */
int sensor_write            (sensor_t* sensor, ss_register_t ss_register, void* buffer, uint16_t buffer_sz);

/**
 * Write register with the specified instance index from provided data buffer
 * Eg:  SENSOR_0_DATA can have 4 indexed instances which are SENSOR_(0 1 2 3)_DATA
 *      SENSOR_1_DATA can have 3 indexed instances which are SENSOR(1 2 3)_DATA
 *
 *      - data buffer must be large enough to hold data size of the register, otherwise
 *          E_BUFFER_MEM_SIZE is returned.
 *      - Only actual data size of the register are read from the buffer and written to sensor.
 * @param ctx sensor instance
 * @param base_register register to write
 * @param index instance index
 * @param buffer buffer to write from
 * @return see @error_t
 */
int sensor_indexed_write    (sensor_t* sensor, ss_register_t base_register, uint8_t index, void* buffer, uint16_t buffer_sz);


/**
 * Get the number of possible instances to index from a base register.
 * To be used in indexed read/write parameter.
 * Eg:  SENSOR_0_DATA can have 4 indexed instances which are SENSOR_(0 1 2 3)_DATA
 *      SENSOR_1_DATA can have 3 indexed instances which are SENSOR(1 2 3)_DATA
 * @param ss_register
 * @return
 */
int get_register_instance_cnt (ss_register_t ss_register);


int get_sensor_reading      (sensor_t* sensor, int sensor_num, float *reading);
int get_sensor_gain         (sensor_t* sensor, int sensor_num, float *gain);
int get_sensor_offset       (sensor_t* sensor, int sensor_num, float *offset);
int get_sensor_unit         (sensor_t* sensor, int sensor_num, sensor_unit_t unit);
int get_sensor_descriptor   (sensor_t* sensor, int sensor_num, sensor_descriptor_t *descriptor);
int get_sensor_type         (sensor_t* sensor, int sensor_num, measurement_type_t *sensor_type);
int get_device_name         (sensor_t* sensor, device_name_t name);
int get_system_status       (sensor_t* sensor, system_status_t *status);
int get_interrupt_status    (sensor_t* sensor, interrupt_status_t *status);
int get_current_time        (sensor_t* sensor, sensor_time_t *time);
int get_io_count            (sensor_t* sensor, io_count_t *io_count);
int get_operating_stat      (sensor_t* sensor, operating_stat_t *stat);
int get_calibration_date    (sensor_t* sensor, sensor_date_t *date);
int get_manufacturing_date  (sensor_t* sensor, sensor_date_t *date);


int set_current_time        (sensor_t* sensor, sensor_time_t *time);
int set_interrupt_control   (sensor_t* sensor, interrupt_control_t control);
int set_sample_time         (sensor_t* sensor, uint16_t sample_time);
int get_sample_time         (sensor_t* sensor, uint16_t* sample_time);

int wait_for_device_ready   (sensor_t* sensor, int max_wait_msec);
int soft_reset              (sensor_t* sensor);
int factory_reset           (sensor_t* sensor);
int preset_config           (sensor_t* sensor);
unsigned int sensor_sdk_version();
const char* measurement_str (measurement_type_t meas);

int probe_default_init      (sensor_t* sensor);

int system_control_clear_bits(sensor_t* sensor, system_control_t bits);
int system_control_set_bits  (sensor_t* sensor, system_control_t bits);
int system_control_write_bits(sensor_t* sensor, system_control_t bits);

#endif //OMEGA_SMARTSENSOR_C_SMARTSENSOR_H
