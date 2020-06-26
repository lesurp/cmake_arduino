set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/avr-toolchain.cmake)

project(cmake_arduino)
cmake_minimum_required(VERSION 3.17)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

### Device config
set(DEVICE_NAME "/dev/ttyUSB0" CACHE STRING "Name of the device on host machine")
set(CHIP_ID "m328p" CACHE STRING "Name of the device's chip")
set(ARDUINO_VARIANT "eightanaloginputs" CACHE STRING "Name of the device variant")

### Fetch and create target for arduinocore
include(fetch_arduinocore)

### Actually build our project
include(add_target_executable)
add_target_executable(main src/main.cpp src/L3G.cpp src/LSM303.cpp)
target_link_libraries(main arduino_core arduino_wire)

add_target_executable(test src/test.cpp)
target_link_libraries(test arduino_core)
