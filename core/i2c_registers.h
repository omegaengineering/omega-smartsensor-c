/*!********************************************************************************************
  @file i2c_registers.h

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
            Contains defined I2C registers address for Smartsensor devices.
            The I2C registes are defined as 16 bit however, only 8 bit register address
            are transmitted on bus. Therefore, a special routing "i2c_set_index" is used
            handle addresses > 0xff. Basically it breaks 16 bit addresses into 2 part
            transactions.

***********************************************************************************************/
#ifndef I2C_REGISTERS_H
#define I2C_REGISTERS_H

#define R_DEVICE_ID                 0x0000
#define R_VERSION                   0x0004
#define R_HARDWARE_VERSION          0x0008
#define R_DEVICE_LIST_INDEX         0x000c
#define R_BLOCK_START               0x000d
#define R_USER_HOURS                0x000e

#define R_EVENT_0_TIMEBASE          0x0010
#define R_EVENT_1_TIMEBASE          0x0012

#define R_SYSTEM_CONTROL            0x0014

#define R_INTERRUPT_STATUS          0x0016
#define R_INTERRUPT_CONTROL         0x0018

#define R_NUMOFSENSORS              0x001a
#define R_NUMOFOUTPUTS              0x001b
#define R_OPERATING_TEMP            0x001c
#define R_OPERATING_VOLTAGE         0x001d
#define R_FAULT_PROCESS             0x001e
#define R_FAULT_CODE                0x001f

#define R_EVENT_0_TIMER             0x0020
#define R_EVENT_1_TIMER             0x0022

#define R_SYSTEM_STATUS             0x0024
#define R_TRIGGERS                  0x0026

#define R_EXTRACT_START_TIME        0x0028
#define R_EXTRACT_END_TIME          0x002c

#define R_ACCESS_REGISTER           0x0030
#define R_FACTORY_ACCESS            0x0032
#define R_BLOCK_ACCESS              0x0033
#define R_SENSOR_ACCESS             0x0034
#define R_EXTENSION_ACCESS          0x0035

#define R_NUMBER_RECORDS            0x0036

#define R_CURRENT_TIME              0x0038

#define R_SENSOR_VALUE              0x003c
#define R_SENSOR_0_VALUE            0x003c
#define R_SENSOR_1_VALUE            0x0040
#define R_SENSOR_2_VALUE            0x0044
#define R_SENSOR_3_VALUE            0x0048

#define R_EXTRACTED_TIME            0x004c

#define R_EXTRACTED_0_VALUE         0x0050
#define R_EXTRACTED_1_VALUE         0x0054
#define R_EXTRACTED_2_VALUE         0x0058
#define R_EXTRACTED_3_VALUE         0x005c

#define R_SENSOR_DESCRIPTORS        0x0060
#define R_SENSOR_0_DESCRIPTOR       0x0060
#define R_SENSOR_0_TYPE             0x0060
#define R_SENSOR_0_FORMAT           0x0061
#define R_SENSOR_0_CONFIG           0x0062
#define R_SENSOR_0_DEVICE           0x0063

#define R_SENSOR_UNITS              0x0064
#define R_SENSOR_0_UNITS            0x0064

#define R_SENSOR_1_DESCRIPTOR       0x0068
#define R_SENSOR_1_TYPE             0x0068
#define R_SENSOR_1_FORMAT           0x0069
#define R_SENSOR_1_CONFIG           0x006a
#define R_SENSOR_1_DEVICE           0x006b
#define R_SENSOR_1_UNITS            0x006c

#define R_SENSOR_2_DESCRIPTOR       0x0070
#define R_SENSOR_2_TYPE             0x0070
#define R_SENSOR_2_FORMAT           0x0071
#define R_SENSOR_2_CONFIG           0x0072
#define R_SENSOR_2_DEVICE           0x0073
#define R_SENSOR_2_UNITS            0x0074

#define R_SENSOR_3_DESCRIPTOR       0x0078
#define R_SENSOR_3_TYPE             0x0078
#define R_SENSOR_3_FORMAT           0x0079
#define R_SENSOR_3_CONFIG           0x007a
#define R_SENSOR_3_DEVICE           0x007b
#define R_SENSOR_3_UNITS            0x007c

