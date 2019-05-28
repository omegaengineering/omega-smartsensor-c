//
// Created by dev on 5/13/19.
//

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "smartsensor_private.h"
#include "platform/memory.h"
#include "common/log.h"
#include "hw/bus.h"
#include "port/port.h"


typedef struct {
    uint16_t    modbus_addr;
    uint16_t    i2c_addr;
    uint16_t    access;
    uint16_t    size;
} _register_t;

typedef enum {
    RD  = 1 << 0,
    WR  = 1 << 1,
    EXE = 1 << 2,

    BYTES = 1 << 3  // LSB format
} control_t;


static const _register_t _def [] =
{
    // register             Modbus      I2C     Access  Size
    [DEVICE_ID]                 =   {0xf000,    R_DEVICE_ID,            RD,     sizeof(uint32_t)},
    [FIRMARE_VERSION]           =   {0xf002,    R_VERSION,              RD,     sizeof(uint32_t)},
    [HARDWARE_VERSION]          =   {0xf004,    R_HARDWARE_VERSION,     RD,     sizeof(uint32_t)},
    [LIST_INDEX_BLOCK_SELECT]   =   {0xf006,    R_DEVICE_LIST_INDEX,    RD|BYTES,  sizeof(list_select_t)},
    [USER_HOURS]                =   {0xf007,    R_USER_HOURS,           RD|WR,  sizeof(uint16_t)},

    [EVENT_0_TIME_BASE]         =   {0xf008,    R_EVENT_0_TIMEBASE,     RD|WR,  sizeof(uint16_t)},
    [EVENT_1_TIME_BASE]         =   {0xf009,    R_EVENT_1_TIMEBASE,     RD|WR,  sizeof(uint16_t)},
    [SYSTEM_CONTROL]            =   {0xf00a,    R_SYSTEM_CONTROL,       RD|WR,  sizeof(uint16_t)},
    [INTERRUPT_STATUS]          =   {0xf00b,    R_INTERRUPT_STATUS,     RD|WR,  sizeof(uint16_t)},
    [INTERRUPT_CONTROL]         =   {0xf00c,    R_INTERRUPT_CONTROL,    RD|WR,  sizeof(uint16_t)},

    [NUMBER_OF_SENSORS]         =   {0xf00d,    R_NUMOFSENSORS,         RD|BYTES,     sizeof(io_count_t)},
    [NUMBER_OF_OUTPUTS]         =   {0xf00d,    R_NUMOFOUTPUTS,         RD|BYTES,     sizeof(io_count_t)},
    [OPERATING_TEMP]            =   {0xf00e,    R_OPERATING_TEMP,       RD|BYTES,     sizeof(operating_stat_t)},
    [OPERATING_VOLTAGE]         =   {0xf00e,    R_OPERATING_TEMP,       RD|BYTES,     sizeof(operating_stat_t)},
    [FAULT_PROCESS]             =   {0xf00f,    R_FAULT_PROCESS,        RD|BYTES,     sizeof(fault_stat_t)},
    [FAULT_CODE]                =   {0xf00f,    R_FAULT_PROCESS,        RD|BYTES,     sizeof(fault_stat_t)},
    [EVENT_0_TIMER]             =   {0xf010,    R_EVENT_0_TIMER,        RD,     sizeof(uint16_t)},
    [EVENT_1_TIMER]             =   {0xf011,    R_EVENT_1_TIMER,        RD,     sizeof(uint16_t)},
    [SYSTEM_STATUS]             =   {0xf012,    R_SYSTEM_STATUS,        RD|BYTES,     sizeof(uint16_t)},
    [TRIGGER_REQUESTS]          =   {0xf013,    R_TRIGGERS,             RD|WR,  sizeof(uint16_t)},

    [EXTRACT_START_TIME]        =   {0xf014,    R_EXTRACT_START_TIME,   RD|WR,  sizeof(uint16_t)},
    [EXTRACT_END_TIME]          =   {0xf015,    R_EXTRACT_END_TIME,     RD|WR,  sizeof(uint16_t)},

    [NUMBER_OF_RECORDS]         =   {0xf01b,    R_NUMBER_RECORDS,       RD,     sizeof(uint16_t)},
    [CURRENT_TIME]              =   {0xf01c,    R_CURRENT_TIME,         RD,  sizeof(uint32_t)},

    [SENSOR_0_DATA]             =   {0xf01e,    R_SENSOR_0_VALUE,       RD,  sizeof(float)},
    [SENSOR_1_DATA]             =   {0xf020,    R_SENSOR_1_VALUE,       RD,  sizeof(float)},
    [SENSOR_2_DATA]             =   {0xf022,    R_SENSOR_2_VALUE,       RD,  sizeof(float)},
    [SENSOR_3_DATA]             =   {0xf024,    R_SENSOR_3_VALUE,       RD,  sizeof(float)},

    [EXTRACTED_TIME_STAMP]      =   {0xf026,    R_EXTRACTED_TIME,       RD,  sizeof(uint32_t)},
    [EXTRACTED_DATA_0]          =   {0xf028,    R_EXTRACTED_0_VALUE,    RD,  sizeof(float)},
    [EXTRACTED_DATA_1]          =   {0xf02a,    R_EXTRACTED_1_VALUE,    RD,  sizeof(float)},
    [EXTRACTED_DATA_2]          =   {0xf02c,    R_EXTRACTED_2_VALUE,    RD,  sizeof(float)},
    [EXTRACTED_DATA_3]          =   {0xf02e,    R_EXTRACTED_3_VALUE,    RD,  sizeof(float)},

    [SENSOR_0_DESCRIPTOR]       =   {0xf030,    R_SENSOR_0_DESCRIPTOR,  RD,  sizeof(Sensor_Descriptor_t)},
    [SENSOR_1_DESCRIPTOR]       =   {0xf034,    R_SENSOR_1_DESCRIPTOR,  RD,  sizeof(Sensor_Descriptor_t)},
    [SENSOR_2_DESCRIPTOR]       =   {0xf038,    R_SENSOR_2_DESCRIPTOR,  RD,  sizeof(Sensor_Descriptor_t)},
    [SENSOR_3_DESCRIPTOR]       =   {0xf03c,    R_SENSOR_3_DESCRIPTOR,  RD,  sizeof(Sensor_Descriptor_t)},

    [USER_PARAMETER_0]          =   {0xf040,    R_USER_PARAMETER_0_VALUE,   RD|WR,  4},
    [USER_PARAMETER_1]          =   {0xf042,    R_USER_PARAMETER_1_VALUE,   RD|WR,  4},
    [USER_PARAMETER_2]          =   {0xf044,    R_USER_PARAMETER_2_VALUE,   RD|WR,  4},
    [USER_PARAMETER_3]          =   {0xf046,    R_USER_PARAMETER_3_VALUE,   RD|WR,  4},
    [USER_PARAMETER_4]          =   {0xf048,    R_USER_PARAMETER_4_VALUE,   RD|WR,  4},
    [USER_PARAMETER_5]          =   {0xf04a,    R_USER_PARAMETER_5_VALUE,   RD|WR,  4},
    [USER_PARAMETER_6]          =   {0xf04c,    R_USER_PARAMETER_6_VALUE,   RD|WR,  4},
    [USER_PARAMETER_7]          =   {0xf04e,    R_USER_PARAMETER_7_VALUE,   RD|WR,  4},
    [USER_PARAMETER_8]          =   {0xf050,    R_USER_PARAMETER_8_VALUE,   RD|WR,  4},
    [USER_PARAMETER_9]          =   {0xf052,    R_USER_PARAMETER_9_VALUE,   RD|WR,  4},
    [USER_PARAMETER_10]         =   {0xf054,    R_USER_PARAMETER_10_VALUE,  RD|WR,  4},
    [USER_PARAMETER_11]         =   {0xf056,    R_USER_PARAMETER_11_VALUE,  RD|WR,  4},
    [USER_PARAMETER_12]         =   {0xf058,    R_USER_PARAMETER_12_VALUE,  RD|WR,  4},
    [USER_PARAMETER_13]         =   {0xf05a,    R_USER_PARAMETER_13_VALUE,  RD|WR,  4},
    [USER_PARAMETER_14]         =   {0xf05c,    R_USER_PARAMETER_14_VALUE,  RD|WR,  4},
    [USER_PARAMETER_15]         =   {0xf05e,    R_USER_PARAMETER_15_VALUE,  RD|WR,  4},

    [SENSOR_0_GAIN]             =   {0xf060,    R_SENSOR_0_GAIN,        RD|WR,  sizeof(float)},
    [SENSOR_1_GAIN]             =   {0xf064,    R_SENSOR_1_GAIN,        RD|WR,  sizeof(float)},
    [SENSOR_2_GAIN]             =   {0xf068,    R_SENSOR_2_GAIN,        RD|WR,  sizeof(float)},
    [SENSOR_3_GAIN]             =   {0xf06c,    R_SENSOR_3_GAIN,        RD|WR,  sizeof(float)},

    [SENSOR_0_OFFSET]           =   {0xf062,    R_SENSOR_0_OFFSET,      RD|WR,  sizeof(float)},
    [SENSOR_1_OFFSET]           =   {0xf066,    R_SENSOR_1_OFFSET,      RD|WR,  sizeof(float)},
    [SENSOR_2_OFFSET]           =   {0xf06a,    R_SENSOR_2_OFFSET,      RD|WR,  sizeof(float)},
    [SENSOR_3_OFFSET]           =   {0xf06e,    R_SENSOR_3_OFFSET,      RD|WR,  sizeof(float)},

    [SENSOR_0_UNIT]             =   {0xf032,    R_SENSOR_0_UNITS,       RD|WR|BYTES,  sizeof(sensor_unit_t)-1},
    [SENSOR_1_UNIT]             =   {0xf036,    R_SENSOR_1_UNITS,       RD|WR|BYTES,  sizeof(sensor_unit_t)-1},
    [SENSOR_2_UNIT]             =   {0xf03a,    R_SENSOR_2_UNITS,       RD|WR|BYTES,  sizeof(sensor_unit_t)-1},
    [SENSOR_3_UNIT]             =   {0xf03e,    R_SENSOR_3_UNITS,       RD|WR|BYTES,  sizeof(sensor_unit_t)-1},

    [DEVICE_NAME]               =   {0xf070,    R_DEVICE_NAME,          RD|BYTES,     sizeof(device_name_t)-1},
    [OUTPUT_0]                  =   {0xf078,    R_OUTPUT_0_VALUE,       RD,  sizeof(float)},
    [OUTPUT_1]                  =   {0xf07a,    R_OUTPUT_1_VALUE,       RD,  sizeof(float)},
    [OUTPUT_2]                  =   {0xf07c,    R_OUTPUT_2_VALUE,       RD,  sizeof(float)},
    [OUTPUT_3]                  =   {0xf07e,    R_OUTPUT_3_VALUE,       RD,  sizeof(float)},

    [LONG_DEVICE_ID]            =   {0xf080,    0x00,   RD,  sizeof(uint64_t)},
    [CORE_VERSION]              =   {0xf084,    0x00,   RD,  sizeof(uint32_t)},
    [BLOCK_START_RETRY_COUNT]   =   {0xf086,    0x00,   RD,  sizeof(uint16_t)},
    [RTC_CALIBRATION_CONTROL]   =   {0xf087,    0x00,   RD,  sizeof(uint16_t)},
    [FEATURE_BITS]              =   {0xf088,    0x00,   RD,  sizeof(uint32_t)},
    [DEFAULT_EVENT_0_TIMEBASE]  =   {0xf08a,    0x00,   RD,  sizeof(uint16_t)},
    [DEFAULT_EVENT_1_TIMEBASE]  =   {0xf08b,    0x00,   RD,  sizeof(uint16_t)},
    [DEFAULT_SYSTEM_CONTROL]    =   {0xf08c,    0x00,   RD,  sizeof(uint16_t)},
    [DEFAULT_INTERRUPT_CONTROL] =   {0xf08d,    0x00,   RD,  sizeof(uint16_t)},
    [SENSOR_LIST_INDEX]         =   {0xf08e,    0x00,   RD,  sizeof(uint16_t)},
    [SENSOR_LIST_SELECT]        =   {0xf08f,    0x00,   RD,  sizeof(uint16_t)},
    [SENSOR_0_ERROR_COUNT]      =   {0xf090,    0x00,   RD,  sizeof(uint16_t)},
    [SENSOR_1_ERROR_COUNT]      =   {0xf091,    0x00,   RD,  sizeof(uint16_t)},
    [SENSOR_2_ERROR_COUNT]      =   {0xf092,    0x00,   RD,  sizeof(uint16_t)},
    [SENSOR_3_ERROR_COUNT]      =   {0xf093,    0x00,   RD,  sizeof(uint16_t)},
    [MANUFACTURED_DATE]         =   {0xf094,    0x00,   RD,  sizeof(uint16_t)}, ///todo bad i2c
    [CALIBRATION_DATE]          =   {0xf095,    0x00,   RD,  sizeof(uint16_t)}, ///todo bad i2c
    [OPERATING_TIME]            =   {0xf096,    0x00,   RD,  sizeof(uint32_t)},
    [CALIBRATION_TIME]          =   {0xf098,    0x00,   RD,  sizeof(uint32_t)},
    [OUTPUT_0_CONFIG]           =   {0xf09a,    0x00,   RD,  sizeof(uint16_t)},
    [OUTPUT_1_CONFIG]           =   {0xf09b,    0x00,   RD,  sizeof(uint16_t)},
    [OUTPUT_2_CONFIG]           =   {0xf09c,    0x00,   RD,  sizeof(uint16_t)},
    [OUTPUT_3_CONFIG]           =   {0xf09d,    0x00,   RD,  sizeof(uint16_t)},
    [BASE_HARDWARE_TYPE]        =   {0xf09e,    0x00,   RD,  sizeof(uint16_t)},
    [EXTRACT_STOP_SIZE]         =   {0xf09f,    0x00,   RD,  sizeof(uint16_t)},
    [DEVICE_NAME_LIST]          =   {0xf0a0,    0x00,   RD,  sizeof(device_name_list_t)-1},

    [CALIBRATION_DATA]          =   {0xf100,    0x00,   RD,  4},
    [DEVICE_PARAMETER_DATA]     =   {0xf1f0,    0x00,   RD, 4*8},

    [FUNCTION_BLOCK_PROGRAM]    =   {0xf200,    0x00,   RD,  1 *512},
    [FUNCTION_BLOCK_PARAM]      =   {0xf300,    0x00,   RD,  4 *32},
    [FUNCTION_BLOCK_PARAM_TYPE] =   {0xf340,    0x00,   RD,  2 *32},
    [FUNCTION_BLOCK_STATUS]     =   {0xf360,    0x00,   RD,  2 *32},
    [OUTPUT_0_NAME_LIST]        =   {0xf380,    0x00,   RD,  1 *64},
    [OUTPUT_1_NAME_LIST]        =   {0xf3a0,    0x00,   RD,  1 *64},
    [OUTPUT_2_NAME_LIST]        =   {0xf3c0,    0x00,   RD,  1 *64},
    [OUTPUT_3_NAME_LIST]        =   {0xf3e0,    0x00,   RD,  1 *64},

    [SENSOR_0_NAME_LIST]        =   {0xf400,    0x00,   RD,  164},
    [SENSOR_0_IPSO_TYPE]        =   {0xf454,    0x00,   RD,     sizeof(uint16_t)},
    [SENSOR_0_PRECISION]        =   {0xf455,    0x00,   RD,     sizeof(uint16_t)},
    [SENSOR_0_FUNCTION]         =   {0xf456,    0x00,   RD,     sizeof(uint16_t)},
    [SENSOR_0_MIN_VALUE]        =   {0xf458,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_MAX_VALUE]        =   {0xf45a,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_MIN_RANGE]        =   {0xf45c,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_MAX_RANGE]        =   {0xf45e,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_PARAM_0_VALUE]    =   {0xf460,    0x00,   RD|WR,  sizeof(float)},
    [SENSOR_0_PARAM_0_MIN]      =   {0xf462,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_PARAM_0_MAX]      =   {0xf464,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_PARAM_0_INCREMENT]=   {0xf466,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_PARAM_1_VALUE]    =   {0xf468,    0x00,   RD|WR,  sizeof(float)},
    [SENSOR_0_PARAM_1_MIN]      =   {0xf46a,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_PARAM_1_MAX]      =   {0xf46c,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_PARAM_1_INCREMENT]=   {0xf46e,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_PARAM_2_VALUE]    =   {0xf470,    0x00,   RD|WR,  sizeof(float)},
    [SENSOR_0_PARAM_2_MIN]      =   {0xf472,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_PARAM_2_MAX]      =   {0xf474,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_PARAM_2_INCREMENT]=   {0xf476,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_PARAM_3_VALUE]    =   {0xf478,    0x00,   RD|WR,  sizeof(float)},
    [SENSOR_0_PARAM_3_MIN]      =   {0xf47a,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_PARAM_3_MAX]      =   {0xf47c,    0x00,   RD,     sizeof(float)},
    [SENSOR_0_PARAM_3_INCREMENT]=   {0xf47e,    0x00,   RD,     sizeof(float)},

    [DEVICE_EXTENSION_SPACE]    =   {0xf600,    0x00,   RD|WR,     2 * 128},
    [USER_EXTENSION_SPACE]      =   {0xf680,    0x00,   RD|WR,     1 * 256},
    [SENSOR_0_NAME]             =   {0xf700,    0x00,   RD|WR,     16},
    [SENSOR_1_NAME]             =   {0xf709,    0x00,   RD|WR,     16},
    [SENSOR_2_NAME]             =   {0xf710,    0x00,   RD|WR,     16},
    [SENSOR_3_NAME]             =   {0xf719,    0x00,   RD|WR,     16},
    [OUTPUT_0_NAME]             =   {0xf720,    0x00,   RD|WR,     16},
    [OUTPUT_1_NAME]             =   {0xf729,    0x00,   RD|WR,     16},
    [OUTPUT_2_NAME]             =   {0xf730,    0x00,   RD|WR,     16},
    [OUTPUT_3_NAME]             =   {0xf739,    0x00,   RD|WR,     16},
    [PARAM_0_NAME]              =   {0xf740,    0x00,   RD|WR,     16},
    [PARAM_1_NAME]              =   {0xf748,    0x00,   RD|WR,     16},
    [PARAM_2_NAME]              =   {0xf750,    0x00,   RD|WR,     16},
    [PARAM_3_NAME]              =   {0xf758,    0x00,   RD|WR,     16},
    [PARAM_4_NAME]              =   {0xf760,    0x00,   RD|WR,     16},
    [PARAM_5_NAME]              =   {0xf768,    0x00,   RD|WR,     16},
    [PARAM_6_NAME]              =   {0xf770,    0x00,   RD|WR,     16},
    [PARAM_7_NAME]              =   {0xf778,    0x00,   RD|WR,     16},
    [PARAM_8_NAME]              =   {0xf780,    0x00,   RD|WR,     16},
    [PARAM_9_NAME]              =   {0xf788,    0x00,   RD|WR,     16},
    [PARAM_10_NAME]             =   {0xf790,    0x00,   RD|WR,     16},
    [PARAM_11_NAME]             =   {0xf798,    0x00,   RD|WR,     16},
    [PARAM_12_NAME]             =   {0xf7a0,    0x00,   RD|WR,     16},
    [PARAM_13_NAME]             =   {0xf7a8,    0x00,   RD|WR,     16},
    [PARAM_14_NAME]             =   {0xf7b0,    0x00,   RD|WR,     16},
    [PARAM_15_NAME]             =   {0xf7b8,    0x00,   RD|WR,     16},
    [CALIBRATION_STRING]        =   {0xf7e0,    0x00,   RD,        64},

};

