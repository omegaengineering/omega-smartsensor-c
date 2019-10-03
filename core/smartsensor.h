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

#include "common/common.h"
#include "common/errors.h"
#include "registers.h"


#define SMARTSENSOR_I2C_ADDR            0x68
#define SMARTSENSOR_MODBUS_ADDR         0x01
#define SMARTSENSOR_MODBUS_BAUDRATE     38400
#define DEFAULT_SAMPLE_TIME             1
#define HEARTBEAT                       0
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
    SENSOR_BUS_I2C,                             /**< type of bus_id is I2C bus */
    SENSOR_BUS_MODBUS                           /**< type of bus_id is Uart Modbus */
} sensor_bus_type_t;

typedef struct {
    char                bus_id[128];            /**< bus name string eg: /dev/ttyAMC0 */
    sensor_bus_type_t   bus_type;               /**< bus type @sensor_bus_type_t */
    int                 interrupt_pin;          /**< gpio pin to receive interrupts */
    event_callback_t    event_callback;         /**< user callback, pass NULL to disable interrupt processing */
    void*               event_callback_ctx;     /**< user provided data pointer to be passed into callback, pass NULL to disable */
    int                 heartbeat_period;       /**< heartbeat period to check for sensor online, pass 0 to disable */
} sensor_init_t;

/**
 * Create an instance of smartsensor device
 * @param ctx instance to create
 * @param init pointer to init structure. User owns this memory and it is no longer needed
 * after the function returns
 * @return see @error_t
 */
int sensor_new              (sensor_t **ctx, const sensor_init_t *init);

/**
 * Open the communicating bus and initialize the device
 * @param ctx sensor instance
 * @return see @error_t
 */
int sensor_open             (sensor_t *ctx);

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
int sensor_read             (sensor_t *ctx, ss_register_t ss_register, data_buffer_t * buffer);

/**
 * Read register with the specified instance index into provided data buffer
 * Eg:  SENSOR_0_DATA can have 4 indexed instances which are SENSOR_(0 1 2 3)_DATA
 *      SENSOR_1_DATA can have 3 indexed instances which are SENSOR(1 2 3)_DATA
 *
 *      - data buffer must be large enough to hold data size of the register, otherwise
 *          E_BUFFER_MEM_SIZE is returned.
 *      - Only actual data size of the register are read into the provided buffer.
 * @param ctx sensor instance
 * @param ss_register register base to read
 * @param index instance index
 * @param buffer buffer data to read into
 * @return see @error_t
 */
int sensor_indexed_read     (sensor_t *ctx, ss_register_t ss_register, uint8_t index, data_buffer_t * buffer);

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
int sensor_write            (sensor_t *ctx, ss_register_t ss_register, data_buffer_t * buffer);

/**
 * Write register with the specified instance index from provided data buffer
 * Eg:  SENSOR_0_DATA can have 4 indexed instances which are SENSOR_(0 1 2 3)_DATA
 *      SENSOR_1_DATA can have 3 indexed instances which are SENSOR(1 2 3)_DATA
 *
 *      - data buffer must be large enough to hold data size of the register, otherwise
 *          E_BUFFER_MEM_SIZE is returned.
 *      - Only actual data size of the register are read from the buffer and written to sensor.
 * @param ctx sensor instance
 * @param ss_register register to write
 * @param index instance index
 * @param buffer buffer to write from
 * @return see @error_t
 */
int sensor_indexed_write    (sensor_t *ctx, ss_register_t ss_register, uint8_t index, data_buffer_t * buffer);

/**
 * Close the communicating bus and deinitialize the device
 * @param ctx sensor instance
 * @return see @error_t
 */
int sensor_close            (sensor_t *ctx);

/**
 * Free the sensor instance
 * @param ctx sensor instance
 * @return see @error_t
 */
int sensor_free             (sensor_t *ctx);

