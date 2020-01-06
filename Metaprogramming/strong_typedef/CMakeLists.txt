cmake_minimum_required(VERSION 3.15)
project(strong_typedef)

add_executable(strong_typedef)
target_sources(strong_typedef
    PRIVATE
        strong_typedef.hpp
        main.cpp
)
target_include_directories(strong_typedef
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)
target_compile_features(strong_typedef
    PUBLIC
        cxx_std_17
)
target_compile_options(strong_typedef
    PRIVATE
        -Wall
        -Wextra
        -Weffc++
        -Wpedantic
        -pedantic-errors
)