s19_log_create("Sensor");

static void timeout_triggered(void *ctx);
static void run_user_callback(sensor_t * ctx, api_event_t event);


int sensor_new(sensor_t **ctx, const sensor_init_t *init)
{
    int ret;
    sensor_t *new_sensor = s19_mem_malloc(sizeof(sensor_t));
    if (new_sensor == NULL)
        return E_NO_MEM;

    memset(new_sensor, 0, sizeof(sensor_t));
    new_sensor->bus_type = init->bus_type;
    new_sensor->gpio_pin = init->gpio_id;
    new_sensor->event_callback = init->event_callback;
    new_sensor->event_callback_ctx = init->event_callback_ctx;
    new_sensor->heartbeat_period = init->heartbeat_period;

    if (new_sensor->bus_type == SENSOR_BUS_I2C) {
        if ((ret = port_bus_i2c_new(&new_sensor->bus)) != E_OK)
            goto ERROR;
        new_sensor->bus_cfg = (void*) port_bus_i2c_cfg;
    }
    else if (new_sensor->bus_type == SENSOR_BUS_MODBUS) {
        if ((ret = port_bus_modbus_new(&new_sensor->bus)) != E_OK)
            goto ERROR;
        new_sensor->bus_cfg = (void*) port_bus_modbus_cfg;
    }
    else {
        ret = E_NOT_SUPPORTED;
        goto ERROR;
    }
    strcpy(new_sensor->bus->bus_id, init->bus_id);  // do this after bus create !

    if ((ret = s19_mutex_create(&new_sensor->bus_lock)) != E_OK)
        goto ERROR;
    if ((ret = s19_mutex_create(&new_sensor->data_lock)) != E_OK)
        goto ERROR;

#if HEARTBEAT
    if (new_sensor->heartbeat_period)
        if ((ret = s19_timer_create(&new_sensor->timer) != E_OK))
            goto ERROR;
#endif
    *ctx = new_sensor;
    return E_OK;

ERROR:
    if (new_sensor) {
        if (new_sensor->bus)
            bus_free(new_sensor->bus);
        if (new_sensor->timer)
            s19_timer_destroy(new_sensor->timer);
        if (new_sensor->bus_lock)
            s19_mutex_destroy(new_sensor->bus_lock);
        if (new_sensor->data_lock)
            s19_mutex_destroy(new_sensor->data_lock);
        s19_mem_free(new_sensor);
    }
    *ctx = NULL;
    return ret;
}

