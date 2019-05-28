cmake_minimum_required(VERSION 3.7)

include_directories(../)

add_executable(test_interface test_interface.c)
target_link_libraries(test_interface omega_smartsensor_lib modbus pthread)

add_executable(test_linux_timer test_linux_timer.c)
target_link_libraries(test_linux_timer omega_smartsensor_lib pthread)

add_executable(test_probe_attach test_probe_attach.c)
target_link_libraries(test_probe_attach omega_smartsensor_lib pthread modbus)

add_executable(clock_stretch clock_stretch.c)
target_link_libraries(clock_stretch omega_smartsensor_lib pthread modbus)


add_executable(registers_integrity registers_integrity.c)
target_compile_definitions(omega_smartsensor_lib PRIVATE
        ENABLE_UNIT_TEST=1)
target_link_libraries(registers_integrity omega_smartsensor_lib pthread modbus)