cmake_minimum_required(VERSION 3.15)
project(memory_management)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(config.cmake)

add_subdirectory(tracking_allocation)
