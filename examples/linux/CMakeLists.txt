cmake_minimum_required(VERSION 3.7)

include_directories(../../port)

add_library(linux_i2c
        ../../port/linux/port_platform.c
        ../../port/linux/port_i2c.c
        ../../port/linux/port_dio.c
        ../../port/linux/port_timer.c)

add_executable(demo_linux basic_sensor.c)
target_link_libraries(demo_linux smartsensor linux_i2c pthread)
