cmake_minimum_required(VERSION 3.15)
project(Zetcode_Breakout)

include(copy_if_different)

CopyIfDifferent(${PROJECT_NAME}_CopyAssets
    OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/paddle.png
        ${CMAKE_CURRENT_LIST_DIR}/brick.png
        ${CMAKE_CURRENT_LIST_DIR}/ball.png
    NO_DEPENDS
)

zetcode_executable(breakout
    SOURCES
        paddle.h
        paddle.cpp
        brick.h
        brick.cpp
        ball.h
        ball.cpp
        breakout.h
        breakout.cpp
        main.cpp
    LIBS
        Qt5::Core Qt5::Widgets
    AUTOMOC
    DEPENDS ${PROJECT_NAME}_CopyAssets
)