int sensor_open(sensor_t *ctx)
{
    int ret;
    if ((ret = bus_open(ctx->bus, ctx->bus_cfg)) != E_OK)
        return ret;

    if (ctx->bus_type == SENSOR_BUS_I2C)
    {
        if ((ret = port_intr_init(ctx)) != E_OK)
            return ret;
        if ((ret = port_platform_init(ctx)) != E_OK)
            return ret;
    }
#if HEARTBEAT
    if (ctx->heartbeat_period)
    {
        if ((ret = s19_timer_entry_add(ctx->timer, &ctx->heartbeat) != E_OK))
            return ret;
        if ((ret = s19_timer_entry_set_period(ctx->heartbeat, ctx->heartbeat_period)) != E_OK)
            return ret;
        if ((ret = s19_timer_entry_set_ctx(ctx->heartbeat, ctx) != E_OK))
            return ret;
        if ((ret = s19_timer_entry_set_callback(ctx->heartbeat, timeout_triggered)) != E_OK)
            return ret;
    }
#endif
    ctx->ready = true;
    return E_OK;
}

static void reverse_bytes(data_buffer_t * data_buffer)
{
    for (int i = 0; i < data_buffer->data_len/2; i++)
    {
        uint8_t temp = data_buffer->data[i];
        data_buffer->data[i] = data_buffer->data[data_buffer->data_len - i-1];
        data_buffer->data[data_buffer->data_len - i-1] = temp;
    }
}

