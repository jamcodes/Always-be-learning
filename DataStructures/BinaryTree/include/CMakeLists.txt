cmake_minimum_required(VERSION 3.15)

# target_sources(BinaryTree_BinaryTree
#     PRIVATE
#         binary_tree/binary_tree.hpp
# )
target_include_directories(BinaryTree_BinaryTree
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
