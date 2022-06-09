/*!********************************************************************************************
  @file registers.h

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
            Contains enumerations for Smartsensor registers and structure declarations
            for various datatypes.

***********************************************************************************************/

#ifndef OMEGA_SMARTSENSOR_C_REGISTERS_H
#define OMEGA_SMARTSENSOR_C_REGISTERS_H


#include <stdint.h>

#if defined(__GNU__) || defined(__GNUC__)
#define GCC_PACKED __attribute__((packed))
    #define PACKED
#elif defined(__DCC__)
#define GCC_PACKED
    #define PACKED __packed__
#else
#define GCC_PACKED
#define PACKED packed
#endif

#define MAX_SENSOR_COUNT    4
#define MAX_OUTPUT_COUNT    4

#define SENSOR_0            0
#define SENSOR_1            1
#define SENSOR_2            2
#define SENSOR_3            3

typedef char sensor_unit_t[4  ];
typedef char sensor_name_t[16];
typedef char sensor_password_t[16];
typedef char device_name_t[32 ];
typedef char device_name_list_t[192 ];
typedef char device_model_name_t[16];

typedef struct {
    uint8_t list_select;
    uint8_t block_select;
} list_select_t;

typedef struct {
    uint8_t default_index;
    uint8_t max_index;
} io_list_t;

typedef struct {
    uint8_t sensor_bits:    4;
    uint8_t power_reset:    1;
    uint8_t device_reset:   1;
    uint8_t intr_active:    1;
    uint8_t system_fault:   1;
    uint8_t sensor_valid:   1;
    uint8_t extract_valid:  1;
    uint8_t read_active:    1;
    uint8_t sensor_fault:   1;
    uint8_t health_fault:   1;
    uint8_t device_ready:   1;
    uint8_t factory_access: 1;
    uint8_t device_locked : 1;
} GCC_PACKED system_status_t;

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
} sensor_time_t;

typedef struct {
    uint8_t month;
    uint8_t day;
    uint16_t year;
} sensor_date_t;

typedef enum
{
    ENABLE_SENSOR_CHANGE_LOG    = (1U << 0U),
    ENABLE_POWER_CHANGE_LOG     = (1U << 1U),
    ENABLE_HEALTH_FAULT_LOG     = (1U << 2U),
    ENABLE_TIME_CHANGE_LOG      = (1U << 3U),
    ENABLE_EVENT_1_READ         = (1U << 4U),
    ENABLE_EVENT_1_LOG          = (1U << 5U),
    ENABLE_EVENT_2_READ         = (1U << 6U),
    ENABLE_EVENT_2_LOG          = (1U << 7U),
    ENABLE_EXTN_READ            = (1U << 8U),
    ENABLE_EXTN_LOG             = (1U << 9U),
    ENABLE_INTERNAL_CLOCK_READ  = (1U << 10U), //
    ENABLE_LOG_TRACKING	        = (1U << 11U), // enable log tracking, if set, each time read, extract data will auto advance to next one
                                                // default behavnior does not advance pointer
    ENABLE_FUNCTION_BLOCK       = (1U << 12U),
    ENABLE_HEALTH_MONITOR       = (1U << 13U),
    ENABLE_LOG_OVERWRITE        = (1U << 14U),
    ENABLE_RTC                  = (1U << 15U),
} system_control_t;

enum    TRIGGER_TAG
{
    TRIGGER_EXTRACT                 = 0x0001,
    TRIGGER_EXTRACT_NEXT            = 0x0002,
    TRIGGER_CLEAR_LOG               = 0x0003,

    TRIGGER_DEVICE_RESET            = 0x0004,
    TRIGGER_FACTORY_RESET           = 0x0005,
    TRIGGER_POWER_RESET             = 0x0006,
    TRIGGER_PASSWORD_RESET          = 0x0007,
    TRIGGER_AUTHENTICATE            = 0x0008,

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
};

