cmake_minimum_required(VERSION 3.13)

project(contract_macros VERSION 0.1.0)

set(contract_macros_headers
    contract_macro.h
)
set(contract_macros_sources
)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>
)

add_executable(contract_macros_test main.cpp)
target_link_libraries(contract_macros_test
    PRIVATE
        ${PROJECT_NAME}::${PROJECT_NAME}
)