#define R_USER_PARAMETERS           0x0080
#define R_USER_PARAMETER_0_VALUE    0x0080
#define R_USER_PARAMETER_1_VALUE    0x0084
#define R_USER_PARAMETER_2_VALUE    0x0088
#define R_USER_PARAMETER_3_VALUE    0x008c
#define R_USER_PARAMETER_4_VALUE    0x0090
#define R_USER_PARAMETER_5_VALUE    0x0094
#define R_USER_PARAMETER_6_VALUE    0x0098
#define R_USER_PARAMETER_7_VALUE    0x009c
#define R_USER_PARAMETER_8_VALUE    0x00a0
#define R_USER_PARAMETER_9_VALUE    0x00a4
#define R_USER_PARAMETER_10_VALUE   0x00a8
#define R_USER_PARAMETER_11_VALUE   0x00ac
#define R_USER_PARAMETER_12_VALUE   0x00b0
#define R_USER_PARAMETER_13_VALUE   0x00b4
#define R_USER_PARAMETER_14_VALUE   0x00b8
#define R_USER_PARAMETER_15_VALUE   0x00bc

#define R_SCALE_PARAMETERS          0x00c0
#define R_SENSOR_0_GAIN             0x00c0
#define R_SENSOR_0_OFFSET           0x00c4
#define R_SENSOR_1_GAIN             0x00c8
#define R_SENSOR_1_OFFSET           0x00cc
#define R_SENSOR_2_GAIN             0x00d0
#define R_SENSOR_2_OFFSET           0x00d4
#define R_SENSOR_3_GAIN             0x00d8
#define R_SENSOR_3_OFFSET           0x00dc

#define R_DEVICE_NAME               0x00e0

#define R_OUTPUT_VALUE              0x00f0
#define R_OUTPUT_0_VALUE            0x00f0
#define R_OUTPUT_1_VALUE            0x00f4
#define R_OUTPUT_2_VALUE            0x00f8
#define R_OUTPUT_3_VALUE            0x00fc

#define F_DEVICE_ID                         0x0100
#define F_HARDWARE_VERSION                  0x0104
#define R_CORE_VERSION                      0x0108
#define R_BLOCK_START_RETRY                 0x010c
#define R_RTC_TUNING                        0x010e
#define R_FEATURE_BITS                      0x0110
#define R_DEFAULT_EVENT_0_TIMEBASE          0x0114
#define R_DEFAULT_EVENT_1_TIMEBASE          0x0116
#define R_DEFAULT_SYSTEM_CONTROL            0x0118
#define R_DEFAULT_INTERRUPT_CONTROL         0x011a
#define R_DEVICE_NUMBER_INDEX               0x011c    // List Selection & Max available
#define R_DEVICE_NUMBER_LIST                0x011e    // Which device List to use & Max available
#define R_SENSOR_0_ERRORS                   0x0120    // Sensor Error List
#define R_SENSOR_1_ERRORS                   0x0122    // Sensor Error List
#define R_SENSOR_2_ERRORS                   0x0124    // Sensor Error List
#define R_SENSOR_3_ERRORS                   0x0126    // Sensor Error List
#define R_MANUFACTURED_DATE                 0x0128
#define R_CALIBRATION_DATE                  0x012a
#define R_TOTAL_OPERATING_TIME              0x012c
#define R_TIME_SINCE_CALIBRATION            0x0130

#define R_OUTPUT_DESCRIPTORS                0x0134
#define R_OUTPUT_0_DESCRIPTOR               0x0134    //  PWM_Config/Type is Integer addressable
#define R_OUTPUT_1_DESCRIPTOR               0x0136
#define R_OUTPUT_2_DESCRIPTOR               0x0138
#define R_OUTPUT_3_DESCRIPTOR               0x013a
#define R_BASE_TYPE                         0x013c    // Base type
#define R_SENTINEL                          0x013e    // SENTINEL
#define R_IO_DEVICE_LIST_NAMES              0x0140    // treated as an array of bytes...

#define R_OUTPUT_0_NAMELIST                 0x0700
#define R_OUTPUT_1_NAMELIST                 0x0740
#define R_OUTPUT_2_NAMELIST                 0x0780
#define R_OUTPUT_3_NAMELIST                 0x07c0

#define R_SENSOR_0_NAMELIST                 0x0800
#define R_SENSOR_0_IPSO_TYPE                0x08a8
#define R_SENSOR_0_PRECISION                0x08aa  // int8_t giving number of of digits of precision, ie: 12.345  -> Precision = +3, negative prc implies  12?.? -> Precision = -1
#define R_SENSOR_0_RESET_MINMAX             0x08ac  // Write any value to reset the Min/Max values

