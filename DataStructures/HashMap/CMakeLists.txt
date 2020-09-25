cmake_minimum_required(VERSION 3.15)
project(HashMap)

add_library(HashMap_HashMap INTERFACE)
add_library(HashMap::HashMap ALIAS HashMap_HashMap)

target_link_libraries(HashMap_HashMap
    INTERFACE
        DataStructures::Utils
)

add_subdirectory(include)
add_subdirectory(src)

include(CTest)
if (BUILD_TESTING)
    add_subdirectory(tests)
endif()

