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

//#define R_ACCESS_REGISTER     0x0030
//#define R_FACTORY_ACCESS      0x0032
//#define R_BLOCK_ACCESS              0x0033
//#define R_SENSOR_ACCESS             0x0034
//#define R_EXTENSION_ACCESS          0x0035

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

#define R_CONFIG_PASSWORD 0x0380
#define R_CONN_PASSWORD 0x0390

#define R_OUTPUT_0_NAMELIST                 0x0700
#define R_OUTPUT_1_NAMELIST                 0x0740
#define R_OUTPUT_2_NAMELIST                 0x0780
#define R_OUTPUT_3_NAMELIST                 0x07c0

#define R_SENSOR_0_NAMELIST                 0x0800
#define R_SENSOR_0_IPSO_TYPE                0x08a8
#define R_SENSOR_0_PRECISION                0x08aa  // int8_t giving number of of digits of precision, ie: 12.345  -> Precision = +3, negative prc implies  12?.? -> Precision = -1
#define R_SENSOR_0_FUNCTION             0x08ac  // Refer to the sensor function enum

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
#define R_SENSOR_1_FUNCTION             0x09ac  // Refer to the sensor function enum

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
#define R_SENSOR_2_FUNCTION             0x0aac  // Refer to the sensor function enum

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
#define R_SENSOR_3_FUNCTION             0x0bac  // Refer to the sensor function enum

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
#define R_SENSOR_1_NAME                     0x0e10
#define R_SENSOR_2_NAME                     0x0e20
#define R_SENSOR_3_NAME                     0x0e30

#define R_OUTPUT_0_NAME                     0x0e40
#define R_OUTPUT_1_NAME                     0x0e50
#define R_OUTPUT_2_NAME                     0x0e60
#define R_OUTPUT_3_NAME                     0x0e70

#define R_USER_PARAMETER_0_NAME             0x0e80
#define R_USER_PARAMETER_1_NAME             0x0e90
#define R_USER_PARAMETER_2_NAME             0x0ea0
#define R_USER_PARAMETER_3_NAME             0x0eb0
#define R_USER_PARAMETER_4_NAME             0x0ec0
#define R_USER_PARAMETER_5_NAME             0x0ed0
#define R_USER_PARAMETER_6_NAME             0x0ee0
#define R_USER_PARAMETER_7_NAME             0x0ef0

#define R_USER_PARAMETER_8_NAME             0x0f00
#define R_USER_PARAMETER_9_NAME             0x0f10
#define R_USER_PARAMETER_10_NAME            0x0f20
#define R_USER_PARAMETER_11_NAME            0x0f30
#define R_USER_PARAMETER_12_NAME            0x0f40
#define R_USER_PARAMETER_13_NAME            0x0f50
#define R_USER_PARAMETER_14_NAME            0x0f60
#define R_USER_PARAMETER_15_NAME            0x0f70

#define R_REGISTER_ACCESS                   0x0030

#define R_ACCESS_FACTORY_INDEX              0x0032
#define R_ACCESS_FACTORY_START              0x0000
#define R_ACCESS_FACTORY_END                0x03ff

/********************************************************************************************************/

#define R_ACCESS_BLOCK_INDEX                0x0033
#define R_ACCESS_BLOCK_START                0x0400

#define R_FUNCTION_BLOCK_PROGRAM	        0x0400      // 512 byte block program
#define R_FUNCTION_BLOCK_PROGRAM_0          R_FUNCTION_BLOCK_PROGRAM
#define R_FUNCTION_BLOCK_PROGRAM_1          0x0404

#define R_FUNCTION_BLOCK_PARAMETER          0x0600      // 128 byte block param
#define R_FUNCTION_BLOCK_PARAMETER_0	    R_FUNCTION_BLOCK_PARAMETER
#define R_FUNCTION_BLOCK_PARAMETER_1	    0x0604
#define R_FUNCTION_BLOCK_PARAMETER_2	    0x0608
#define R_FUNCTION_BLOCK_PARAMETER_3	    0x060c
#define R_FUNCTION_BLOCK_PARAMETER_4	    0x0610
#define R_FUNCTION_BLOCK_PARAMETER_5	    0x0614
#define R_FUNCTION_BLOCK_PARAMETER_6	    0x0618
#define R_FUNCTION_BLOCK_PARAMETER_7	    0x061c
#define R_FUNCTION_BLOCK_PARAMETER_8	    0x0620
#define R_FUNCTION_BLOCK_PARAMETER_9	    0x0624
#define R_FUNCTION_BLOCK_PARAMETER_10	    0x0628
#define R_FUNCTION_BLOCK_PARAMETER_11	    0x062c
#define R_FUNCTION_BLOCK_PARAMETER_12	    0x0630
#define R_FUNCTION_BLOCK_PARAMETER_13	    0x0634
#define R_FUNCTION_BLOCK_PARAMETER_14	    0x0638
#define R_FUNCTION_BLOCK_PARAMETER_15	    0x063c
#define R_FUNCTION_BLOCK_PARAMETER_16	    0x0640
#define R_FUNCTION_BLOCK_PARAMETER_17	    0x0644
#define R_FUNCTION_BLOCK_PARAMETER_18	    0x0648
#define R_FUNCTION_BLOCK_PARAMETER_19	    0x064c
#define R_FUNCTION_BLOCK_PARAMETER_20	    0x0650
#define R_FUNCTION_BLOCK_PARAMETER_21	    0x0654
#define R_FUNCTION_BLOCK_PARAMETER_22	    0x0658
#define R_FUNCTION_BLOCK_PARAMETER_23	    0x065c
#define R_FUNCTION_BLOCK_PARAMETER_24	    0x0660
#define R_FUNCTION_BLOCK_PARAMETER_25	    0x0664
#define R_FUNCTION_BLOCK_PARAMETER_26	    0x0668
#define R_FUNCTION_BLOCK_PARAMETER_27	    0x066c
#define R_FUNCTION_BLOCK_PARAMETER_28	    0x0670
#define R_FUNCTION_BLOCK_PARAMETER_29	    0x0674
#define R_FUNCTION_BLOCK_PARAMETER_30	    0x0678
#define R_FUNCTION_BLOCK_PARAMETER_31	    0x067c

