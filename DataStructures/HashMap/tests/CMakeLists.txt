cmake_minimum_required(VERSION 3.15)

add_executable(HashMapBucket_UT ut_hash_map_bucket.cpp)
target_link_libraries(HashMapBucket_UT
    PRIVATE
        HashMap::HashMap
        gtest
        gtest_main
        DataStructures::CompilerConfig
)
add_test(NAME HashMapBucket_UT COMMAND HashMapBucket_UT)


add_executable(HashMap_UT ut_hash_map.cpp)
target_link_libraries(HashMap_UT
    PRIVATE
        HashMap::HashMap
        gtest
        gtest_main
        DataStructures::CompilerConfig
)
add_test(NAME HashMap_UT COMMAND HashMap_UT)
