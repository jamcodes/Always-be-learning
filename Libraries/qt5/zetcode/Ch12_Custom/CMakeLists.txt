cmake_minimum_required(VERSION 3.15)
project(Zetcode_Custom)

include(copy_if_different)

# CopyIfDifferent(${PROJECT_NAME}_CopyAssets
#     OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}/assets
#     FILES
#         ${LearningQt5_SOURCE_DIR}/assets/kiwi.png
#     NO_DEPENDS
# )

zetcode_executable(burning
    SOURCES
        application.h
        widget.h
        widget.cpp
        application.cpp
        main.cpp
    LIBS
        Qt5::Core Qt5::Widgets
    AUTOMOC
)