typedef struct {
    enum TRIGGER_TAG trigger: 16;
} GCC_PACKED trigger_t;

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
} GCC_PACKED interrupt_status_t;

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

    SENSOR_DIFFERENTIAL_PRESSURE_TYPE = 0x05,

    SENSOR_VOLT_TYPE        = 0x10,        // Voltage
    SENSOR_MILLIVOLT_TYPE   = 0x11,
    SENSOR_AMP_TYPE         = 0x12,        // Current Loops
    SENSOR_MILLIAMP_TYPE    = 0x13,

    SENSOR_DIO_TYPE         = 0x17,        // DIO type

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

    SENSOR_WEIGHT_TYPE      = 0x27,         // use this to report weight..

    SENSOR_PRESSURE_TYPE    = 0x28,

    SENSOR_ACCELEROMETER_TYPE   = 0x29,
    SENSOR_INCLINOMETER_TYPE    = 0x2a,
    SENSOR_MAGNETOMETER_TYPE    = 0x2b,
    SENSOR_SPEED_TYPE           = 0x2c,
    SENSOR_DISTANCE_TYPE        = 0x2d,

    SENSOR_SERIAL_TYPE          = 0x30,
    SENSOR_ADC_COUNTS_TYPE      = 0x31,

    SENSOR_MASS_CONCENTRATION   = 0x32,
    SENSOR_MASS_COUNT           = 0x33,
    SENSOR_PARTICLE_SIZE        = 0x34,

    SENSOR_HEATFLUX_TYPE        = 0x35,
    SENSOR_DEWPOINT_TYPE        = 0x36,
    SENSOR_CLAMPON_TYPE         = 0x37,

    SENSOR_TVOC_TYPE            = 0x38,

    SENSOR_LEAK_TYPE            = 0x39,
    SENSOR_HEATINDEX_TYPE       = 0x3a,
    SENSOR_HUMIDEX_TYPE         = 0x3b,

} measurement_type_t;

typedef enum {
    SENSOR_DATA_TYPE_U8     = 0,
    SENSOR_DATA_TYPE_S8     = 1,
    SENSOR_DATA_TYPE_U16    = 2,
    SENSOR_DATA_TYPE_S16    = 3,
    SENSOR_DATA_TYPE_U32    = 4,
    SENSOR_DATA_TYPE_S32    = 5,
    SENSOR_DATA_TYPE_FLOAT  = 6,
    SENSOR_DATA_TYPE_STR    = 7,
    SENSOR_DATA_TYPE_BOOL   = 8
} sensor_datatype_t;

typedef enum {
    OPEN_TC_DETECT_OFF = 0,
    OPEN_TC_DETECT_ON = 16,

} open_tc_detect_t;

typedef enum {
	TC_J = 0x00,
	TC_K = 0x01,
	TC_T = 0x02,
	TC_E = 0x03,
	TC_N = 0x04,
	TC_R = 0x06,
	TC_S = 0x07,
	TC_B = 0x08,
	TC_C = 0x09
}thermocouple_type_t;

typedef struct Sensor_Descriptor_Format_Tag
{
    sensor_datatype_t       u8_Data_Type : 4;
    uint8_t                 u8_Reserved : 1;
    uint8_t                 u8_Factory_Calibrate : 1;
    uint8_t                 u8_Configurable_Descriptor : 1;
    uint8_t                 u8_Smartsensor : 1;
} GCC_PACKED sensor_descriptor_format_t;

typedef struct Sensor_Config_Tag
{
    uint8_t     u8_Sensor_Range : 4;
    uint8_t     u8_Lock: 1;
    uint8_t     u8_Scaling: 1;
    uint8_t     u8_Channel: 1;
    uint8_t     u8_Reserved: 1;
} GCC_PACKED sensor_config_t;

typedef struct  Sensor_Descriptor_Tag
{
    measurement_type_t	        e_Measurement_Type: 8;      // 1 byte
    sensor_descriptor_format_t  st_Format;                  // 1 byte
    sensor_config_t     		st_Config;                  // 1 byte
    uint8_t     		        u8_Device;                  // 1 byte
    uint8_t                     au8_unit[4];                // 4 bytes
} GCC_PACKED sensor_descriptor_t;