/*!*******************************************************************
static uint16_t    u16_SmartSensor_SetIndex(uint16_t u16_Register);

 @brief sets up indexed register based on selected u16_Register

 @param     u16_Register        Smart Sensor register to be accessed
 @returns   u16_Register        Adjusted register or error condition

 If u16_Register is < 0xff it is returned unmodified.
 if u16_Register is 0x100 .. 0x0fff it used to set the INDEX REGISTER and
 the correct ACCESS register is returned.

 If no error occurs the returned value will be in the range 0x0000 .. 0x00ff and
 may be used for subsequent read or write functions

 If u16_Register is > 0x0fff an E_REGISTER error is returned.

 Errors

 E_REGISTER     invalid register
 E_WRITE        Write request error during INDEX REGISTER write

*********************************************************************/

#define R_ACCESS_REGISTER           0x0030
#define R_FACTORY_ACCESS            0x0032
static int i2c_set_index(bus_t *ctx, uint16_t u16_Register_in, uint16_t *u16_Register_out)
{
    int ret;
    if (u16_Register_in > 0x0fff)
        return E_INVALID_PARAM;

    *u16_Register_out = u16_Register_in & 0xff;
    if (u16_Register_in > 0xff)
    {
        uint8_t buffer[3];
        buffer[0] = R_ACCESS_REGISTER;
        buffer[1] = u16_Register_in >> 8;
        buffer[2] = u16_Register_in;

        data_buffer_t data = {.data = buffer, .data_len = 3};
        if ((ret = bus_write_raw(ctx, &data) != E_OK))
            return ret;
        *u16_Register_out = ((u16_Register_in >> 10) + R_FACTORY_ACCESS);
    }
    return (E_OK);
}

