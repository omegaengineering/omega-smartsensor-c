cmake_minimum_required(VERSION 3.7)

include_directories(../../port)


add_executable(demo_linux basic_sensor.c)
target_link_libraries(demo_linux smartsensorsdk portlinux modbus pthread)

add_executable(example_alarm example_alarm.c)
target_link_libraries(example_alarm smartsensorsdk portlinux modbus pthread)

add_executable(example_datalog example_datalog.c)
target_link_libraries(example_datalog smartsensorsdk portlinux modbus pthread)