typedef enum    EXTENSION_SPACE_STATUS_TAG
{
 VDC_LOW_THRESHOLD      = 4000,        // Assume battery power if VDC < 4000 mV
 BATTERY_MINIMUM        = 1800,        // <= 1.8 Vdc, treat as 0%
 BATTERY_CUTOUT         = 1600,        // < 1.6 then shut down completely
 BATTERY_MAXIMUM        = 3000,        // >= 3.0 Vdc, treat as 100 %

// Status fields
 BATTERY_POWERED        = 0x0001,      // if Set then Battery Powered, otherwise Line Powered
 BATTERY_LOW            = 0x0002,      // Set if Battery below BAT_MINIMUM
 SHUTDOWN_REQUIRED      = 0x0004,      // set if < SHUTDOWN voltage (1.6 volts)

 BATTERY_100            = 0x0080,      // Battery @ 100% capacity
 BATTERY_75             = 0x0040,
 BATTERY_50             = 0x0020,
 BATTERY_25             = 0x0010,      // Battery less than 25% capacity
 BATTERY_0              = 0x0000,      // No battery left
 BATTERY_CAPACITY_MASK  = 0x00f0,

 PROBE_ATTACHED          = 0x0100,      // probe has successfully attached
 AUTHENTICATION_FAULT    = 0x0200,      // Commissioning fault
 PASSWORD_FAULT          = 0x0400,      // Password fault detected
 ENUMERATING_PROBE       = 0x4000,      // re-purposed to tell gw we are using float values
 EXTENDED_VALID          = 0x8000,      // set iff PROBE_ATTACHED, AUTHENTICATION FAULT & PASSWORD FAULT are valid

} Extension_Space_Status_t;


// The following have been added to support the User Cal functions
typedef enum SENSOR_FUNCTION_TYPES
{
	RESET_IPSO_MINMAX               = 0x0001,
	SENSOR_CALIBRATE_START          = 0x0100,
	SENSOR_CAPTURE_LOW              = 0x0200,
	SENSOR_CAPTURE_HIGH             = 0x0400,
	SENSOR_CALIBRATE_MODE           = 0x0800,
	SENSOR_CALIBRATE_STATUS         = 0x1000,
	SENSOR_CALIBRATE_RESET          = 0x2000,   // reset the CAL to factory reset values
}Sensor_Function_Types_t;


