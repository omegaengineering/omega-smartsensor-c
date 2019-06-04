cmake_minimum_required(VERSION 3.7)

include_directories(../)

add_executable(basic_sensor_i2c basic_sensor_i2c.c)
target_link_libraries(basic_sensor_i2c
        omega_smartsensor_lib
        modbus
        pthread)

add_executable(basic_sensor_modbus basic_sensor_modbus.c)
target_link_libraries(basic_sensor_modbus
        modbus)