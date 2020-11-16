cmake_minimum_required(VERSION 3.15)
project(LearningASIO_timer)

add_executable(LearningASIO_timer)
set_target_properties(LearningASIO_timer
    PROPERTIES EXPORT_NAME timer_1 OUTPUT_NAME timer_1
)
target_sources(LearningASIO_timer
    PRIVATE
        timer_1.cpp
)
target_link_libraries(LearningASIO_timer
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)


add_executable(LearningASIO_timer_2)
set_target_properties(LearningASIO_timer_2
    PROPERTIES EXPORT_NAME timer_2 OUTPUT_NAME timer_2
)
target_sources(LearningASIO_timer_2
    PRIVATE
        timer_2.cpp
)
target_link_libraries(LearningASIO_timer_2
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)

add_executable(LearningASIO_timer_3)
set_target_properties(LearningASIO_timer_3
    PROPERTIES EXPORT_NAME timer_3 OUTPUT_NAME timer_3
)
target_sources(LearningASIO_timer_3
    PRIVATE
        timer_3.cpp
)
target_link_libraries(LearningASIO_timer_3
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)

add_executable(LearningASIO_timer_4)
set_target_properties(LearningASIO_timer_4
    PROPERTIES EXPORT_NAME timer_4 OUTPUT_NAME timer_4
)
target_sources(LearningASIO_timer_4
    PRIVATE
        timer_4.cpp
)
target_link_libraries(LearningASIO_timer_4
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)

add_executable(LearningASIO_timer_5)
set_target_properties(LearningASIO_timer_5
    PROPERTIES EXPORT_NAME timer_5 OUTPUT_NAME timer_5
)
target_sources(LearningASIO_timer_5
    PRIVATE
        timer_5.cpp
)
target_link_libraries(LearningASIO_timer_5
    PRIVATE
        LearningASIO::asio
        LearningASIO::CompilerConfig
        fmt::fmt
)
