cmake_minimum_required(VERSION 3.15)

project(OLC_Networking_Client)

add_executable(OLC_Networking_Client)
set_target_properties(OLC_Networking_Client
    PROPERTIES
        EXPORT_NAME olc_networking_clinet
        OUTPUT_NAME olc_networking_client
)
target_sources(OLC_Networking_Client
    PRIVATE
        client.cpp
)
target_include_directories(OLC_Networking_Client
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(OLC_Networking_Client
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)
