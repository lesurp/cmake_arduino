cmake_minimum_required(VERSION 3.12)

################################################################################
#                        ARDUINO SPEFICIC CONFIGURATION                        #
#                     YOU PROBABLY SHOULDN'T CHANGE THIS !                     #
################################################################################
set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)

set(CSTANDARD "-std=gnu99")
set(CDEBUG "-gstabs")
set(CWARN "-Wall -Wstrict-prototypes")
set(CTUNING "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
set(COPT "-Os")
set(CINCS "-I${ArduinoCode_SOURCE_DIR}/libarduinocore")
set(CMCU "-mmcu=atmega328p")
set(CDEFS "-DF_CPU=16000000")

set(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA}")
set(CXXFLAGS "${CMCU} ${CDEFS} ${CINCS} ${COPT}")

set(CMAKE_C_FLAGS  ${CFLAGS})
set(CMAKE_CXX_FLAGS ${CXXFLAGS})

################################################################################
#                        REGULAR PROJECT CONFIGURATION                         #
################################################################################
project(cmake_arduino)
add_subdirectory(libarduinocore)
set(device_name "/dev/ttyUSB1" CACHE STRING "name of the target device")
set(chip_id "m328p" CACHE STRING "name of the device's chip")

add_executable(output_elf src/main.cpp)
target_link_libraries(output_elf arduinocore)

set(output_hex ${PROJECT_BINARY_DIR}/a.hex)
add_custom_target(convert
    COMMAND avr-objcopy -O ihex -R .eeprom $<TARGET_FILE:output_elf> ${output_hex}
    BYPRODUCTS ${PROJECT_BINARY_DIR}/a.hex)
add_dependencies(convert output_elf)

add_custom_target(upload
    avrdude -D -carduino -p${chip_id} -b57600 -P${device_name} -Uflash:w:${output_hex} )
add_dependencies(upload convert)
