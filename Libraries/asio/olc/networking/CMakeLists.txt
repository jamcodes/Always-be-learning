cmake_minimum_required(VERSION 3.15)

project(OLC_Networking)

add_library(OLC_Networking INTERFACE)
add_library(OLC::Networking ALIAS OLC_Networking)
set_target_properties(OLC_Networking
    PROPERTIES
        EXPORT_NAME olc_networking
)
target_sources(OLC_Networking
    INTERFACE
        include/networking/net_common.h
        include/networking/net_client.h
        include/networking/net_connection.h
        include/networking/net_message.h
        include/networking/net_server.h
        include/networking/net_tsqueue.h
        include/olc_net.h
)
target_include_directories(OLC_Networking
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(OLC_Networking
    INTERFACE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)


add_executable(OLC_Networking_Client)
set_target_properties(OLC_Networking_Client
    PROPERTIES
        EXPORT_NAME olc_networking_client
        OUTPUT_NAME olc_networking_client
)
target_sources(OLC_Networking_Client
    PRIVATE
        net_client.cpp
)
target_link_libraries(OLC_Networking_Client
    PRIVATE
        OLC::Networking
)

add_executable(OLC_Networking_Server)
set_target_properties(OLC_Networking_Server
    PROPERTIES
        EXPORT_NAME olc_networking_server
        OUTPUT_NAME olc_networking_server
)
target_sources(OLC_Networking_Server
    PRIVATE
        net_server.cpp
)
target_link_libraries(OLC_Networking_Server
    PRIVATE
        OLC::Networking
)
