project(trace LANGUAGES CXX)

set(TRACE_LEVEL "Fatal" CACHE STRING "Global debug logs level")
if (NOT TRACE_LEVEL STREQUAL "Off")
    set(ENABLE_TRACE_LOGS true)
endif()
configure_file(trace_level.h.in ${CMAKE_CURRENT_BINARY_DIR}/trace_level.h @ONLY)

add_library(${PROJECT_NAME} INTERFACE)
add_library(trace::trace ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