int sensor_read(sensor_t *ctx, ss_register_t ss_register, data_buffer_t *data_buffer)
{
    int ret = E_API_ERROR;
    uint16_t reg_addr;
    const _register_t *reg = &_def[ss_register];

    if (data_buffer->data_len < reg->size)
    {
        ret = E_BUFFER_MEM_SIZE;
        goto ERROR;
    }
    s19_mutex_lock(ctx->bus_lock);

    switch (ctx->bus_type)
    {
        case SENSOR_BUS_I2C:
            reg_addr = reg->i2c_addr;
            if ((ret = i2c_set_index(ctx->bus, reg_addr, &reg_addr) != E_OK))
                goto ERROR;
            break;
        case SENSOR_BUS_MODBUS:
            reg_addr = reg->modbus_addr;
            break;
        default:
            ret = E_API_ERROR;
            goto ERROR;
    }
    // only read up to the actual data size
    data_buffer_t buf = {data_buffer->data, reg->size};
    ret = bus_read(ctx->bus, reg_addr, &buf);
    if (ret == E_OK && !(reg->access & BYTES))    // reverse I2C data (in MSB) to LSB format
        reverse_bytes(&buf);
    s19_mutex_unlock(ctx->bus_lock);
    return ret;

ERROR:
    s19_mutex_unlock(ctx->bus_lock);
    return ret;
}