#define R_SENSOR_0_MINVALUE                 0x08b0  // float giving minimum Value
#define R_SENSOR_0_MAXVALUE                 0x08b4  // float giving maximum Value
#define R_SENSOR_0_MINRANGE                 0x08b8  // float giving minimum Range
#define R_SENSOR_0_MAXRANGE                 0x08bc  // float giving maximum Range
#define R_SENSOR_0_PARAMETER_0              0x08c0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)
#define R_SENSOR_0_PARAMETER_1              0x08d0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)
#define R_SENSOR_0_PARAMETER_2              0x08e0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)
#define R_SENSOR_0_PARAMETER_3              0x08f0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)

#define R_SENSOR_1_NAMELIST                0x0900

#define R_SENSOR_1_IPSO_TYPE                0x09a8
#define R_SENSOR_1_PRECISION                0x09aa  // int8_t giving number of of digits of precision, ie: 12.345  -> Precision = +3, negative prc implies  12?.? -> Precision = -1
#define R_SENSOR_1_RESET_MINMAX             0x09ac  // Write any value to reset the Min/Max values

#define R_SENSOR_1_MINVALUE                 0x09b0  // float giving minimum Value
#define R_SENSOR_1_MAXVALUE                 0x09b4  // float giving maximum Value
#define R_SENSOR_1_MINRANGE                 0x09b8  // float giving minimum Range
#define R_SENSOR_1_MAXRANGE                 0x09bc  // float giving maximum Range
#define R_SENSOR_1_PARAMETER_0              0x09c0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)
#define R_SENSOR_1_PARAMETER_1              0x09d0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)
#define R_SENSOR_1_PARAMETER_2              0x09e0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)
#define R_SENSOR_1_PARAMETER_3              0x09f0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)

#define R_SENSOR_2_NAMELIST                 0x0a00

#define R_SENSOR_2_IPSO_TYPE                0x0aa8
#define R_SENSOR_2_PRECISION                0x0aaa  // int8_t giving number of of digits of precision, ie: 12.345  -> Precision = +3, negative prc implies  12?.? -> Precision = -1
#define R_SENSOR_2_RESET_MINMAX             0x0aac  // Write any value to reset the Min/Max values

#define R_SENSOR_2_MINVALUE                 0x0ab0  // float giving minimum Value
#define R_SENSOR_2_MAXVALUE                 0x0ab4  // float giving maximum Value
#define R_SENSOR_2_MINRANGE                 0x0ab8  // float giving minimum Range
#define R_SENSOR_2_MAXRANGE                 0x0abc  // float giving maximum Range
#define R_SENSOR_2_PARAMETER_0              0x0ac0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)
#define R_SENSOR_2_PARAMETER_1              0x0ad0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)
#define R_SENSOR_2_PARAMETER_2              0x0ae0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)
#define R_SENSOR_2_PARAMETER_3              0x0af0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)

#define R_SENSOR_3_NAMELIST                 0x0b00

#define R_SENSOR_3_IPSO_TYPE                0x0ba8
#define R_SENSOR_3_PRECISION                0x0baa  // int8_t giving number of of digits of precision, ie: 12.345  -> Precision = +3, negative prc implies  12?.? -> Precision = -1
#define R_SENSOR_3_RESET_MINMAX             0x0bac  // Write any value to reset the Min/Max values

#define R_SENSOR_3_MINVALUE                 0x0bb0  // float giving minimum Value
#define R_SENSOR_3_MAXVALUE                 0x0bb4  // float giving maximum Value
#define R_SENSOR_3_MINRANGE                 0x0bb8  // float giving minimum Range
#define R_SENSOR_3_MAXRANGE                 0x0bbc  // float giving maximum Range
#define R_SENSOR_3_PARAMETER_0              0x0bc0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)
#define R_SENSOR_3_PARAMETER_1              0x0bd0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)
#define R_SENSOR_3_PARAMETER_2              0x0be0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)
#define R_SENSOR_3_PARAMETER_3              0x0bf0  // Each R_SENsor may have up to 4 Parameters, starting at 0xc0 offset (64 bytes / 4 = 16 floats / 4 floats/entry == 4 entries)

