cmake_minimum_required(VERSION 3.15)
project(LearningASIO_server_client)

function(server_client_executable name)
    set(_flags)
    set(_args)
    set(_listArgs SOURCES EXTRA_LIBS)
    cmake_parse_arguments(ARG "${_flags}" "${_args}" "${_listArgs}" ${ARGN})

    foreach(unparsed_arg IN LISTS ARG_UNPARSED_ARGUMENTS)
        message(WARNING "[server_client_executable] Unparsed Argument: ${unparsed_arg}")
    endforeach()

    add_executable(LearningASIO_server_client_${name})
    set_target_properties(LearningASIO_server_client_${name}
        PROPERTIES EXPORT_NAME ${name} OUTPUT_NAME ${name}
    )
    target_sources(LearningASIO_server_client_${name}
        PRIVATE
            ${ARG_SOURCES}
    )
    target_link_libraries(LearningASIO_server_client_${name}
        PRIVATE
            LearningASIO::asio
            LearningASIO::CompilerConfig
            fmt::fmt
            ${ARG_EXTRA_LIBS}
    )
endfunction()


server_client_executable(tcp_daytime_client SOURCES tcp_daytime_client.cpp)
server_client_executable(tcp_daytime_server SOURCES tcp_daytime_server.cpp)
server_client_executable(tcp_daytime_async_server SOURCES tcp_daytime_async_server.cpp)
server_client_executable(udp_daytime_client SOURCES udp_daytime_client.cpp)
server_client_executable(udp_daytime_server SOURCES udp_daytime_server)
server_client_executable(udp_daytime_async_server SOURCES udp_daytime_async_server)
server_client_executable(tcp_udp_daytime_async_server SOURCES tcp_udp_daytime_async_server)
server_client_executable(udp_daytime_future SOURCES udp_daytime_future.cpp)
server_client_executable(tcp_server_shared_buffer SOURCES tcp_server_shared_buffer.cpp)
server_client_executable(multicast_sender SOURCES multicast_sender.cpp)
server_client_executable(multicast_receiver SOURCES multicast_receiver.cpp)
server_client_executable(tcp_echo_server_async SOURCES tcp_echo_server_async.cpp)