int sensor_write(sensor_t *ctx, ss_register_t ss_register, data_buffer_t *data_buffer)
{
    int ret;
    uint16_t reg_addr;
    const _register_t *reg = &_def[ss_register];

    if (data_buffer->data_len < reg->size)
    {
        ret = E_BUFFER_MEM_SIZE;
        goto ERROR;
    }
    s19_mutex_lock(ctx->bus_lock);
    switch (ctx->bus_type)
    {
        case SENSOR_BUS_I2C:
            reg_addr = reg->i2c_addr;
            if ((ret = i2c_set_index(ctx->bus, reg_addr, &reg_addr) != E_OK))
                goto ERROR;
            break;
        case SENSOR_BUS_MODBUS:
            reg_addr = reg->modbus_addr;
            break;
        default:
            ret = E_API_ERROR;
            goto ERROR;
    }
    // only write up to the actual data size
    data_buffer_t buf = {data_buffer->data, reg->size};
    if (ret == E_OK && !(reg->access & BYTES))    // reverse I2C data (in MSB) to LSB format
        reverse_bytes(&buf);
    ret = bus_write(ctx->bus, reg_addr, &buf);
    s19_mutex_unlock(ctx->bus_lock);
    return ret;

ERROR:
    s19_mutex_unlock(ctx->bus_lock);
    return ret;
}

