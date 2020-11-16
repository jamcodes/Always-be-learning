cmake_minimum_required(VERSION 3.15)
project(LearningASIO_server_client)

add_executable(LearningASIO_tcp_daytime_client)
set_target_properties(LearningASIO_tcp_daytime_client
    PROPERTIES EXPORT_NAME tcp_daytime_client OUTPUT_NAME tcp_daytime_client
)
target_sources(LearningASIO_tcp_daytime_client
    PRIVATE
        tcp_daytime_client.cpp
)
target_link_libraries(LearningASIO_tcp_daytime_client
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)

add_executable(LearningASIO_tcp_daytime_server)
set_target_properties(LearningASIO_tcp_daytime_server
    PROPERTIES EXPORT_NAME tcp_daytime_server OUTPUT_NAME tcp_daytime_server
)
target_sources(LearningASIO_tcp_daytime_server
    PRIVATE
        tcp_daytime_server.cpp
)
target_link_libraries(LearningASIO_tcp_daytime_server
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)

add_executable(LearningASIO_tcp_daytime_async_server)
set_target_properties(LearningASIO_tcp_daytime_async_server
    PROPERTIES EXPORT_NAME tcp_daytime_async_server OUTPUT_NAME tcp_daytime_async_server
)
target_sources(LearningASIO_tcp_daytime_async_server
    PRIVATE
        tcp_daytime_async_server.cpp
)
target_link_libraries(LearningASIO_tcp_daytime_async_server
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)


add_executable(LearningASIO_udp_daytime_client)
set_target_properties(LearningASIO_udp_daytime_client
    PROPERTIES EXPORT_NAME udp_daytime_client OUTPUT_NAME udp_daytime_client
)
target_sources(LearningASIO_udp_daytime_client
    PRIVATE
        udp_daytime_client.cpp
)
target_link_libraries(LearningASIO_udp_daytime_client
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)

add_executable(LearningASIO_udp_daytime_server)
set_target_properties(LearningASIO_udp_daytime_server
    PROPERTIES EXPORT_NAME udp_daytime_server OUTPUT_NAME udp_daytime_server
)
target_sources(LearningASIO_udp_daytime_server
    PRIVATE
        udp_daytime_server.cpp
)
target_link_libraries(LearningASIO_udp_daytime_server
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)

add_executable(LearningASIO_udp_daytime_async_server)
set_target_properties(LearningASIO_udp_daytime_async_server
    PROPERTIES EXPORT_NAME udp_daytime_async_server OUTPUT_NAME udp_daytime_async_server
)
target_sources(LearningASIO_udp_daytime_async_server
    PRIVATE
        udp_daytime_async_server.cpp
)
target_link_libraries(LearningASIO_udp_daytime_async_server
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)

add_executable(LearningASIO_tcp_udp_daytime_async_server)
set_target_properties(LearningASIO_tcp_udp_daytime_async_server
    PROPERTIES EXPORT_NAME tcp_udp_daytime_async_server OUTPUT_NAME tcp_udp_daytime_async_server
)
target_sources(LearningASIO_tcp_udp_daytime_async_server
    PRIVATE
        tcp_udp_daytime_async_server.cpp
)
target_link_libraries(LearningASIO_tcp_udp_daytime_async_server
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)
