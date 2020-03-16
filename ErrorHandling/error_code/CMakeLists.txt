cmake_minimum_required(VERSION 3.15)
project(error_code)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE

)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wdouble-promotion
        -Weffc++
        -Wextra
        -Wformat=2
        -Wnon-virtual-dtor
        -Wnull-dereference
        -Wold-style-cast
        -Woverloaded-virtual
        -Wpedantic
        -pedantic-errors
        -Wimplicit-fallthrough
        -Wsuggest-attribute=pure
        -Wsuggest-attribute=const
        -Wsuggest-attribute=cold
        -Wsuggest-final-types
        -Wsuggest-final-methods
        -Wduplicated-branches
        -Wduplicated-cond
        -fuse-ld=gold
)
