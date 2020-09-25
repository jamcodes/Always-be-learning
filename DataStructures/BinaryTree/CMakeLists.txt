cmake_minimum_required(VERSION 3.15)
project(SingleLinkedList)

add_library(SingleLinkedList_SingleLinkedList INTERFACE)
add_library(SingleLinkedList::SingleLinkedList ALIAS SingleLinkedList_SingleLinkedList)

target_link_libraries(SingleLinkedList_SingleLinkedList
    INTERFACE
        DataStructures::Utils
)

add_subdirectory(include)
add_subdirectory(src)

include(CTest)
if (BUILD_TESTING)
    add_subdirectory(tests)
endif()

