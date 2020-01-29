cmake_minimum_required(VERSION 3.15)
project(bind_front)

add_executable(bind_front)
target_sources(bind_front PRIVATE
    main.cpp
    bind_front.hpp
)
target_compile_features(bind_front PRIVATE
    cxx_std_17
)
target_compile_options(bind_front PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Weffc++
)
target_include_directories(bind_front PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}
)
