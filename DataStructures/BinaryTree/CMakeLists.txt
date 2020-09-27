cmake_minimum_required(VERSION 3.15)
project(BinaryTree)

add_library(BinaryTree_BinaryTree INTERFACE)
add_library(BinaryTree::BinaryTree ALIAS BinaryTree_BinaryTree)

target_link_libraries(BinaryTree_BinaryTree
    INTERFACE
        DataStructures::Utils
)

add_subdirectory(include)
add_subdirectory(src)

include(CTest)
if (BUILD_TESTING)
    add_subdirectory(tests)
endif()

