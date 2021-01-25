cmake_minimum_required(VERSION 3.15)

add_library(${CMAKE_PROJECT_NAME}_Utils INTERFACE)
add_library(${CMAKE_PROJECT_NAME}::Utils ALIAS ${CMAKE_PROJECT_NAME}_Utils)
target_include_directories(${CMAKE_PROJECT_NAME}_Utils
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
