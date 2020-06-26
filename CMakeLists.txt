set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/avr-toolchain.cmake)
project(cmake_arduino)
cmake_minimum_required(VERSION 3.17)

### Device config
set(DEVICE_NAME "/dev/ttyUSB0" CACHE STRING "Name of the device on host machine")
set(CHIP_ID "m328p" CACHE STRING "Name of the device's chip")
set(ARDUINO_VARIANT "eightanaloginputs" CACHE STRING "Name of the device variant")

### Fetch and create target for arduinocore
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(fetch_arduinocore)

### Create custom commands, to upload the compiled project to the arduino
set(OUTPUT_HEX ${PROJECT_BINARY_DIR}/a.hex)
add_custom_target(convert
    COMMAND avr-objcopy -O ihex -R .eeprom $<TARGET_FILE:output_elf> ${OUTPUT_HEX}
    BYPRODUCTS ${PROJECT_BINARY_DIR}/a.hex)
add_dependencies(convert output_elf)

add_custom_target(upload
    avrdude -D -carduino -p${CHIP_ID} -b57600 -P${DEVICE_NAME} -Uflash:w:${OUTPUT_HEX} )
add_dependencies(upload convert)

### Actually build our project
add_executable(output_elf src/main.cpp src/L3G.cpp src/LSM303.cpp)
target_link_libraries(output_elf arduino_core arduino_wire)
