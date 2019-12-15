add_library(${CMAKE_PROJECT_NAME}
    foobar.cpp
)
add_library(${CMAKE_PROJECT_NAME}::${CMAKE_PROJECT_NAME} ALIAS ${CMAKE_PROJECT_NAME})
target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)
target_link_libraries(${CMAKE_PROJECT_NAME}
    PUBLIC
        ${CMAKE_PROJECT_NAME}::Api
)
set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
        SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}    # SOVERSION needs to be set explicitly
        VERSION ${CMAKE_PROJECT_VERSION}
        DEBUG_POSTFIX _d
)

# This will generate a file with a set of macros defined, which can be used to portably
# declare exported symbols in shared libraries.
# This macros will correctly be defined for gcc, clang and MSVC, and will take into account
# if the given target is being exported (during build time) or imported (when consumed by clients)
include(GenerateExportHeader)
generate_export_header(${CMAKE_PROJECT_NAME})
set(export_file_name ${CMAKE_PROJECT_NAME}_export.h)
string(TOLOWER ${export_file_name} export_file_name)
# The generated export header needs to be installed
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${export_file_name}
        DESTINATION ${project_install_include_dir}/foobar/
)