typedef enum ss_register {
    /**< System Information */
    DEVICE_ID,
    FIRMARE_VERSION,
    HARDWARE_VERSION,
    LIST_INDEX_BLOCK_SELECT,
    USER_HOURS,

    /**< Control Functions */
    EVENT_0_TIME_BASE,
    EVENT_1_TIME_BASE,
    SYSTEM_CONTROL,                         // system_control_t
    INTERRUPT_STATUS,
    INTERRUPT_CONTROL,

    /**< Monitoring and Control */
    NUMBER_OF_SENSORS,
    NUMBER_OF_OUTPUTS,
    OPERATING_TEMP,
    OPERATING_VOLTAGE,
    FAULT_PROCESS,
    FAULT_CODE,
    EVENT_0_TIMER,
    EVENT_1_TIMER,
    SYSTEM_STATUS,
    TRIGGER_REQUESTS,                       // trigger_t

    /**< Data Extraction */
    EXTRACT_START_TIME,
    EXTRACT_END_TIME,

    /**< Extended Access */
    NUMBER_OF_RECORDS,

    /**< Sensor Readings */
    CURRENT_TIME,
    SENSOR_0_DATA,
    SENSOR_1_DATA,
    SENSOR_2_DATA,
    SENSOR_3_DATA,

    /**< Data Extraction */
    EXTRACTED_TIME_STAMP,
    EXTRACTED_DATA,             // index 4

    /**< Sensor Descriptors */
    SENSOR_0_DESCRIPTOR,
    SENSOR_1_DESCRIPTOR,
    SENSOR_2_DESCRIPTOR,
    SENSOR_3_DESCRIPTOR,
    SENSOR_UNIT,

    /**< User Parameters */
    USER_PARAMETER_0,
    USER_PARAMETER_1,
    USER_PARAMETER_2,
    USER_PARAMETER_3,
    USER_PARAMETER_4,
    USER_PARAMETER_5,
    USER_PARAMETER_6,
    USER_PARAMETER_7,
    USER_PARAMETER_8,
    USER_PARAMETER_9,
    USER_PARAMETER_10,
    USER_PARAMETER_11,
    USER_PARAMETER_12,
    USER_PARAMETER_13,
    USER_PARAMETER_14,
    USER_PARAMETER_15,

    SENSOR_OFFSET,  // indexed 4
    SENSOR_GAIN,    // indexed 4

    /**< Name Strings */
    DEVICE_NAME,
    OUTPUT_0,
    OUTPUT_1,
    OUTPUT_2,
    OUTPUT_3,

    /**< Factory Register Space */
    LONG_DEVICE_ID,
    CORE_VERSION,
    BLOCK_START_RETRY_COUNT,
    RTC_CALIBRATION_CONTROL,
    FEATURE_BITS,
    DEFAULT_EVENT_0_TIMEBASE,
    DEFAULT_EVENT_1_TIMEBASE,
    DEFAULT_SYSTEM_CONTROL,
    DEFAULT_INTERRUPT_CONTROL,
    SENSOR_LIST_INDEX,
    SENSOR_LIST_SELECT,
    IO_LIST_INDEX,
    IO_LIST_SELECT,
    SENSOR_0_ERROR_COUNT,
    SENSOR_1_ERROR_COUNT,
    SENSOR_2_ERROR_COUNT,
    SENSOR_3_ERROR_COUNT,
    MANUFACTURED_DATE,
    CALIBRATION_DATE,
    OPERATING_TIME,
    CALIBRATION_TIME,
    OUTPUT_0_CONFIG,
    OUTPUT_1_CONFIG,
    OUTPUT_2_CONFIG,
    OUTPUT_3_CONFIG,
    BASE_HARDWARE_TYPE,
    EXTRACT_STOP_SIZE,
    DEVICE_NAME_LIST,

	CONFIG_PASSWORD,
	CONNECTIVITY_PASSWORD,

    /**< Factory Calibration */
    CALIBRATION_DATA,
    DEVICE_PARAMETER_DATA,

    /**< Function Block Program space */
    FUNCTION_BLOCK_PROGRAM,
    FUNCTION_BLOCK_PROGRAM_0,
    FUNCTION_BLOCK_PROGRAM_1,
    FUNCTION_BLOCK_PROGRAM_2,

    FUNCTION_BLOCK_PARAM,
    FUNCTION_BLOCK_PARAM_TYPE,
    FUNCTION_BLOCK_STATUS,
    OUTPUT_0_NAME_LIST,
    OUTPUT_1_NAME_LIST,
    OUTPUT_2_NAME_LIST,
    OUTPUT_3_NAME_LIST,

    /**< Attached Sensor Space */
    SENSOR_0_NAME_LIST,
    SENSOR_0_IPSO_TYPE,
    SENSOR_0_PRECISION,
    SENSOR_0_FUNCTION,
    SENSOR_0_MIN_VALUE,
    SENSOR_0_MAX_VALUE,
    SENSOR_0_MIN_RANGE,
    SENSOR_0_MAX_RANGE,
    SENSOR_0_PARAM_0_VALUE,
    SENSOR_0_PARAM_0_MIN,
    SENSOR_0_PARAM_0_MAX,
    SENSOR_0_PARAM_0_INCREMENT,
    SENSOR_0_PARAM_1_VALUE,
    SENSOR_0_PARAM_1_MIN,
    SENSOR_0_PARAM_1_MAX,
    SENSOR_0_PARAM_1_INCREMENT,
    SENSOR_0_PARAM_2_VALUE,
    SENSOR_0_PARAM_2_MIN,
    SENSOR_0_PARAM_2_MAX,
    SENSOR_0_PARAM_2_INCREMENT,
    SENSOR_0_PARAM_3_VALUE,
    SENSOR_0_PARAM_3_MIN,
    SENSOR_0_PARAM_3_MAX,
    SENSOR_0_PARAM_3_INCREMENT,

    SENSOR_1_NAME_LIST,
    SENSOR_1_IPSO_TYPE,
    SENSOR_1_PRECISION,
    SENSOR_1_FUNCTION,
    SENSOR_1_MIN_VALUE,
    SENSOR_1_MAX_VALUE,
    SENSOR_1_MIN_RANGE,
    SENSOR_1_MAX_RANGE,
    SENSOR_1_PARAM_0_VALUE,
    SENSOR_1_PARAM_0_MIN,
    SENSOR_1_PARAM_0_MAX,
    SENSOR_1_PARAM_0_INCREMENT,
    SENSOR_1_PARAM_1_VALUE,
    SENSOR_1_PARAM_1_MIN,
    SENSOR_1_PARAM_1_MAX,
    SENSOR_1_PARAM_1_INCREMENT,
    SENSOR_1_PARAM_2_VALUE,
    SENSOR_1_PARAM_2_MIN,
    SENSOR_1_PARAM_2_MAX,
    SENSOR_1_PARAM_2_INCREMENT,
    SENSOR_1_PARAM_3_VALUE,
    SENSOR_1_PARAM_3_MIN,
    SENSOR_1_PARAM_3_MAX,
    SENSOR_1_PARAM_3_INCREMENT,

    SENSOR_2_NAME_LIST,
    SENSOR_2_IPSO_TYPE,
    SENSOR_2_PRECISION,
    SENSOR_2_FUNCTION,
    SENSOR_2_MIN_VALUE,
    SENSOR_2_MAX_VALUE,
    SENSOR_2_MIN_RANGE,
    SENSOR_2_MAX_RANGE,
    SENSOR_2_PARAM_0_VALUE,
    SENSOR_2_PARAM_0_MIN,
    SENSOR_2_PARAM_0_MAX,
    SENSOR_2_PARAM_0_INCREMENT,
    SENSOR_2_PARAM_1_VALUE,
    SENSOR_2_PARAM_1_MIN,
    SENSOR_2_PARAM_1_MAX,
    SENSOR_2_PARAM_1_INCREMENT,
    SENSOR_2_PARAM_2_VALUE,
    SENSOR_2_PARAM_2_MIN,
    SENSOR_2_PARAM_2_MAX,
    SENSOR_2_PARAM_2_INCREMENT,
    SENSOR_2_PARAM_3_VALUE,
    SENSOR_2_PARAM_3_MIN,
    SENSOR_2_PARAM_3_MAX,
    SENSOR_2_PARAM_3_INCREMENT,

    SENSOR_3_NAME_LIST,
    SENSOR_3_IPSO_TYPE,
    SENSOR_3_PRECISION,
    SENSOR_3_FUNCTION,
    SENSOR_3_MIN_VALUE,
    SENSOR_3_MAX_VALUE,
    SENSOR_3_MIN_RANGE,
    SENSOR_3_MAX_RANGE,
    SENSOR_3_PARAM_0_VALUE,
    SENSOR_3_PARAM_0_MIN,
    SENSOR_3_PARAM_0_MAX,
    SENSOR_3_PARAM_0_INCREMENT,
    SENSOR_3_PARAM_1_VALUE,
    SENSOR_3_PARAM_1_MIN,
    SENSOR_3_PARAM_1_MAX,
    SENSOR_3_PARAM_1_INCREMENT,
    SENSOR_3_PARAM_2_VALUE,
    SENSOR_3_PARAM_2_MIN,
    SENSOR_3_PARAM_2_MAX,
    SENSOR_3_PARAM_2_INCREMENT,
    SENSOR_3_PARAM_3_VALUE,
    SENSOR_3_PARAM_3_MIN,
    SENSOR_3_PARAM_3_MAX,
    SENSOR_3_PARAM_3_INCREMENT,

    /**< Device Extension Space */
    DEVICE_EXTENSION_SPACE,
    USER_EXTENSION_SPACE,
    SENSOR_0_NAME,
    SENSOR_1_NAME,
    SENSOR_2_NAME,
    SENSOR_3_NAME,
    OUTPUT_0_NAME,
    OUTPUT_1_NAME,
    OUTPUT_2_NAME,
    OUTPUT_3_NAME,
    PARAM_0_NAME,
    PARAM_1_NAME,
    PARAM_2_NAME,
    PARAM_3_NAME,
    PARAM_4_NAME,
    PARAM_5_NAME,
    PARAM_6_NAME,
    PARAM_7_NAME,
    PARAM_8_NAME,
    PARAM_9_NAME,
    PARAM_10_NAME,
    PARAM_11_NAME,
    PARAM_12_NAME,
    PARAM_13_NAME,
    PARAM_14_NAME,
    PARAM_15_NAME,
    CALIBRATION_STRING,

	EXTENSION_VBAT,
	EXTENSION_VPOWER,
	EXTENSION_STATUS,
	EXTENSION_BATT_CAPACITY,
	PROBE_STATUS,

    BOOTSTRAP_CONTROL,
    BOOTSTRAP_ADDRESS,
    BOOTSTRAP_DATA,

    CAL_CH0_G16_GAIN,
	CAL_CH0_G16_OFFSET,
	CAL_CH0_G32_GAIN,
	CAL_CH0_G32_OFFSET,
	CAL_CH1_G16_GAIN,
	CAL_CH1_G16_OFFSET,
	CAL_CH1_G32_GAIN,
	CAL_CH1_G32_OFFSET,

	CAL_CJC_1_OFFSET,
	CAL_CJC_2_OFFSET,

	DEVICE_MODEL_NAME,

    LAST_REGISTER
} ss_register_t;