/**
 * Get the number of possible instances to index from a base register
 * Eg:  SENSOR_0_DATA can have 4 indexed instances which are SENSOR_(0 1 2 3)_DATA
 *      SENSOR_1_DATA can have 3 indexed instances which are SENSOR(1 2 3)_DATA
 * @param ss_register
 * @return
 */
int get_max_instance        (ss_register_t ss_register);

/**
 * Get sensor reading
 * @param ctx sensor instance
 * @param sensor_num
 * @return see @error_t
 */
int get_sensor_reading      (sensor_t *ctx, int sensor_num, const float *reading);

/**
 * Get sensor gain
 * @param ctx sensor instance
 * @param sensor_num
 * @return see @error_t
 */
int get_sensor_gain         (sensor_t *ctx, int sensor_num, const float *gain);

/**
 * Get sensor offset
 * @param ctx sensor instance
 * @param sensor_num
 * @return see @error_t
 */
int get_sensor_offset       (sensor_t *ctx, int sensor_num, const float *offset);

/**
 * Get sensor name
 * @param ctx sensor instance
 * @return see @error_t
 */
int get_device_name         (sensor_t *ctx, device_name_t name);

/**
 * Get sensor unit
 * @param ctx sensor instance
 * @return see @error_t
 */
int get_sensor_unit         (sensor_t *ctx, int sensor_num, sensor_unit_t unit);

/**
 * Get sensor descriptor
 * @param ctx sensor instance
 * @return see @error_t
 */
int get_sensor_descriptor   (sensor_t *ctx, int sensor_num, sensor_descriptor_t *descriptor);

/**
 * Get sensor type
 * @param ctx sensor instance
 * @return see @error_t
 */
int get_sensor_type         (sensor_t *ctx, int sensor_num, measurement_type_t *sensor_type);

/**
 * Get system status
 * @param ctx sensor instance
 * @return see @error_t
 */
int get_system_status       (sensor_t *ctx, system_status_t *status);

/**
 * Get interrupt status
 * @param ctx sensor instance
 * @return see @error_t
 */
int get_interrupt_status    (sensor_t *ctx, interrupt_status_t *status);

/**
 * Get sensor time
 * @param ctx sensor instance
 * @return see @error_t
 */
int get_current_time        (sensor_t *ctx, data_time_t *time);

/**
 * Get input/output count
 * @param ctx sensor instance
 * @return see @error_t
 */
int get_io_count            (sensor_t *ctx, io_count_t *io_count);

/**
 * Get operating stat
 * @param ctx sensor instance
 * @return see @error_t
 */
int get_operating_stat      (sensor_t *ctx, operating_stat_t *stat);

/**
 * Get calibration date
 * @param ctx sensor instance
 * @return see @error_t
 */
int get_calibration_date    (sensor_t *ctx, calendar_t *calendar);

/**
 * Get manufacturing date
 * @param ctx sensor instance
 * @return see @error_t
 */
int get_manufacturing_date  (sensor_t *ctx, calendar_t *calendar);

/**
 * Set device name
 * @param ctx sensor instance
 * @return see @error_t
 */
int set_device_name         (sensor_t *ctx, device_name_t name);

/**
 * Set current time
 * @param ctx sensor instance
 * @return see @error_t
 */
int set_current_time        (sensor_t *ctx, data_time_t *time);

/**
 * Set interrupt control
 * @param ctx sensor instance
 * @return see @error_t
 */
int set_interrupt_control   (sensor_t *ctx, interrupt_control_t control);

/**
 * Poll for device ready by checking device ready bit of system status register
 * @param ctx sensor instance
 * @return see @error_t
 */
int wait_for_device_ready   (sensor_t *ctx, int max_wait_msec);

/**
 * Do soft reset
 * @param ctx sensor instance
 * @return see @error_t
 */
int soft_reset              (sensor_t *ctx);

/**
 * Configure sensor to default configuration
 * @param ctx sensor instance
 * @return see @error_t
 */
int preset_config           (sensor_t *ctx);

#endif //OMEGA_SMARTSENSOR_C_SMARTSENSOR_H