int sensor_close(sensor_t *ctx)
{
    ctx->shutdown_req = true;
    ctx->ready = false; // yes, again
    if (ctx->data.sensor_attached)
        run_user_callback(ctx, API_EVENT_SENSOR_DETACHED);  // let user know sensor is detached
    int ret = bus_close(ctx->bus);
    if (ctx->timer)
        s19_timer_entry_remove(ctx->heartbeat);
    if (ctx->bus_type == SENSOR_BUS_I2C)
        port_platform_exit(ctx);
    return ret;
}

int sensor_free(sensor_t *ctx)
{
    if (ctx)
    {
        if (ctx->bus)
            bus_free(ctx->bus);
        if (ctx->bus_lock)
            s19_mutex_destroy(ctx->bus_lock);
        if (ctx->data_lock)
            s19_mutex_destroy(ctx->data_lock);
        if (ctx->timer)
            s19_timer_destroy(ctx->timer);
        s19_mem_free(ctx);
    }
    return E_OK;
}

uint16_t get_register_data_size(ss_register_t ss_register)
{
    return _def[ss_register].size;
}

static int probe_init(sensor_t *ctx)
{
    int ret;
    uint16_t    data16;
    uint32_t    data32;
    data_buffer_t buffer16 = {.data = (uint8_t*) &data16, .data_len = sizeof(data16)};
    data_buffer_t buffer32 = {.data = (uint8_t*) &data32, .data_len = sizeof(data32)};

    // Ensure Interrupt Control register set correctly
    if ((ret = sensor_read(ctx, INTERRUPT_CONTROL, &buffer16)))
        return ret;

    data16 |=  (ENABLE_SENSOR_CHANGE_INTR |
                ENABLE_POWER_CHANGE_INTR |
                ENABLE_HEALTH_CHANGE_INTR |
                ENABLE_DATA_READY_INTR |
                ENABLE_FUNCTION_BLOCK_INTR |
                ENABLE_LOG_DATA_READY_INTR);

    if ((ret = sensor_write(ctx, INTERRUPT_CONTROL, &buffer16)))
        return ret;

    if ((ret = sensor_read(ctx, SYSTEM_CONTROL, &buffer16)))
        return ret;

    // set the CONTROL for the Device
    data16 |= ( ENABLE_SENSOR_CHANGE_LOG |
                ENABLE_POWER_CHANGE_LOG |
                ENABLE_HEALTH_FAULT_LOG |
                ENABLE_TIME_CHANGE_LOG |
                ENABLE_EVENT_1_READ |
                ENABLE_EVENT_1_LOG |
                ENABLE_FUNCTION_BLOCK |
                ENABLE_HEALTH_MONITOR |
                ENABLE_LOG_OVERWRITE |
                ENABLE_RTC);

    if ((ret = sensor_write(ctx, SYSTEM_CONTROL, &buffer16)))
        return ret;

    // Extract full span of data available
    data32 = 0;
    if ((ret = sensor_write(ctx, EXTRACT_START_TIME, &buffer32)))
        return ret;

    data32 = 0xffffffff;
    if ((ret = sensor_write(ctx, EXTRACT_END_TIME, &buffer32)))
        return ret;

    // data16 = SEARCH_RECORD_TRIGGER;
    data16 = TRIGGER_CLEAR_LOG;
    if ((ret = sensor_write(ctx, TRIGGER_REQUESTS, &buffer16)))
        return ret;

#ifdef FORCE_SAMPLE_TIME
    data16 = DEFAULT_SAMPLE_TIME;
    if ((ret = sensor_write(ctx, EVENT_0_TIME_BASE, &buffer16)))
        return ret;
    if ((ret = sensor_write(ctx, EVENT_1_TIME_BASE, &buffer16)))
        return ret;

#else
    if ((ret = sensor_read(ctx, EVENT_0_TIME_BASE, &buffer16)))
        return ret;

    if (data16 < DEFAULT_SAMPLE_TIME)
    {
        data16 = DEFAULT_SAMPLE_TIME;
        if ((ret = sensor_write(ctx, EVENT_0_TIME_BASE, &buffer16)))
            return ret;
    }
#endif
    return E_OK;
}

