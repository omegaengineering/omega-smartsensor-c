//
// Created by dev on 5/13/19.
//

#ifndef OMEGA_SMARTSENSOR_C_REGISTERS_H
#define OMEGA_SMARTSENSOR_C_REGISTERS_H


#include <stdint.h>

#define MAX_SENSOR_COUNT    4
#define MAX_OUTPUT_COUNT    4

typedef char sensor_unit_t[4  + 1];
typedef char device_name_t[32 + 1];

typedef struct {
    uint8_t list_select;
    uint8_t block_select;
} list_select_t;

typedef struct {
    uint8_t device_locked : 1;
    uint8_t factory_access: 1;
    uint8_t device_ready:   1;
    uint8_t health_fault:   1;
    uint8_t sensor_fault:   1;
    uint8_t read_active:    1;
    uint8_t extract_valid:  1;
    uint8_t sensor_valid:   1;
    uint8_t system_fault:   1;
    uint8_t intr_active:    1;
    uint8_t device_reset:   1;
    uint8_t power_reset:    1;
    uint8_t sensor_bits:    4;
} system_status_t;

typedef struct {
    uint8_t sensor_count;
    uint8_t output_count;
} io_count_t;

typedef struct {
    uint8_t operating_temp;
    uint8_t operating_voltage;
} operating_stat_t;

typedef struct {
    uint8_t fault_process;
    uint8_t fault_code;
} fault_stat_t;

typedef struct {
    uint8_t days;
    uint8_t hours;
    uint8_t mins;
    uint8_t secs;
} data_time_t;

typedef struct {
    uint8_t month;
    uint8_t day;
    uint16_t year;
} calendar_t;

typedef enum
{
    ENABLE_SENSOR_CHANGE_LOG    = 1 << 0,
    ENABLE_POWER_CHANGE_LOG     = 1 << 1,
    ENABLE_HEALTH_FAULT_LOG     = 1 << 2,
    ENABLE_TIME_CHANGE_LOG      = 1 << 3,
    ENABLE_EVENT_1_READ         = 1 << 4,
    ENABLE_EVENT_1_LOG          = 1 << 5,
    ENABLE_EVENT_2_READ         = 1 << 6,
    ENABLE_EVENT_2_LOG          = 1 << 7,
    ENABLE_EXTN_READ            = 1 << 8,
    ENABLE_EXTN_LOG             = 1 << 9,
    ENABLE_EXTN_RESET_EVENT_1   = 1 << 10,
    ENABLE_EXTN_RESET_EVENT_2   = 1 << 11,
    ENABLE_FUNCTION_BLOCK       = 1 << 12,
    ENABLE_HEALTH_MONITOR       = 1 << 13,
    ENABLE_LOG_OVERWRITE        = 1 << 14,
    ENABLE_RTC                  = 1 << 15,
} system_control_t;

typedef enum    TRIGGER_TAG
{
    TRIGGER_SEARCH_RECORD           = 0x0001,
    TRIGGER_EXTRACT_NEXT            = 0x0002,
    TRIGGER_CLEAR_LOG               = 0x0003,

    TRIGGER_DEVICE_RESET            = 0x0004,
    TRIGGER_FACTORY_RESET           = 0x0005,
    TRIGGER_POWER_RESET             = 0x0006,

    RESET_EVENT_0                   = 0x0010,
    TRIGGER_EVENT_0                 = 0x0020,
    TRIGGER_RESET_EVENT_0           = 0x0030,

    RESET_EVENT_1                   = 0x0040,
    TRIGGER_EVENT_1                 = 0x0080,
    TRIGGER_RESET_EVENT_1           = 0x00c0,

    TRIGGER_READ_SENSORS            = 0x0100,
    TRIGGER_READ_LOG_SENSORS        = 0x0200,

    TRIGGER_CLEAR_FB_STATUS         = 0x4000,
    TRIGGER_BOOT_STRAP              = 0x8000,       // Dual purpose - user can not trigger bootstrap, bit used to force stay awake
    TRIGGER_USER_STAY_AWAKE         = 0x8000,       // Forces processor to 'stay awake' during auxiliary function processing
} trigger_t;

