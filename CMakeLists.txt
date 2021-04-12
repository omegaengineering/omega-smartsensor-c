cmake_minimum_required(VERSION 3.7)
project(smartsensorsdk C)
set(CMAKE_C_STANDARD 99)

option(NDEBUG "" OFF)
if(NDEBUG)
    add_definitions(-DNDEBUG)
endif()
option(DEFAULT_SAMPLE_TIME "" OFF)
if(DEFAULT_SAMPLE_TIME)
    add_definitions(-DDEFAULT_SAMPLE_TIME=${DEFAULT_SAMPLE_TIME})
endif()
option(HEARTBEAT_MAX_MISS "" OFF)
if(HEARTBEAT_MAX_MISS)
    add_definitions(-DHEARTBEAT_MAX_MISS=${HEARTBEAT_MAX_MISS})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fshort-enums")

set(SDK_INCLUDES
        core/smartsensor.h)

include_directories(
        ${CMAKE_CURRENT_LIST_DIR}
        core
        lib/inc)

# add sdk core
set(SDK_SRC
        core/registers.c
        core/smartsensor_private.c
        core/smartsensor.c
        core/port.c
        core/function_block.c
        core/alarm.c
        core/datalog.c)

# add port linux
set(SDK_SRC
        ${SDK_SRC}
        port/linux/linux_event.c
        port/linux/linux_timer.c
        port/linux/port_linux.c)

if (MODBUS_SENSOR)
    add_definitions(-DMODBUS_SENSOR)
    set(SDK_SRC
            ${SDK_SRC}
            port/linux/linux_modbus.c)
    set(SDK_LIB ${SDK_LIB} modbus)
endif()
if (I2C_SENSOR)
    add_definitions(-DI2C_SENSOR)
    set(SDK_SRC
            ${SDK_SRC}
            port/linux/linux_i2c.c
            port/linux/linux_gpio.c)
    set(SDK_LIB ${SDK_LIB} i2c)
endif()
if (NOT MODBUS_SENSOR AND NOT I2C_SENSOR)
    message(FATAL_ERROR Must enable one: MODBUS_SENSOR, I2C_SENSOR)
endif()

add_library(smartsensorsdk ${SDK_SRC})

install(TARGETS smartsensorsdk
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/omega)
install(FILES core/smartsensor.h            DESTINATION include/omega/smartsensor)
install(FILES core/smartsensor_private.h    DESTINATION include/omega/smartsensor)
install(FILES core/registers.h              DESTINATION include/omega/smartsensor)
install(FILES core/smartsensor_errors.h     DESTINATION include/omega/smartsensor)
install(FILES port/linux/port_linux.h       DESTINATION include/omega/smartsensor)
install(FILES core/port.h                   DESTINATION include/omega/smartsensor)
install(FILES core/datalog.h                DESTINATION include/omega/smartsensor)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_LIST_DIR}/uninstall.cmake")

option(SDK_WITH_TESTS "Build with Tests" OFF)
if (SDK_WITH_TESTS)
    add_subdirectory(tests)
endif()
option(SDK_WITH_EXAMPLES "Build with Examples" ON)
if (SDK_WITH_EXAMPLES)
    add_subdirectory(examples/linux)
endif()