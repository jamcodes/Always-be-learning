cmake_minimum_required(VERSION 3.15)

function(low_level_executable name)
    set(_flags)
    set(_args)
    set(_listArgs SOURCES DEPS)
    cmake_parse_arguments(ARG "${_flags}" "${_args}" "${_listArgs}" ${ARGN})

    foreach(unparsed_arg IN LISTS ARG_UNPARSED_ARGUMENTS)
        message(WARNING "[low_level_executable] Unparsed Argument: ${unparsed_arg}")
    endforeach()

    add_executable(LearningSockets_low_level_${name})
    set_target_properties(LearningSockets_low_level_${name}
        PROPERTIES EXPORT_NAME ${name} OUTPUT_NAME ${name}
    )
    target_sources(LearningSockets_low_level_${name}
        PRIVATE
            ${ARG_SOURCES}
    )
    target_link_libraries(LearningSockets_low_level_${name}
        PRIVATE
            LearningSockets::CompilerConfig
            fmt::fmt
            ${ARG_DEPS}
    )
endfunction()

# ---------------------------------------------------------------------------------------------------------------------

low_level_executable(showip SOURCES showip.cpp)
low_level_executable(hello_server SOURCES hello_server.cpp)
low_level_executable(hello_client SOURCES hello_client.cpp)
low_level_executable(dgram_listener SOURCES dgram_listener.cpp)
low_level_executable(dgram_talker SOURCES dgram_talker.cpp)
low_level_executable(poll_example SOURCES poll_example.cpp)
low_level_executable(poll_server SOURCES poll_server.cpp)
low_level_executable(select_demo SOURCES select_demo.cpp)
low_level_executable(selectserver SOURCES selectserver.cpp)
low_level_executable(float_serialization SOURCES float_serialization.cpp)
target_compile_options(LearningSockets_low_level_float_serialization
    PRIVATE
        -Wno-error=conversion
        -Wno-error=float-conversion
)