#define R_FUNCTION_BLOCK_PARAMETER_TYPE_0	0x0680
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_1	0x0682
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_2	0x0684
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_3	0x0686
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_4	0x0688
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_5	0x068a
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_6	0x068c
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_7	0x068e
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_8	0x0690
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_9	0x0692
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_10	0x0694
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_11	0x0696
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_12	0x0698
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_13	0x069a
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_14	0x069c
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_15	0x069e
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_16	0x06a0
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_17	0x06a2
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_18	0x06a4
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_19	0x06a6
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_20	0x06a8
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_21	0x06aa
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_22	0x06ac
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_23	0x06ae
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_24	0x06b0
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_25	0x06b2
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_26	0x06b4
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_27	0x06b6
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_28	0x06b8
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_29	0x06ba
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_30	0x06bc
#define R_FUNCTION_BLOCK_PARAMETER_TYPE_31	0x06be
#define R_FUNCTION_BLOCK_CONTROL_STATUS_0   0x06c0
#define R_FUNCTION_BLOCK_CONTROL_STATUS_1   0x06c2
#define R_FUNCTION_BLOCK_CONTROL_STATUS_2   0x06c4
#define R_FUNCTION_BLOCK_CONTROL_STATUS_3   0x06c6
#define R_FUNCTION_BLOCK_CONTROL_STATUS_4   0x06c8
#define R_FUNCTION_BLOCK_CONTROL_STATUS_5   0x06ca
#define R_FUNCTION_BLOCK_CONTROL_STATUS_6   0x06cc
#define R_FUNCTION_BLOCK_CONTROL_STATUS_7   0x06ce
#define R_FUNCTION_BLOCK_CONTROL_STATUS_8   0x06d0
#define R_FUNCTION_BLOCK_CONTROL_STATUS_9   0x06d2
#define R_FUNCTION_BLOCK_CONTROL_STATUS_10	0x06d4
#define R_FUNCTION_BLOCK_CONTROL_STATUS_11	0x06d6
#define R_FUNCTION_BLOCK_CONTROL_STATUS_12	0x06d8
#define R_FUNCTION_BLOCK_CONTROL_STATUS_13	0x06da
#define R_FUNCTION_BLOCK_CONTROL_STATUS_14	0x06dc
#define R_FUNCTION_BLOCK_CONTROL_STATUS_15	0x06de
#define R_FUNCTION_BLOCK_CONTROL_STATUS_16	0x06e0
#define R_FUNCTION_BLOCK_CONTROL_STATUS_17	0x06e2
#define R_FUNCTION_BLOCK_CONTROL_STATUS_18	0x06e4
#define R_FUNCTION_BLOCK_CONTROL_STATUS_19	0x06e6
#define R_FUNCTION_BLOCK_CONTROL_STATUS_20	0x06e8
#define R_FUNCTION_BLOCK_CONTROL_STATUS_21	0x06ea
#define R_FUNCTION_BLOCK_CONTROL_STATUS_22	0x06ec
#define R_FUNCTION_BLOCK_CONTROL_STATUS_23	0x06ee
#define R_FUNCTION_BLOCK_CONTROL_STATUS_24	0x06f0
#define R_FUNCTION_BLOCK_CONTROL_STATUS_25	0x06f2
#define R_FUNCTION_BLOCK_CONTROL_STATUS_26	0x06f4
#define R_FUNCTION_BLOCK_CONTROL_STATUS_27	0x06f6
#define R_FUNCTION_BLOCK_CONTROL_STATUS_28	0x06f8
#define R_FUNCTION_BLOCK_CONTROL_STATUS_29	0x06fa
#define R_FUNCTION_BLOCK_CONTROL_STATUS_30	0x06fc
#define R_FUNCTION_BLOCK_CONTROL_STATUS_31	0x06fe

#define R_ACCESS_BLOCK_END                  0x07ff

/********************************************************************************************************/

#define R_ACCESS_SENSOR_INDEX               0x0034  // Sensor space
#define R_ACCESS_SENSOR_START               0x0800

#define R_ACCESS_SENSOR_END                 0x0bff

#define R_ACCESS_EXTENSION_INDEX            0x0035
#define R_ACCESS_EXTENSION_START            0x0c00

#define R_ACCESS_EXTENSION_END              0x0fff

/********************************************************************************************************/

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

#define R_EXTENSION_VBAT 0x0c02
#define R_EXTENSION_VPOWER 0x0c04
#define R_EXTENSION_STATUS					0x0c06
#define R_EXTENSION_BATT_CAPACITY 0x0c0a
#define R_PROBE_STATUS                      0x0c0e

#define R_CAL_CH0_G16_GAIN 0x020c
#define R_CAL_CH0_G16_OFFSET 0x0208
#define R_CAL_CH0_G32_GAIN 0x021c
#define R_CAL_CH0_G32_OFFSET 0x0218
#define R_CAL_CH1_G16_GAIN 0x022c
#define R_CAL_CH1_G16_OFFSET 0x0228
#define R_CAL_CH1_G32_GAIN 0x023c
#define R_CAL_CH1_G32_OFFSET 0x0238

#define R_CAL_CJC_1_OFFSET 0x0248
#define R_CAL_CJC_2_OFFSET 0x0258

#endif /* I2C_REGISTERS_H */

