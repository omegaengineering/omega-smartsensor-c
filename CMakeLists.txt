cmake_minimum_required(VERSION 3.7)
project(omega_smartsensor_c C)

set(CMAKE_C_STANDARD 99)

set(SDK_SOURCES
        core/smartsensor_private.c
        core/smartsensor.c
        )

set(SDK_INCLUDES
        core/smartsensor.h
        )

find_package(Threads REQUIRED)

include_directories(
        ${CMAKE_CURRENT_LIST_DIR}
        core/
        lib/inc/
)

add_library(omega_lib
        lib/src/hw/linux/bus_i2c_smbus.c
        lib/src/hw/linux/bus_uart_modbus.c
        lib/src/hw/linux/gpio_sysfs.c
        lib/src/hw/bus.c
        lib/src/hw/gpio.c
        lib/src/platform/linux/linux_mutex.c
        lib/src/platform/linux/linux_memory.c
        lib/src/platform/linux/linux_timer.c
        lib/src/common/log.c
        port/linux/port_linux.c
        )


add_library(omega_smartsensor_lib ${SDK_SOURCES})
target_link_libraries(omega_smartsensor_lib omega_lib)

add_subdirectory(tests)
add_subdirectory(examples/linux)