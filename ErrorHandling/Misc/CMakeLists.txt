cmake_minimum_required(VERSION 3.15)
project(misc_error_handling_snippets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(${PROJECT_NAME}
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
