cmake_minimum_required(VERSION 3.15)
project(structured_bindings)

add_executable(structured_bindings)
target_sources(structured_bindings
    PRIVATE
        structured_bindings.hpp
        main.cpp
)
target_include_directories(structured_bindings
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)
target_compile_features(structured_bindings
    PUBLIC
        cxx_std_17
)
target_compile_options(structured_bindings
    PRIVATE
        -Wall
        -Wextra
        -Weffc++
        -Wpedantic
        -pedantic-errors
)
