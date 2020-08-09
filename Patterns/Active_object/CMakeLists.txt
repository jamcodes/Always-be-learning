cmake_minimum_required(VERSION 3.15)
project(ActiveObject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(active
    active.h
    active.cpp
    main.cpp
)

target_compile_options(active
    PRIVATE
        -Wall -Wextra -Weffc++ -Wpedantic
)

