cmake_minimum_required(VERSION 3.15)
project(Qt5Zetcode)

function(zetcode_executable _name)
    set(_flags)
    set(_args)
    set(_listArgs SOURCES LIBS)
    cmake_parse_arguments(ARG "${_flags}" "${_args}" "${_listArgs}" ${ARGN})

    foreach(unparsed_arg IN LISTS ARG_UNPARSED_ARGUMENTS)
        message(WARNING "[server_client_executable] Unparsed Argument: ${unparsed_arg}")
    endforeach()

    add_executable(${PROJECT_NAME}_${_name} ${ARG_SOURCES})
    set_target_properties(${PROJECT_NAME}_${_name}
        PROPERTIES
            EXPORT_NAME ${_name}
            OUTPUT_NAME ${_name}
    )
    target_link_libraries(${PROJECT_NAME}_${_name}
        PRIVATE
            ${ARG_LIBS}
    )
endfunction()


add_subdirectory(Ch1_Intro)
add_subdirectory(Ch2_Strings)
