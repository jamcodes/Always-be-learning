function(objcopy_target _name)
    if (NOT CMAKE_OBJCOPY)
        message(FATAL_ERROR "CMAKE_OBJCOPY required to create an objcopy_target post-build command")
    endif()
    get_filename_component(_outName $<TARGET_FILE:${_name}> NAME_WLE)
    get_filename_component(_outDir $<TARGET_FILE:${_name} DIRECTORY)
    set(_outFile "${_outDir}${_outName}.bin")
    add_custom_command(TARGET ${_name} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY}
            $<TARGET_FILE:${_name}>
            -O binary ${_outFile}
        COMMENT "Convert $<TARGET_FILE:${_name} into a binary image"
    )
endfunction()