#define R_NAME_LIST                         0x0e00    // Names of Sensors, Output, Parameters etc...
#define R_SENSOR_0_NAME                     0x0e00
#define R_SENSOR_1_NAME                     0x0e08
#define R_SENSOR_2_NAME                     0x0e10
#define R_SENSOR_3_NAME                     0x0e18

#define R_OUTPUT_0_NAME                     0x0e20
#define R_OUTPUT_1_NAME                     0x0e28
#define R_OUTPUT_2_NAME                     0x0e30
#define R_OUTPUT_3_NAME                     0x0e38

#define R_USER_PARAMETER_0_NAME             0x0e40
#define R_USER_PARAMETER_1_NAME             0x0e48
#define R_USER_PARAMETER_2_NAME             0x0e50
#define R_USER_PARAMETER_3_NAME             0x0e58
#define R_USER_PARAMETER_4_NAME             0x0e60
#define R_USER_PARAMETER_5_NAME             0x0e68
#define R_USER_PARAMETER_6_NAME             0x0e70
#define R_USER_PARAMETER_7_NAME             0x0e78
#define R_USER_PARAMETER_8_NAME             0x0e80
#define R_USER_PARAMETER_9_NAME             0x0e88
#define R_USER_PARAMETER_10_NAME            0x0e90
#define R_USER_PARAMETER_11_NAME            0x0e98
#define R_USER_PARAMETER_12_NAME            0x0ea0
#define R_USER_PARAMETER_13_NAME            0x0ea8
#define R_USER_PARAMETER_14_NAME            0x0eb0
#define R_USER_PARAMETER_15_NAME            0x0eb8

#define R_FUNCTION_BLOCK_0_NAME             0x0ec0
#define R_FUNCTION_BLOCK_1_NAME             0x0ec8
#define R_FUNCTION_BLOCK_2_NAME             0x0ed0
#define R_FUNCTION_BLOCK_3_NAME             0x0ed8
#define R_FUNCTION_BLOCK_4_NAME             0x0ee0
#define R_FUNCTION_BLOCK_5_NAME             0x0ee8
#define R_FUNCTION_BLOCK_6_NAME             0x0ef0
#define R_FUNCTION_BLOCK_7_NAME             0x0ef8
#define R_FUNCTION_BLOCK_8_NAME             0x0f00
#define R_FUNCTION_BLOCK_9_NAME             0x0f08
#define R_FUNCTION_BLOCK_10_NAME            0x0f10
#define R_FUNCTION_BLOCK_11_NAME            0x0f18
#define R_FUNCTION_BLOCK_12_NAME            0x0f20
#define R_FUNCTION_BLOCK_13_NAME            0x0f28
#define R_FUNCTION_BLOCK_14_NAME            0x0f30
#define R_FUNCTION_BLOCK_15_NAME            0x0f38
#define R_FUNCTION_BLOCK_16_NAME            0x0f40
#define R_FUNCTION_BLOCK_17_NAME            0x0f48
#define R_FUNCTION_BLOCK_18_NAME            0x0f50
#define R_FUNCTION_BLOCK_19_NAME            0x0f58
#define R_FUNCTION_BLOCK_20_NAME            0x0f60
#define R_FUNCTION_BLOCK_21_NAME            0x0f68
#define R_FUNCTION_BLOCK_22_NAME            0x0f70
#define R_FUNCTION_BLOCK_23_NAME            0x0f78
#define R_FUNCTION_BLOCK_24_NAME            0x0f80
#define R_FUNCTION_BLOCK_25_NAME            0x0f88
#define R_FUNCTION_BLOCK_26_NAME            0x0f90
#define R_FUNCTION_BLOCK_27_NAME            0x0f98
#define R_FUNCTION_BLOCK_28_NAME            0x0fa0
#define R_FUNCTION_BLOCK_29_NAME            0x0fa8
#define R_FUNCTION_BLOCK_30_NAME            0x0fb0
#define R_FUNCTION_BLOCK_31_NAME            0x0fb8

#define R_FB_PARAMETER_0_NAME               0x0fc0
#define R_FB_PARAMETER_1_NAME               0x0fc8
#define R_FB_PARAMETER_2_NAME               0x0fd0
#define R_FB_PARAMETER_3_NAME               0x0fd8
#define R_FB_PARAMETER_4_NAME               0x0fe0
#define R_FB_PARAMETER_5_NAME               0x0fe8
#define R_FB_PARAMETER_6_NAME               0x0ff0
#define R_FB_PARAMETER_7_NAME               0x0ff8

