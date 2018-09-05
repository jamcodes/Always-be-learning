cmake_minimum_required(VERSION 3.0)

project(Clara)

# Prepare "Clara" library for other executables
add_library(Clara INTERFACE)
add_library(Clara::Clara ALIAS Clara)
target_include_directories(Clara INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(Clara
    PROPERTIES
    CMAKE_CXX_CLANG_TIDY ""
    CMAKE_CXX_CPPCHECK ""
)