cmake_minimum_required(VERSION 3.15)

target_include_directories(HashMap_HashMap
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
