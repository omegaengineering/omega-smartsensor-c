cmake_minimum_required(VERSION 3.7)

include_directories(../../port)

add_library(portlinux
        ../../port/linux/linux_event.c
        ../../port/linux/linux_i2c.c
        ../../port/linux/linux_modbus.c
        ../../port/linux/linux_gpio.c
        ../../port/linux/linux_timer.c
        ../../port/platform/linux.c)

add_executable(demo_linux basic_sensor.c)
target_link_libraries(demo_linux smartsensor portlinux modbus pthread)

add_executable(example_alarm example_alarm.c)
target_link_libraries(example_alarm smartsensor portlinux modbus pthread)
