cmake_minimum_required(VERSION 3.15)

add_executable(LearningASIO_main)
set_target_properties(LearningASIO_main
    PROPERTIES
        EXPORT_NAME main
        OUTPUT_NAME main
)
target_sources(LearningASIO_main
    PRIVATE
        main.cpp
)
target_link_libraries(LearningASIO_main
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)
