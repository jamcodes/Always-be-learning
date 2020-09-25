cmake_minimum_required(VERSION 3.15)

# target_sources(SingleLinkedList_SingleLinkedList
#     PRIVATE
#         single_linked_list/single_linked_list.hpp
# )
target_include_directories(SingleLinkedList_SingleLinkedList
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
