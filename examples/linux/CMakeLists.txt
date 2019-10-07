cmake_minimum_required(VERSION 3.7)

include_directories(../)

add_executable(basic_sensor basic_sensor.c)
target_link_libraries(basic_sensor smartsensor)