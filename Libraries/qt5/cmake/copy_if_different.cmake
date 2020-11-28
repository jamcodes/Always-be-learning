#[[
Function declaring a target named ${targetName_} that carries dependency to files listed in
argument FILES, copied into directory OUTPUT_DIRECTORY. By default changes to the FILES are
tracked - changing any of the FILES will prompt an update. NO_DEPENDS flag can be given to
disable the tracking - this will only cause the files to be copied if they don't already exist at
the OUTPUT_DIRECTORY
Usage:

-- For scripts, etc. that need to be monitored for changes use the following form:
CopyIfDifferent(CopyShaders
    OUTPUT_DIRECTORY some/output/directory/assets/shaders
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/shader.vs
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/shader.fs
)

-- For files that won't be updated and only need to be copied once add NO_DEPENDS flag
CopyIfDifferent(CopyTextures
    OUTPUT_DIRECTORY some/output/directory/assets/textures
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/awesomeface.png
    NO_DEPENDS
)

These will create `CopyShaders` and `CopyTextures` targets accordingly. These targets can then
be added as a dependency to the library/executable to make sure they're copied where they belong

add_dependencies(MyExecutable CopyShaders CopyTextures)
]]
function(CopyIfDifferent targetName_)
    set(options_ NO_DEPENDS)
    set(kwargs_ OUTPUT_DIRECTORY)
    set(list_args_ FILES)
    cmake_parse_arguments(arg "${options_}" "${kwargs_}" "${list_args_}" ${ARGN})
    foreach(unparsed_arg IN LISTS arg_UNPARSED_ARGUMENTS)
        message(WARNING "${ColorYellow}Unparsed argument: ${unparsed_arg}${ColorReset}")
    endforeach()
    if (NOT arg_FILES)
        message(FATAL_ERROR "${ColorRed}Error: Specify at least one FILES argument${ColorReset}")
    endif()

    get_filename_component(output_directory_ ${arg_OUTPUT_DIRECTORY}
        ABSOLUTE ${CMAKE_CURRENT_BINARY_DIR}
    )
    # message(STATUS "output_directory_ = ${output_directory_}")
    set(outputs_)
    foreach(file_ IN LISTS arg_FILES)
        get_filename_component(file_name_ "${file_}" NAME)
        list(APPEND outputs_ "${output_directory_}/${file_name_}")
    endforeach()

    add_custom_command(OUTPUT ${outputs_})
    foreach(input_ output_ IN ZIP_LISTS arg_FILES outputs_)
        if(NOT arg_NO_DEPENDS)
            add_custom_command(OUTPUT ${outputs_}
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${input_} ${output_}
                DEPENDS ${input_}
                APPEND
            )
        else()
            add_custom_command(OUTPUT ${outputs_}
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${input_} ${output_}
                APPEND
            )
        endif()
    endforeach()

    add_custom_target(${targetName_} DEPENDS ${outputs_})

    # message(STATUS "FILES = ${arg_FILES}")
    # message(STATUS "OUTPUTS = ${outputs_}")
endfunction()

#[[
In comparison, work we'd need to do manually:

add_custom_command(
    OUTPUT
        some/output/directory/assets/shaders/shader.vs
        some/output/directory/assets/shaders/shader.fs
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/shader.vs
        some/output/directory/assets/shaders/shader.vs
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/shader.fs
        some/output/directory/assets/shaders/shader.fs
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/shader.vs
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/shader.fs
)

add_custom_command(
    OUTPUT
        some/output/directory/assets/textures/awesomeface.png
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/awesomeface.png
        some/output/directory/assets/textures/awesomeface.png
)

add_custom_target(CopyShaders
    DEPENDS
        some/output/directory/assets/shaders/shader.vs
        some/output/directory/assets/shaders/shader.fs
)
add_custom_target(CopyTextures
    DEPENDS
        some/output/directory/assets/textures/awesomeface.png
)

add_dependencies(MyExecutable CopyShaders CopyTextures)

This is a multi-stage process with lots of room for error
]]
