include(${CMAKE_CURRENT_LIST_DIR}/colors.cmake)

function(openocd_find)
    set(options_)
    set(kwargs_)
    set(list_args_ HINTS)
    cmake_parse_arguments(arg "${options_}" "${kwargs_}" "${list_args_}" ${ARGN})
    foreach(unparsed_arg IN LISTS arg_UNPARSED_ARGUMENTS)
        message(WARNING "${ColorYellow}Unparsed argument: ${unparsed_arg}${ColorReset}")
    endforeach()

    find_program(${CMAKE_PROJECT_NAME}_OpenOCD NAMES openocd HINTS ${arg_HINTS} NO_DEFAULT_PATH)
    if (NOT ${${CMAKE_PROJECT_NAME}_OpenOCD})
        find_program(${CMAKE_PROJECT_NAME}_OpenOCD NAMES openocd)   # fallback, only called if not found in hinted paths
    endif()
    if (NOT ${CMAKE_PROJECT_NAME}_OpenOCD)
        message(WARNING "${ColorRed}Failed to find openocd${ColorReset}")
    endif()
    message(STATUS "${ColorGreen}openocd found: ${${CMAKE_PROJECT_NAME}_OpenOCD}${ColorReset}")
endfunction()

function(openocd_flash_target _targetName)
    set(options_ NO_ELF)
    set(kwargs_ SUFFIX BOARD_CFG BIN_EXTENSION)
    set(list_args_)
    cmake_parse_arguments(arg "${options_}" "${kwargs_}" "${list_args_}" ${ARGN})
    foreach(unparsed_arg IN LISTS arg_UNPARSED_ARGUMENTS)
        message(WARNING "${ColorYellow}Unparsed argument: ${unparsed_arg}${ColorReset}")
    endforeach()

    if (NOT arg_SUFFIX)
        message(FATAL_ERROR "Suffix required")
    endif()
    if (NOT arg_BOARD_CFG)
        message(FATAL_ERROR "BOARD_CFG required")
    endif()
    if (NOT arg_BIN_EXTENSION)
        set(arg_BIN_EXTENSION "bin")
    endif()

    get_filename_component(_outName $<TARGET_FILE:${_targetName}> NAME_WLE)
    get_filename_component(_outDir $<TARGET_FILE:${_targetName} DIRECTORY)
    set(_binFile "${_outDir}${_outName}.bin")

    if (NOT arg_NO_ELF)
        set(_flashElf flash-elf-${arg_SUFFIX})
        add_custom_target(${_flashElf} DEPENDS ${_targetName}
            COMMAND
                ${${CMAKE_PROJECT_NAME}_OpenOCD} -f ${arg_BOARD_CFG}
                -c "program $<TARGET_FILE:${_targetName}> verify reset exit"
        )
        message(STATUS "${ColorGreen}${_flashElf} registered ${ColorReset}")
    endif()

    set(_flashBin flash-bin-${arg_SUFFIX})
    add_custom_target(${_flashBin} DEPENDS ${_targetName}
        COMMAND
            ${${CMAKE_PROJECT_NAME}_OpenOCD}
            -f "${arg_BOARD_CFG}"
            # The 0x08000000 address is the start of flash - taken from the STM32f103 user manual
            # It's required for stripped binary files
            # TODO: Parameterize to allow a correct address based on the MCU
            -c "program ${_binFile} verify reset exit 0x08000000"
    )
    message(STATUS "${ColorGreen}${_flashBin} registered ${ColorReset}")
endfunction()