typedef struct {
    uint16_t    modbus_addr;// base modbus address
    uint16_t    i2c_addr;   // base i2c address
    uint8_t     nInstance;  // no of instances
    uint8_t     access;     // read/write
    uint16_t    size;       // no bytes of register size
    uint8_t     offset;     // no bytes offset from one instance addr to another
} _register_t;

typedef enum {
    RD      = (1U << 0U),
    WR      = (1U << 1U),
    EXE     = (1U << 2U),
    BYTES   = (1U << 3U),// LSB format
	STR     = (1U << 4U)//check to see if swapping of bytes needed for strings
} control_t;

typedef struct {
    uint8_t     p0_active: 1;
    uint8_t     p0_auth_fault: 1;
    uint8_t     p0_passwd_fault: 1;
    uint8_t     p0_intr: 1;
    uint8_t     p1_active: 1;
    uint8_t     p1_auth_fault: 1;
    uint8_t     p1_passwd_fault: 1;
    uint8_t     p1_intr: 1;
    uint8_t     padding;        // all zeros
} probe_status_t;

//password status
typedef enum SENSOR_PASSWORD_STATUS
{
	PASSWORD_NO_SUPPORT = 0xffff,
	PASSWORD_DISABLED = 0x0101,
	PASSWORD_ENABLED = 0,
}Sensor_Password_Status_t;

typedef enum BootStrap_Control
{
    WRITE_CMD           = 0x1,
    ERASE_CMD           = 0x2,
    INFO_READ_CMD       = 0x4,
    CRC_VERIFY_CMD      = 0x8,
    RESET_CMD           = 0x10,
    INDEX_FAULT         = 0x20,
    CRC_FAULT           = 0x40,
    ADDRESS_FAULT       = 0x80,
    ANY_FAULT           = 0xE0,
} BootStrap_Control_t;

extern const _register_t* get_register_entry(ss_register_t ss_register);

#endif //OMEGA_SMARTSENSOR_C_REGISTERS_H