typedef enum INTERRUPT_STATUS_TAG
{
    SENSOR_CHANGE_INTR          = 0x0001,       // Changes to sensor status
    POWER_CHANGE_INTR           = 0x0002,       // Power UP/DOWN change
    HEALTH_CHANGE_INTR          = 0x0004,       // Fault (Temp/Voltage) conditions
    EVENT_0_INTR                = 0x0008,       // Event Timer Expired
    EVENT_1_INTR                = 0x0010,       // Log Timer Expired
    DATA_READY_INTR             = 0x0020,       // Sensor Data Ready bit set

    FUNCTION_BLOCK_INTR         = 0x0040,       // Function Block Interrupt
    LOG_DATA_READY_INTR         = 0x0080,       // Data available in log file
} interrupt_status_t;

typedef enum INTERRUPT_CONTROL_TAG
{
    ENABLE_SENSOR_CHANGE_INTR   = 0x0001,       // Changes to sensors
    ENABLE_POWER_CHANGE_INTR    = 0x0002,       // Power UP/DOWN change
    ENABLE_HEALTH_CHANGE_INTR   = 0x0004,       // Fault (Temp/Voltage) conditions
    ENABLE_EVENT_0_INTR         = 0x0008,       // Event Timer will trigger interrupt
    ENABLE_EVENT_1_INTR         = 0x0010,       // Log Timer will trigger interrupt
    ENABLE_DATA_READY_INTR      = 0x0020,       // Sensor Data Ready will trigger interrupt

    ENABLE_FUNCTION_BLOCK_INTR  = 0x0040,       // Function Block Interrupt
    ENABLE_LOG_DATA_READY_INTR  = 0x0080,       // Data available in log file
} interrupt_control_t;

typedef enum    MEASUREMENT_TYPE_TAG
{
    SENSOR_NULL_TYPE        = 0x00,
    SENSOR_TEMPERATURE_TYPE = 0x01,
    SENSOR_HUMIDITY_TYPE    = 0x02,
    SENSOR_BAROMETRIC_TYPE  = 0x03,
    SENSOR_LIGHT_TYPE       = 0x04,

    // PROCESS TYPES
            SENSOR_VOLT_TYPE        = 0x10,        // Voltage
    SENSOR_MILLIVOLT_TYPE   = 0x11,
    SENSOR_AMP_TYPE         = 0x12,        // Current Loops
    SENSOR_MILLIAMP_TYPE    = 0x13,

    SENSOR_DIGITAL_TYPE     = 0x18,        // Any Digital Type
    SENSOR_DIGITAL_DIN_TYPE = 0x18,        // Digital Inputs
    SENSOR_RATE_TYPE        = 0x19,        // Rate (Hz)
    SENSOR_PULSE_WIDTH_TYPE = 0x1a,        // Width (usec) of pulse input
    SENSOR_PULSE_DELAY_TYPE = 0x1b,        // Delay (usec) between two inputs
    SENSOR_DUTY_CYCLE_TYPE  = 0x1c,        // PWM duty cycle (% pulse width)
    SENSOR_COUNT_TYPE       = 0x1d,        // Totalizers (CNT) - Up Counter
    SENSOR_UP_DOWN_COUNT_TYPE = 0x1e,      // Totalizers (CNT) - Up/Down Counter
    SENSOR_QUAD_COUNT_TYPE  = 0x1f,        // Totalizers (CNT) - Quad Counter
    SENSOR_DIGITAL_MASK     = 0xf8,        // Masks all valid Digital types

    SENSOR_TC_TYPE          = 0x20,
    SENSOR_RTD_TYPE         = 0x21,
    SENSOR_THERMISTOR_TYPE  = 0x22,
    SENSOR_IR_TYPE          = 0x23,
    SENSOR_CJC_TYPE         = 0x24,
    SENSOR_PH_TYPE          = 0x25,
    SENSOR_CO2_TYPE         = 0x26,

    SENSOR_PRESSURE_TYPE    = 0x28,

    SENSOR_ACCELERATION_TYPE= 0x29,
    SENSOR_INCLINATION_TYPE = 0x2a,

    SENSOR_SERIAL_TYPE      = 0x30,
    SENSOR_ADC_COUNTS_TYPE  = 0x31,

} __attribute__ ((__packed__)) Measurement_Type_t;


