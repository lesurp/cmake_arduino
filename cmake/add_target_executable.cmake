function(add_target_executable target)
    add_executable(${target} ${ARGN})

    ### Create custom commands, to upload the compiled project to the arduino
    set(OUTPUT_HEX ${PROJECT_BINARY_DIR}/${target}.hex)
    add_custom_target(convert_${target}
        COMMAND avr-objcopy -O ihex -R .eeprom $<TARGET_FILE:${target}> ${OUTPUT_HEX}
        BYPRODUCTS ${PROJECT_BINARY_DIR}/a.hex)
    add_dependencies(convert_${target} ${target})

    add_custom_target(upload_${target}
        avrdude -D -carduino -p${CHIP_ID} -b57600 -P${DEVICE_NAME} -Uflash:w:${OUTPUT_HEX} )
    add_dependencies(upload_${target} convert_${target})
endfunction()
