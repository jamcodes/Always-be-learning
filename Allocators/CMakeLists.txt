cmake_minimum_required(VERSION 3.15)
project(Allocators)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(A5)
add_subdirectory(memory)

add_executable(memory_example_1 memory_example_1.cpp)
target_link_libraries(memory_example_1
    PRIVATE
        foonathan_memory
)

add_executable(short_alloc short_alloc.cpp)
target_include_directories(short_alloc
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/short_alloc
)
