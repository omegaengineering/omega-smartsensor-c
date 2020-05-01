#include <stdint.h>
#include <string.h>
#include "registers.h"
#include "i2c_registers.h"

static const _register_t _def [] =
{
    // register             Modbus      I2C     Access  Size
    [DEVICE_ID]                 =   {0xf000,    R_DEVICE_ID,            1, RD,     sizeof(uint32_t)},
    [FIRMARE_VERSION]           =   {0xf002,    R_VERSION,              1, RD,     sizeof(uint32_t)},
    [HARDWARE_VERSION]          =   {0xf004,    R_HARDWARE_VERSION,     1, RD,     sizeof(uint32_t)},
    [LIST_INDEX_BLOCK_SELECT]   =   {0xf006,    R_DEVICE_LIST_INDEX,    1, RD|BYTES,  sizeof(list_select_t)},
    [USER_HOURS]                =   {0xf007,    R_USER_HOURS,           1, RD|WR,  sizeof(uint16_t)},

    [EVENT_0_TIME_BASE]         =   {0xf008,    R_EVENT_0_TIMEBASE,     1, RD|WR,  sizeof(uint16_t)},
    [EVENT_1_TIME_BASE]         =   {0xf009,    R_EVENT_1_TIMEBASE,     1, RD|WR,  sizeof(uint16_t)},
    [SYSTEM_CONTROL]            =   {0xf00a,    R_SYSTEM_CONTROL,       1, RD|WR,  sizeof(uint16_t)},
    [INTERRUPT_STATUS]          =   {0xf00b,    R_INTERRUPT_STATUS,     1, RD|WR,  sizeof(uint16_t)},
    [INTERRUPT_CONTROL]         =   {0xf00c,    R_INTERRUPT_CONTROL,    1, RD|WR,  sizeof(uint16_t)},

    [NUMBER_OF_SENSORS]         =   {0xf00d,    R_NUMOFSENSORS,         1, RD|BYTES,     sizeof(io_count_t)},
    [NUMBER_OF_OUTPUTS]         =   {0xf00d,    R_NUMOFOUTPUTS,         1, RD|BYTES,     sizeof(io_count_t)},
    [OPERATING_TEMP]            =   {0xf00e,    R_OPERATING_TEMP,       1, RD|BYTES,     sizeof(operating_stat_t)},
    [OPERATING_VOLTAGE]         =   {0xf00e,    R_OPERATING_TEMP,       1, RD|BYTES,     sizeof(operating_stat_t)},
    [FAULT_PROCESS]             =   {0xf00f,    R_FAULT_PROCESS,        1, RD|BYTES,     sizeof(fault_stat_t)},
    [FAULT_CODE]                =   {0xf00f,    R_FAULT_PROCESS,        1, RD|BYTES,     sizeof(fault_stat_t)},
    [EVENT_0_TIMER]             =   {0xf010,    R_EVENT_0_TIMER,        1, RD,     sizeof(uint16_t)},
    [EVENT_1_TIMER]             =   {0xf011,    R_EVENT_1_TIMER,        1, RD,     sizeof(uint16_t)},
    [SYSTEM_STATUS]             =   {0xf012,    R_SYSTEM_STATUS,        1, RD,     sizeof(uint16_t)},
    [TRIGGER_REQUESTS]          =   {0xf013,    R_TRIGGERS,             1, RD|WR,  sizeof(uint16_t)},

    [EXTRACT_START_TIME]        =   {0xf014,    R_EXTRACT_START_TIME,   1, RD|WR,  sizeof(uint16_t)},
    [EXTRACT_END_TIME]          =   {0xf015,    R_EXTRACT_END_TIME,     1, RD|WR,  sizeof(uint16_t)},

    [NUMBER_OF_RECORDS]         =   {0xf01b,    R_NUMBER_RECORDS,       1, RD,     sizeof(uint16_t)},
    [CURRENT_TIME]              =   {0xf01c,    R_CURRENT_TIME,         1, RD,  sizeof(uint32_t)},

    [SENSOR_0_DATA]             =   {0xf01e,    R_SENSOR_0_VALUE,       4, RD,  sizeof(float)},
    [SENSOR_1_DATA]             =   {0xf020,    R_SENSOR_1_VALUE,       3, RD,  sizeof(float)},
    [SENSOR_2_DATA]             =   {0xf022,    R_SENSOR_2_VALUE,       2, RD,  sizeof(float)},
    [SENSOR_3_DATA]             =   {0xf024,    R_SENSOR_3_VALUE,       1, RD,  sizeof(float)},

    [EXTRACTED_TIME_STAMP]      =   {0xf026,    R_EXTRACTED_TIME,       1, RD,  sizeof(uint32_t)},
    [EXTRACTED_DATA_0]          =   {0xf028,    R_EXTRACTED_0_VALUE,    4, RD,  sizeof(float)},
    [EXTRACTED_DATA_1]          =   {0xf02a,    R_EXTRACTED_1_VALUE,    3, RD,  sizeof(float)},
    [EXTRACTED_DATA_2]          =   {0xf02c,    R_EXTRACTED_2_VALUE,    2, RD,  sizeof(float)},
    [EXTRACTED_DATA_3]          =   {0xf02e,    R_EXTRACTED_3_VALUE,    1, RD,  sizeof(float)},

    [SENSOR_0_DESCRIPTOR]       =   {0xf030,    R_SENSOR_0_DESCRIPTOR,  4, RD|BYTES,  sizeof(sensor_descriptor_t)},
    [SENSOR_1_DESCRIPTOR]       =   {0xf034,    R_SENSOR_1_DESCRIPTOR,  3, RD|BYTES,  sizeof(sensor_descriptor_t)},
    [SENSOR_2_DESCRIPTOR]       =   {0xf038,    R_SENSOR_2_DESCRIPTOR,  2, RD|BYTES,  sizeof(sensor_descriptor_t)},
    [SENSOR_3_DESCRIPTOR]       =   {0xf03c,    R_SENSOR_3_DESCRIPTOR,  1, RD|BYTES,  sizeof(sensor_descriptor_t)},

    [USER_PARAMETER_0]          =   {0xf040,    R_USER_PARAMETER_0_VALUE,   16, RD|WR,  4},
    [USER_PARAMETER_1]          =   {0xf042,    R_USER_PARAMETER_1_VALUE,   15, RD|WR,  4},
    [USER_PARAMETER_2]          =   {0xf044,    R_USER_PARAMETER_2_VALUE,   14, RD|WR,  4},
    [USER_PARAMETER_3]          =   {0xf046,    R_USER_PARAMETER_3_VALUE,   13, RD|WR,  4},
    [USER_PARAMETER_4]          =   {0xf048,    R_USER_PARAMETER_4_VALUE,   12, RD|WR,  4},
    [USER_PARAMETER_5]          =   {0xf04a,    R_USER_PARAMETER_5_VALUE,   11, RD|WR,  4},
    [USER_PARAMETER_6]          =   {0xf04c,    R_USER_PARAMETER_6_VALUE,   10, RD|WR,  4},
    [USER_PARAMETER_7]          =   {0xf04e,    R_USER_PARAMETER_7_VALUE,   9, RD|WR,  4},
    [USER_PARAMETER_8]          =   {0xf050,    R_USER_PARAMETER_8_VALUE,   8, RD|WR,  4},
    [USER_PARAMETER_9]          =   {0xf052,    R_USER_PARAMETER_9_VALUE,   7, RD|WR,  4},
    [USER_PARAMETER_10]         =   {0xf054,    R_USER_PARAMETER_10_VALUE,  6, RD|WR,  4},
    [USER_PARAMETER_11]         =   {0xf056,    R_USER_PARAMETER_11_VALUE,  5, RD|WR,  4},
    [USER_PARAMETER_12]         =   {0xf058,    R_USER_PARAMETER_12_VALUE,  4, RD|WR,  4},
    [USER_PARAMETER_13]         =   {0xf05a,    R_USER_PARAMETER_13_VALUE,  3, RD|WR,  4},
    [USER_PARAMETER_14]         =   {0xf05c,    R_USER_PARAMETER_14_VALUE,  2, RD|WR,  4},
    [USER_PARAMETER_15]         =   {0xf05e,    R_USER_PARAMETER_15_VALUE,  1, RD|WR,  4},

    [SENSOR_0_GAIN]             =   {0xf060,    R_SENSOR_0_GAIN,        4, RD|WR,  sizeof(float)},
    [SENSOR_1_GAIN]             =   {0xf064,    R_SENSOR_1_GAIN,        3, RD|WR,  sizeof(float)},
    [SENSOR_2_GAIN]             =   {0xf068,    R_SENSOR_2_GAIN,        2, RD|WR,  sizeof(float)},
    [SENSOR_3_GAIN]             =   {0xf06c,    R_SENSOR_3_GAIN,        1, RD|WR,  sizeof(float)},

    [SENSOR_0_OFFSET]           =   {0xf062,    R_SENSOR_0_OFFSET,      4, RD|WR,  sizeof(float)},
    [SENSOR_1_OFFSET]           =   {0xf066,    R_SENSOR_1_OFFSET,      3, RD|WR,  sizeof(float)},
    [SENSOR_2_OFFSET]           =   {0xf06a,    R_SENSOR_2_OFFSET,      2, RD|WR,  sizeof(float)},
    [SENSOR_3_OFFSET]           =   {0xf06e,    R_SENSOR_3_OFFSET,      1, RD|WR,  sizeof(float)},

    [SENSOR_0_UNIT]             =   {0xf032,    R_SENSOR_0_UNITS,       4, RD|WR|BYTES,  sizeof(sensor_unit_t)-1},
    [SENSOR_1_UNIT]             =   {0xf036,    R_SENSOR_1_UNITS,       3, RD|WR|BYTES,  sizeof(sensor_unit_t)-1},
    [SENSOR_2_UNIT]             =   {0xf03a,    R_SENSOR_2_UNITS,       2, RD|WR|BYTES,  sizeof(sensor_unit_t)-1},
    [SENSOR_3_UNIT]             =   {0xf03e,    R_SENSOR_3_UNITS,       1, RD|WR|BYTES,  sizeof(sensor_unit_t)-1},

    [DEVICE_NAME]               =   {0xf070,    R_DEVICE_NAME,          1, RD|BYTES,     sizeof(device_name_t)-1},
    [OUTPUT_0]                  =   {0xf078,    R_OUTPUT_0_VALUE,       4, RD,  sizeof(float)},
    [OUTPUT_1]                  =   {0xf07a,    R_OUTPUT_1_VALUE,       3, RD,  sizeof(float)},
    [OUTPUT_2]                  =   {0xf07c,    R_OUTPUT_2_VALUE,       2, RD,  sizeof(float)},
    [OUTPUT_3]                  =   {0xf07e,    R_OUTPUT_3_VALUE,       1, RD,  sizeof(float)},

    [LONG_DEVICE_ID]            =   {0xf080,    F_DEVICE_ID,                    1, RD,  sizeof(uint64_t)},
    [CORE_VERSION]              =   {0xf084,    R_CORE_VERSION,                 1, RD,  sizeof(uint32_t)},
    [BLOCK_START_RETRY_COUNT]   =   {0xf086,    R_BLOCK_START_RETRY,            1, RD,  sizeof(uint16_t)},
    [RTC_CALIBRATION_CONTROL]   =   {0xf087,    R_RTC_TUNING,                   1, RD,  sizeof(uint16_t)},
    [FEATURE_BITS]              =   {0xf088,    R_FEATURE_BITS,                 1, RD,  sizeof(uint32_t)},
    [DEFAULT_EVENT_0_TIMEBASE]  =   {0xf08a,    R_DEFAULT_EVENT_0_TIMEBASE,     2, RD,  sizeof(uint16_t)},
    [DEFAULT_EVENT_1_TIMEBASE]  =   {0xf08b,    R_DEFAULT_EVENT_1_TIMEBASE,     1, RD,  sizeof(uint16_t)},
    [DEFAULT_SYSTEM_CONTROL]    =   {0xf08c,    R_DEFAULT_SYSTEM_CONTROL,       1, RD,  sizeof(uint16_t)},
    [DEFAULT_INTERRUPT_CONTROL] =   {0xf08d,    R_DEFAULT_INTERRUPT_CONTROL,    1, RD,  sizeof(uint16_t)},
    [SENSOR_LIST_INDEX]         =   {0xf08e,    R_DEVICE_NUMBER_INDEX,          1, RD,  sizeof(list_select_t)},
    [SENSOR_LIST_SELECT]        =   {0xf08e,    R_DEVICE_NUMBER_INDEX,          1, RD,  sizeof(list_select_t)},
    [SENSOR_0_ERROR_COUNT]      =   {0xf090,    R_SENSOR_0_ERRORS,              4, RD,  sizeof(uint16_t)},
    [SENSOR_1_ERROR_COUNT]      =   {0xf091,    R_SENSOR_1_ERRORS,              3, RD,  sizeof(uint16_t)},
    [SENSOR_2_ERROR_COUNT]      =   {0xf092,    R_SENSOR_2_ERRORS,              2, RD,  sizeof(uint16_t)},
    [SENSOR_3_ERROR_COUNT]      =   {0xf093,    R_SENSOR_3_ERRORS,              1, RD,  sizeof(uint16_t)},
    [MANUFACTURED_DATE]         =   {0xf094,    R_MANUFACTURED_DATE,            1, RD,  sizeof(uint16_t)}, ///todo bad i2c
    [CALIBRATION_DATE]          =   {0xf095,    R_CALIBRATION_DATE,             1, RD,  sizeof(uint16_t)}, ///todo bad i2c
    [OPERATING_TIME]            =   {0xf096,    R_TOTAL_OPERATING_TIME,         1, RD,  sizeof(uint32_t)},
    [CALIBRATION_TIME]          =   {0xf098,    R_TIME_SINCE_CALIBRATION,       1, RD,  sizeof(uint32_t)},
    [OUTPUT_0_CONFIG]           =   {0xf09a,    R_OUTPUT_0_DESCRIPTOR,          4, RD,  sizeof(uint16_t)},
    [OUTPUT_1_CONFIG]           =   {0xf09b,    R_OUTPUT_1_DESCRIPTOR,          3, RD,  sizeof(uint16_t)},
    [OUTPUT_2_CONFIG]           =   {0xf09c,    R_OUTPUT_2_DESCRIPTOR,          2, RD,  sizeof(uint16_t)},
    [OUTPUT_3_CONFIG]           =   {0xf09d,    R_OUTPUT_3_DESCRIPTOR,          1, RD,  sizeof(uint16_t)},
    [BASE_HARDWARE_TYPE]        =   {0xf09e,    R_BASE_TYPE,                    1, RD,  sizeof(uint16_t)},
    [EXTRACT_STOP_SIZE]         =   {0xf09f,    R_SENTINEL,                     1, RD,  sizeof(uint16_t)},
    [DEVICE_NAME_LIST]          =   {0xf0a0,    R_IO_DEVICE_LIST_NAMES,         1, RD,  sizeof(device_name_list_t)-1},

    [CALIBRATION_DATA]          =   {0xf100,    0x00,   1, RD,  4},
    [DEVICE_PARAMETER_DATA]     =   {0xf1f0,    0x00,   1, RD, 4*8},

    [FUNCTION_BLOCK_PROGRAM]    =   {0xf200,    0x00,   1, RD,  1 *512},
    [FUNCTION_BLOCK_PARAM]      =   {0xf300,    0x00,   1, RD,  4 *32},
    [FUNCTION_BLOCK_PARAM_TYPE] =   {0xf340,    0x00,   1, RD,  2 *32},
    [FUNCTION_BLOCK_STATUS]     =   {0xf360,    0x00,   1, RD,  2 *32},
    [OUTPUT_0_NAME_LIST]        =   {0xf380,    R_OUTPUT_0_NAMELIST,   4, RD,  1 *64},
    [OUTPUT_1_NAME_LIST]        =   {0xf3a0,    R_OUTPUT_1_NAMELIST,   3, RD,  1 *64},
    [OUTPUT_2_NAME_LIST]        =   {0xf3c0,    R_OUTPUT_2_NAMELIST,   2, RD,  1 *64},
    [OUTPUT_3_NAME_LIST]        =   {0xf3e0,    R_OUTPUT_3_NAMELIST,   1, RD,  1 *64},

    [SENSOR_0_NAME_LIST]        =   {0xf400,    R_SENSOR_0_NAMELIST,        1, RD,      164},
    [SENSOR_0_IPSO_TYPE]        =   {0xf454,    R_SENSOR_0_IPSO_TYPE,       1, RD,     sizeof(uint16_t)},
    [SENSOR_0_PRECISION]        =   {0xf455,    R_SENSOR_0_PRECISION,       1, RD,     sizeof(uint16_t)},
    [SENSOR_0_FUNCTION]         =   {0xf456,    0x00,                       1, RD,     sizeof(uint16_t)},
    [SENSOR_0_MIN_VALUE]        =   {0xf458,    R_SENSOR_0_MINVALUE,        1, RD,     sizeof(float)},
    [SENSOR_0_MAX_VALUE]        =   {0xf45a,    R_SENSOR_0_MAXVALUE,        1, RD,     sizeof(float)},
    [SENSOR_0_MIN_RANGE]        =   {0xf45c,    R_SENSOR_0_MINRANGE,        1, RD,     sizeof(float)},
    [SENSOR_0_MAX_RANGE]        =   {0xf45e,    R_SENSOR_0_MAXRANGE,        1, RD,     sizeof(float)},
    [SENSOR_0_PARAM_0_VALUE]    =   {0xf460,    R_SENSOR_0_PARAMETER_0,     1, RD|WR,  sizeof(float)},
    [SENSOR_0_PARAM_0_MIN]      =   {0xf462,    0x00,                       1, RD,     sizeof(float)},
    [SENSOR_0_PARAM_0_MAX]      =   {0xf464,    0x00,                       1, RD,     sizeof(float)},
    [SENSOR_0_PARAM_0_INCREMENT]=   {0xf466,    0x00,                       1, RD,     sizeof(float)},
    [SENSOR_0_PARAM_1_VALUE]    =   {0xf468,    R_SENSOR_1_PARAMETER_1,     1, RD|WR,  sizeof(float)},
    [SENSOR_0_PARAM_1_MIN]      =   {0xf46a,    0x00,                       1, RD,     sizeof(float)},
    [SENSOR_0_PARAM_1_MAX]      =   {0xf46c,    0x00,                       1, RD,     sizeof(float)},
    [SENSOR_0_PARAM_1_INCREMENT]=   {0xf46e,    0x00,                       1, RD,     sizeof(float)},
    [SENSOR_0_PARAM_2_VALUE]    =   {0xf470,    R_SENSOR_2_PARAMETER_1,     1, RD|WR,  sizeof(float)},
    [SENSOR_0_PARAM_2_MIN]      =   {0xf472,    0x00,                       1, RD,     sizeof(float)},
    [SENSOR_0_PARAM_2_MAX]      =   {0xf474,    0x00,                       1, RD,     sizeof(float)},
    [SENSOR_0_PARAM_2_INCREMENT]=   {0xf476,    0x00,                       1, RD,     sizeof(float)},
    [SENSOR_0_PARAM_3_VALUE]    =   {0xf478,    R_SENSOR_3_PARAMETER_1,     1, RD|WR,  sizeof(float)},
    [SENSOR_0_PARAM_3_MIN]      =   {0xf47a,    0x00,                       1, RD,     sizeof(float)},
    [SENSOR_0_PARAM_3_MAX]      =   {0xf47c,    0x00,                       1, RD,     sizeof(float)},
    [SENSOR_0_PARAM_3_INCREMENT]=   {0xf47e,    0x00,                       1, RD,     sizeof(float)},

    [DEVICE_EXTENSION_SPACE]    =   {0xf600,    0x00,                       1,  RD|WR,     2 * 128},
    [USER_EXTENSION_SPACE]      =   {0xf680,    0x00,                       1,  RD|WR,     1 * 256},
    [SENSOR_0_NAME]             =   {0xf700,    0x00,                       4,  RD|WR,     16},
    [SENSOR_1_NAME]             =   {0xf709,    0x00,                       3,  RD|WR,     16},
    [SENSOR_2_NAME]             =   {0xf710,    0x00,                       2,  RD|WR,     16},
    [SENSOR_3_NAME]             =   {0xf719,    0x00,                       1,  RD|WR,     16},
    [OUTPUT_0_NAME]             =   {0xf720,    0x00,                       4,  RD|WR,     16},
    [OUTPUT_1_NAME]             =   {0xf729,    0x00,                       3,  RD|WR,     16},
    [OUTPUT_2_NAME]             =   {0xf730,    0x00,                       2,  RD|WR,     16},
    [OUTPUT_3_NAME]             =   {0xf739,    0x00,                       1,  RD|WR,     16},
    [PARAM_0_NAME]              =   {0xf740,    R_USER_PARAMETER_0_NAME,    16, RD|WR,     16},
    [PARAM_1_NAME]              =   {0xf748,    R_USER_PARAMETER_1_NAME,    15, RD|WR,     16},
    [PARAM_2_NAME]              =   {0xf750,    R_USER_PARAMETER_2_NAME,    14, RD|WR,     16},
    [PARAM_3_NAME]              =   {0xf758,    R_USER_PARAMETER_3_NAME,    13, RD|WR,     16},
    [PARAM_4_NAME]              =   {0xf760,    R_USER_PARAMETER_4_NAME,    12, RD|WR,     16},
    [PARAM_5_NAME]              =   {0xf768,    R_USER_PARAMETER_5_NAME,    11, RD|WR,     16},
    [PARAM_6_NAME]              =   {0xf770,    R_USER_PARAMETER_6_NAME,    10, RD|WR,     16},
    [PARAM_7_NAME]              =   {0xf778,    R_USER_PARAMETER_7_NAME,    9,  RD|WR,     16},
    [PARAM_8_NAME]              =   {0xf780,    R_USER_PARAMETER_8_NAME,    8,  RD|WR,     16},
    [PARAM_9_NAME]              =   {0xf788,    R_USER_PARAMETER_9_NAME,    7,  RD|WR,     16},
    [PARAM_10_NAME]             =   {0xf790,    R_USER_PARAMETER_10_NAME,   6,  RD|WR,     16},
    [PARAM_11_NAME]             =   {0xf798,    R_USER_PARAMETER_11_NAME,   5,  RD|WR,     16},
    [PARAM_12_NAME]             =   {0xf7a0,    R_USER_PARAMETER_12_NAME,   4,  RD|WR,     16},
    [PARAM_13_NAME]             =   {0xf7a8,    R_USER_PARAMETER_13_NAME,   3,  RD|WR,     16},
    [PARAM_14_NAME]             =   {0xf7b0,    R_USER_PARAMETER_14_NAME,   2,  RD|WR,     16},
    [PARAM_15_NAME]             =   {0xf7b8,    R_USER_PARAMETER_15_NAME,   1,  RD|WR,     16},
    [CALIBRATION_STRING]        =   {0xf7e0,    0x00,                       1,  RD,        64},

};

const _register_t* get_register_entry(ss_register_t ss_register)
{
    if (ss_register >= LAST_REGISTER)
        return NULL;
    return &_def[ss_register];
}

uint16_t get_register_data_size(ss_register_t ss_register)
{
    return _def[ss_register].size;
}