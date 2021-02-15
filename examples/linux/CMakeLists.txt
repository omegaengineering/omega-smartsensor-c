cmake_minimum_required(VERSION 3.7)

include_directories(../../port)

add_executable(basic_sensor basic_sensor.c)
target_link_libraries(basic_sensor smartsensorsdk i2c pthread)
if(${MODBUS_SENSOR})
    target_link_libraries(basic_sensor modbus)
endif()

add_executable(example_alarm example_alarm.c)
target_link_libraries(example_alarm smartsensorsdk modbus pthread)

add_executable(example_datalog example_datalog.c)
target_link_libraries(example_datalog smartsensorsdk modbus pthread)

add_executable(junk junk.c)
target_link_libraries(junk i2c)