static void run_user_callback(sensor_t * ctx, api_event_t event)
{
    if (ctx->event_callback)
        ctx->event_callback(event, ctx->event_callback_ctx);
}

void miss_heartbeat(sensor_t * ctx)
{
    ctx->data.heartbeat_misses++;
    if (ctx->data.heartbeat_misses > HEARTBEAT_MAX_MISS)
    {
        ctx->data.sensor_attached = false;
        run_user_callback(ctx, API_EVENT_SENSOR_DETACHED);
        s19_log_dbg("Probe detached\n");
    }
}


static void resolve_intr_event(sensor_t * ctx)
{
    int ret;
    interrupt_status_t intr_status;
    ret = get_interrupt_status(ctx, &intr_status);
    if (ret == E_OK)
    {
        run_user_callback(ctx, (api_event_t) intr_status);
    }
    else
    {
        miss_heartbeat(ctx);
    }
}

static void do_probe_attach(sensor_t * ctx)
{
    int ret;
    ctx->data.stat_attach_counter++;
    ret = probe_init(ctx);
    s19_log_dbg("Probe attached %d\n", ret);
    if (ret == E_OK)
    {   // okay, new probe attached
        wait_for_device_ready(ctx, 1000);
        ctx->data.sensor_attached = true;
        ctx->data.heartbeat_misses = 0;
        run_user_callback(ctx, API_EVENT_SENSOR_ATTACHED);
    }
    else
        s19_log_warn("Probe attached failed with code %d\n", ret);
}

/// level 2 handling
static void event_handler_low_level(sensor_t *ctx, event_type_t intr)
{
    int ret;
    if (intr == SENSOR_EVT_HARD_INTR)
    {
        if (ctx->data.sensor_attached)
        {   // normal operation, attached and receiving interrupts
            resolve_intr_event(ctx);
        }
        else
        {
            system_status_t status;
            ret = get_system_status(ctx, &status);
            if (ret == E_OK && status.device_ready)
            {
                do_probe_attach(ctx);
            }
        }
    } else if (intr == SENSOR_EVT_TIMEOUT)
    {
        if (ctx->data.sensor_attached)
        {   // was attached, then check if still attached
            system_status_t status;
            ret = get_system_status(ctx, &status);
            if (ret != E_OK) {  // looks like no response
                miss_heartbeat(ctx);
            }
        } else {
            // wasn't attached, keep checking for device ready
            system_status_t status;
            ret = get_system_status(ctx, &status);
            if (ret == E_OK && status.device_ready)
            {
                do_probe_attach(ctx);
            }
        }
    }
}


/// level 1 signal
void hard_intr_triggered(sensor_t *ctx)
{
    if (ctx->ready)
    {
        //s19_log_dbg("Hard interrupt\n");
        event_handler_low_level(ctx, SENSOR_EVT_HARD_INTR);
    }
}

/// level 1 signal
static void timeout_triggered(void *ctx)
{
    //s19_log_dbg("Heartbeat timeout\n");
    event_handler_low_level(ctx, SENSOR_EVT_TIMEOUT);
}

#if ENABLE_UNIT_TEST
int get_def_table_size()
{
    return sizeof(_def);
}

_register_t* get_def_table()
{
    return (_register_t*) &_def[0];
}
#endif