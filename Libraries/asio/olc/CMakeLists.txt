cmake_minimum_required(VERSION 3.15)
project(LearningASIO_OLC)

add_subdirectory(networking)

add_executable(LearningASIO_OLC_simple_asio)
set_target_properties(LearningASIO_OLC_simple_asio
    PROPERTIES
        EXPORT_NAME olc_simple_asio
        OUTPUT_NAME olc_simple_asio
)
target_sources(LearningASIO_OLC_simple_asio
    PRIVATE
        simple_asio.cpp
)
target_link_libraries(LearningASIO_OLC_simple_asio
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)
