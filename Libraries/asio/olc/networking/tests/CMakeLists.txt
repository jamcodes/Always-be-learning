cmake_minimum_required(VERSION 3.15)


add_executable(OLC_Networking_tests)
target_sources(OLC_Networking_tests
    PRIVATE
        old_networking_tests.cpp
)
target_link_libraries(OLC_Networking_tests
    PRIVATE
        OLC::Networking
)
