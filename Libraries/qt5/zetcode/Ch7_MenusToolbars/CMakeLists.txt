cmake_minimum_required(VERSION 3.15)
project(Zetcode_MenusToolbars)

include(copy_if_different)

CopyIfDifferent(CopyAssets
    OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}/assets
    FILES
        ${LearningQt5_SOURCE_DIR}/assets/044-open.png
        ${LearningQt5_SOURCE_DIR}/assets/018-add_file.png
        ${LearningQt5_SOURCE_DIR}/assets/logout.png
    NO_DEPENDS
)

zetcode_executable(simplemenu SOURCES simplemenu.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(anothermenu SOURCES anothermenu.cpp LIBS Qt5::Core Qt5::Widgets DEPENDS CopyAssets)
zetcode_executable(checkable SOURCES checkable.cpp LIBS Qt5::Core Qt5::Widgets AUTOMOC)
zetcode_executable(toolbar SOURCES toolbar.cpp LIBS Qt5::Core Qt5::Widgets DEPENDS CopyAssets AUTOMOC)
zetcode_executable(skeleton
    SOURCES
        skeleton.h
        skeleton.cpp
        skeleton_main.cpp
    LIBS Qt5::Core Qt5::Widgets
    DEPENDS CopyAssets
    AUTOMOC
)
