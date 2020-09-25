cmake_minimum_required(VERSION 3.15)

add_executable(BinaryTree_UT
    ut_binary_tree.cpp
)
target_link_libraries(BinaryTree_UT
    PRIVATE
        BinaryTree::BinaryTree
        gtest
        gtest_main
        DataStructures::CompilerConfig
)

add_test(NAME BinaryTree_UT COMMAND BinaryTree_UT)
