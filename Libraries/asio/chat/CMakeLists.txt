cmake_minimum_required(VERSION 3.15)
project(LearningASIO_chat)

function(chat_executable name)
set(_flags)
    set(_args)
    set(_listArgs SOURCES EXTRA_LIBS)
    cmake_parse_arguments(ARG "${_flags}" "${_args}" "${_listArgs}" ${ARGN})

    foreach(unparsed_arg IN LISTS ARG_UNPARSED_ARGUMENTS)
        message(WARNING "[chat_executable] Unparsed Argument: ${unparsed_arg}")
    endforeach()

    add_executable(LearningASIO_chat_${name})
    set_target_properties(LearningASIO_chat_${name}
        PROPERTIES EXPORT_NAME ${name} OUTPUT_NAME ${name}
    )
    target_sources(LearningASIO_chat_${name}
        PRIVATE
            ${ARG_SOURCES}
    )
    target_link_libraries(LearningASIO_chat_${name}
        PRIVATE
            LearningASIO::asio
            LearningASIO::CompilerConfig
            fmt::fmt
            ${ARG_EXTRA_LIBS}
    )
    
endfunction()

chat_executable(message_test SOURCES message_test.cpp)
chat_executable(chat_client SOURCES chat_client.cpp)
chat_executable(chat_server SOURCES chat_server.cpp)
target_compile_options(LearningASIO_chat_chat_server
    PRIVATE
        -Wno-non-virtual-dtor
        -Wno-error=non-virtual-dtor
)
