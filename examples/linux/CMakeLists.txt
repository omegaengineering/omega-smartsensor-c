cmake_minimum_required(VERSION 3.7)

include_directories(../../port)

add_library(linux_i2c
        ../../port/linux/event.c
        ../../port/linux/linux_platform.c
        ../../port/linux/linux_i2c.c
        ../../port/linux/linux_gpio.c
        ../../port/linux/linux_timer.c)

add_executable(demo_linux basic_sensor.c)
target_link_libraries(demo_linux smartsensor linux_i2c pthread)
