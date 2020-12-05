cmake_minimum_required(VERSION 3.15)
project(Zetcode_Snake)

include(copy_if_different)

CopyIfDifferent(${PROJECT_NAME}_CopyAssets
    OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}/
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/dot.png
        ${CMAKE_CURRENT_LIST_DIR}/head.png
        ${CMAKE_CURRENT_LIST_DIR}/apple.png
    NO_DEPENDS
)

zetcode_executable(snake
    SOURCES
        snake.h
        snake.cpp
        main.cpp
    LIBS
        Qt5::Core Qt5::Widgets
    AUTOMOC
    DEPENDS ${PROJECT_NAME}_CopyAssets
)
