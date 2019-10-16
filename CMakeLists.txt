cmake_minimum_required(VERSION 3.7)
project(smartsensor C)
set(CMAKE_C_STANDARD 99)

add_definitions(-DNDEBUG=${NDEBUG})

set(SDK_SOURCES
        core/smartsensor_private.c
        core/smartsensor.c)

set(SDK_INCLUDES
        core/smartsensor.h)

find_package(Threads REQUIRED)

include_directories(
        ${CMAKE_CURRENT_LIST_DIR}
        core/
        lib/inc/)

set(LINUX_SOURCES
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

add_library(${PROJECT_NAME} ${SDK_SOURCES} ${LINUX_SOURCES})
target_link_libraries(${PROJECT_NAME} pthread modbus)
set_target_properties(${PROJECT_NAME} PROPERTIES
        PUBLIC_HEADER "core/smartsensor.h")

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/omega)
install(FILES core/registers.h DESTINATION include/omega)
install(FILES lib/inc/common/errors.h DESTINATION include/omega/common)
install(FILES lib/inc/common/common.h DESTINATION include/omega/common)
add_custom_target(uninstall
        "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_LIST_DIR}/uninstall.cmake")

option(SDK_WITH_TESTS "Build with Tests" OFF)
if (SDK_WITH_TESTS)
    add_subdirectory(tests)
endif()
option(SDK_WITH_EXAMPLES "Build with Examples" ON)
if (SDK_WITH_EXAMPLES)
    add_subdirectory(examples/linux)
endif()