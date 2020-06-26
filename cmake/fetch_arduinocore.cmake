include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/thirdparties)
FetchContent_Declare(
    arduino
    GIT_REPOSITORY https://github.com/arduino/ArduinoCore-avr
    GIT_TAG        1.8.3
    )
FetchContent_MakeAvailable(arduino)

### Arduino core
add_library(arduino_core 
    ${arduino_SOURCE_DIR}/cores/arduino/CDC.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/HardwareSerial.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/HardwareSerial0.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/HardwareSerial1.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/HardwareSerial2.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/HardwareSerial3.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/IPAddress.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/PluggableUSB.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/Print.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/Stream.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/Tone.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/USBCore.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/WInterrupts.c
    ${arduino_SOURCE_DIR}/cores/arduino/WMath.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/WString.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/abi.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/hooks.c
    ${arduino_SOURCE_DIR}/cores/arduino/main.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/new.cpp
    ${arduino_SOURCE_DIR}/cores/arduino/wiring.c
    ${arduino_SOURCE_DIR}/cores/arduino/wiring_analog.c
    ${arduino_SOURCE_DIR}/cores/arduino/wiring_digital.c
    ${arduino_SOURCE_DIR}/cores/arduino/wiring_pulse.c
    ${arduino_SOURCE_DIR}/cores/arduino/wiring_shift.c
    )

target_include_directories(arduino_core SYSTEM PUBLIC
    ${arduino_SOURCE_DIR}/variants/${ARDUINO_VARIANT})

# FIXME: cannot make those SYSTEM as we get "conflicting declaration"
# why does this work with -I but not -isystem..?
target_include_directories(arduino_core PUBLIC
    ${arduino_SOURCE_DIR}/cores/arduino)

### Arduino Wire
add_library(arduino_wire 
    ${arduino_SOURCE_DIR}/libraries/Wire/src/Wire.cpp
    ${arduino_SOURCE_DIR}/libraries/Wire/src/utility/twi.c
    )

target_include_directories(arduino_wire SYSTEM PUBLIC
    ${arduino_SOURCE_DIR}/libraries/Wire/src)
target_link_libraries(arduino_wire arduino_core)
