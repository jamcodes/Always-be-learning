cmake_minimum_required(VERSION 3.15)
project(Qt5Zetcode)

function(zetcode_executable _name)
    set(_flags AUTOMOC)
    set(_args)
    set(_listArgs SOURCES LIBS DEPENDS)
    cmake_parse_arguments(ARG "${_flags}" "${_args}" "${_listArgs}" ${ARGN})

    foreach(unparsed_arg IN LISTS ARG_UNPARSED_ARGUMENTS)
        message(WARNING "[server_client_executable] Unparsed Argument: ${unparsed_arg}")
    endforeach()

    add_executable(${PROJECT_NAME}_${_name} ${ARG_SOURCES})
    set_target_properties(${PROJECT_NAME}_${_name}
        PROPERTIES
            EXPORT_NAME ${_name}
            OUTPUT_NAME ${_name}
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}
    )
    target_link_libraries(${PROJECT_NAME}_${_name}
        PRIVATE
            ${ARG_LIBS}
    )

    if (ARG_AUTOMOC)
        set_target_properties(${PROJECT_NAME}_${_name}
            PROPERTIES AUTOMOC ${ARG_AUTOMOC}
        )
    endif()

    if (ARG_DEPENDS)
        add_dependencies(${PROJECT_NAME}_${_name} ${ARG_DEPENDS})
    endif()
endfunction()

add_subdirectory(Ch1_Intro)
add_subdirectory(Ch2_Strings)
add_subdirectory(Ch3_DateTime)
add_subdirectory(Ch4_Containers)
add_subdirectory(Ch5_FilesDirs)
add_subdirectory(Ch6_FirstApps)
add_subdirectory(Ch7_MenusToolbars)
add_subdirectory(Ch8_Layouts)
add_subdirectory(Ch9_EventsSignals)
add_subdirectory(Ch10_Widgets)
add_subdirectory(Ch11_Painting)