typedef struct Sensor_Descriptor_Format_Tag
{
    uint8_t     u8_Data_Type : 4;
    uint8_t     u8_Reserved : 1;
    uint8_t     u8_Factory_Calibrate : 1;
    uint8_t     u8_Configurable_Descriptor : 1;
    uint8_t     u8_Smartsensor : 1;
} Sensor_Descriptor_Format_t;

typedef struct  Sensor_Descriptor_Tag
{
    Measurement_Type_t	        e_Measurement_Type : 8;
    Sensor_Descriptor_Format_t  st_Format;
    uint8_t     		        u8_Config : 8;
    uint8_t     		        u8_Device : 8;
} Sensor_Descriptor_t;

typedef enum {
    DEVICE_ID,
    FIRMARE_VERSION,
    HARDWARE_VERSION,
    SENSOR_LIST_SELECT,
    BLOCK_OPTIONS,
    OPERATING_HOURS,
    EVENT_1_TIME_BASE,
    EVENT_2_TIME_BASE,
    SYSTEM_CONTROL,
    INTERRUPT_STATUS,
    INTERRUPT_CONTROL,
    NUMBER_OF_SENSORS,
    NUMBER_OF_OUTPUTS,
    OPERATING_TEMP,
    OPERATING_VOLTAGE,
    FAULT_PROCESS,
    FAULT_CODE,
    EVENT_1_TIMER,
    EVENT_2_TIMER,
    SYSTEM_STATUS,
    TRIGGER_REQUESTS,
    EXTRACT_START_TIME,
    EXTRACT_END_TIME,

    NUMBER_OF_RECORDS,
    CURRENT_TIME,
    SENSOR_1_DATA,
    SENSOR_2_DATA,
    SENSOR_3_DATA,
    SENSOR_4_DATA,
    EXTRACTED_TIME_STAMP,
    SENSOR_1_EXTRACTED_DATA,
    SENSOR_2_EXTRACTED_DATA,
    SENSOR_3_EXTRACTED_DATA,
    SENSOR_4_EXTRACTED_DATA,

    SENSOR_1_DESCRIPTOR,
    SENSOR_2_DESCRIPTOR,
    SENSOR_3_DESCRIPTOR,
    SENSOR_4_DESCRIPTOR,

    SENSOR_1_OFFSET,
    SENSOR_2_OFFSET,
    SENSOR_3_OFFSET,
    SENSOR_4_OFFSET,

    SENSOR_1_GAIN,
    SENSOR_2_GAIN,
    SENSOR_3_GAIN,
    SENSOR_4_GAIN,

    SENSOR_1_UNIT,
    SENSOR_2_UNIT,
    SENSOR_3_UNIT,
    SENSOR_4_UNIT,

    DEVICE_NAME,

    OUTPUT_1,
    OUTPUT_2,
    OUTPUT_3,
    OUTPUT_4,


    // Manufacturing Register / May Require unlock

    MFR_DEVICE_ID,
    MFR_HARDWARE_VERSION,
    MFR_CORE_FIRMWARE_VER,
    MFR_BLOCK_I2C_RETRY,
    MFR_RTC_CALIBRATION,
    MFR_FEATURE_BITS,
    MFR_DEFAULT_EVENT_1_TIME_BASE,
    MFR_DEFAULT_EVENT_2_TIME_BASE,
    MFR_SYSTEM_CONTROL,
    MFR_SYSTEM_INTERRUPT,
    MFR_SENSOR_LIST_INDEX,
    MFR_SENSOR_LIST_SELECT,

    MFR_ERROR_COUNT_1,
    MFR_ERROR_COUNT_2,
    MFR_ERROR_COUNT_3,
    MFR_ERROR_COUNT_4,
    MFR_MANUFACTURING_DATE,
    MFR_CALIBRATION_DATE,
    MFR_OPERATING_TIME,
    MFR_CALIBRATION_TIME,
    MFR_OUTPUT_CONFIG_1,
    MFR_OUTPUT_CONFIG_2,
    MFR_OUTPUT_CONFIG_3,
    MFR_OUTPUT_CONFIG_4,

} ss_register_t;

#endif //OMEGA_SMARTSENSOR_C_REGISTERS_H
