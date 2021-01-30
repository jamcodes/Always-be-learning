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
