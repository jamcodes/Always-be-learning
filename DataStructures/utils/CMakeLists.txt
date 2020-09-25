cmake_minimum_required(VERSION 3.15)

add_library(DataStructures_Utils INTERFACE)
add_library(DataStructures::Utils ALIAS DataStructures_Utils)
target_include_directories(DataStructures_Utils
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