#define R_MAX_REGISTER_INDEX                0x0fff

// THESE ARE ALL DERIVED (no direct access)

#define R_SENSOR_0_DEVICENAME               0x1000
#define R_SENSOR_1_DEVICENAME               0x1008
#define R_SENSOR_2_DEVICENAME               0x1010
#define R_SENSOR_3_DEVICENAME               0x1018

#define R_SENSOR_0_NUMPARAMETERS            0x1020
#define R_SENSOR_1_NUMPARAMETERS            0x1021
#define R_SENSOR_2_NUMPARAMETERS            0x1022
#define R_SENSOR_3_NUMPARAMETERS            0x1023

#define R_OUTPUT_0_DEVICENAME               0x1100
#define R_OUTPUT_1_DEVICENAME               0x1108
#define R_OUTPUT_2_DEVICENAME               0x1110
#define R_OUTPUT_3_DEVICENAME               0x1118

// Pseudo 'Event Type' record
#define R_ACCESS_RECORDS                    0x1200      // allows searching
#define R_EVENT_TYPE                        0x1202      // interprets record type
#define R_EVENT_INFO_0                      0x1203
#define R_EVENT_INFO_1                      0x1204

#define R_DEVICE_RESET                      0x1210




#define R_SENSOR_0_NUMOPTIONS               0x1024
#define R_SENSOR_1_NUMOPTIONS               0x1025
#define R_SENSOR_2_NUMOPTIONS               0x1026
#define R_SENSOR_3_NUMOPTIONS               0x1027


#define R_MAX_LIST_OPTION                   0x1028

#define R_PRODUCT_NAME                      0x10b0

#define R_EVENT_2_TIMEBASE                  0x10c0
#define R_EVENT_3_TIMEBASE                  0x10c2

#define R_EVENT_2_TIMER                     0x10c4
#define R_EVENT_3_TIMER                     0x10c6

#define R_EVENT_0_MODE                      0x10c8
#define R_EVENT_1_MODE                      0x10c9
#define R_EVENT_2_MODE                      0x10ca
#define R_EVENT_3_MODE                      0x10cb

#define R_OUTPUT_0_NUMOPTIONS               0x10d0
#define R_OUTPUT_1_NUMOPTIONS               0x10d1
#define R_OUTPUT_2_NUMOPTIONS               0x10d2
#define R_OUTPUT_3_NUMOPTIONS               0x10d3

#define R_OUTPUT_0_UNITS                    0x10d4
#define R_OUTPUT_1_UNITS                    0x10d5
#define R_OUTPUT_2_UNITS                    0x10d6
#define R_OUTPUT_3_UNITS                    0x10d7



#define X_DEVICE_LIST_DESC_0                0x10a0
#define X_DEVICE_LIST_DESC_1                0x10a1
#define X_DEVICE_LIST_DESC_2                0x10a2
#define X_DEVICE_LIST_DESC_3                0x10a3
#define X_DEVICE_LIST_DESC_4                0x10a4
#define X_DEVICE_LIST_DESC_5                0x10a5
#define X_DEVICE_LIST_DESC_6                0x10a6
#define X_DEVICE_LIST_DESC_7                0x10a7
#define X_DEVICE_LIST_DESC_8                0x10a8
#define X_DEVICE_LIST_DESC_9                0x10a9
#define X_DEVICE_LIST_DESC_10               0x10aa
#define X_DEVICE_LIST_DESC_11               0x10ab
#define X_DEVICE_LIST_DESC_12               0x10ac
#define X_DEVICE_LIST_DESC_13               0x10ae
#define X_DEVICE_LIST_DESC_14               0x10ae
#define X_DEVICE_LIST_DESC_15               0x10af
#define X_DEVICE_MODEL_NAME                 0x10b0

#define X_STATS_ATTACHED                    0x1100
#define X_STATS_INTR_COUNT                  0x1101
#define X_STATS_READ_COUNT                  0x1102
#define X_STATS_WRITE_COUNT                 0x1103
#define X_STATS_ATTACH_COUNT                0x1104
#define X_STATS_NAK_COUNT                   0x1105
#define X_STATS_PWRFLT_COUNT                0x1106

#endif /* I2C_REGISTERS_H */

