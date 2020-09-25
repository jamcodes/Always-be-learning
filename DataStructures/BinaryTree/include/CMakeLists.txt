cmake_minimum_required(VERSION 3.15)

# target_sources(BinaryTree_BinaryTree
#     PRIVATE
#         single_linked_list/single_linked_list.hpp
# )
target_include_directories(BinaryTree_BinaryTree
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
