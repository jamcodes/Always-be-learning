cmake_minimum_required(VERSION 3.15)
project(Zetcode_Widgets)

include(copy_if_different)

CopyIfDifferent(${PROJECT_NAME}_CopyAssets
    OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}/assets
    FILES
        ${LearningQt5_SOURCE_DIR}/assets/kiwi.png
    NO_DEPENDS
)


zetcode_executable(label SOURCES label.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(slider SOURCES slider.cpp LIBS Qt5::Core Qt5::Widgets AUTOMOC)
zetcode_executable(combobox SOURCES combobox.cpp LIBS Qt5::Core Qt5::Widgets AUTOMOC)
zetcode_executable(spinbox SOURCES spinbox.cpp LIBS Qt5::Core Qt5::Widgets AUTOMOC)
zetcode_executable(lineedit SOURCES lineedit.cpp LIBS Qt5::Core Qt5::Widgets AUTOMOC)
zetcode_executable(statusbar SOURCES statusbar.cpp LIBS Qt5::Core Qt5::Widgets AUTOMOC)
zetcode_executable(checkbox SOURCES checkbox.cpp LIBS Qt5::Core Qt5::Widgets AUTOMOC)
zetcode_executable(listwidget SOURCES listwidget.cpp LIBS Qt5::Core Qt5::Widgets AUTOMOC)
zetcode_executable(progressbar SOURCES progressbar.cpp LIBS Qt5::Core Qt5::Widgets AUTOMOC)
zetcode_executable(pixmap SOURCES pixmap.cpp LIBS Qt5::Core Qt5::Widgets DEPENDS ${PROJECT_NAME}_CopyAssets AUTOMOC)
zetcode_executable(splitter SOURCES splitter.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(tablewidget SOURCES tablewidget.cpp LIBS Qt5::Core Qt5::Widgets AUTOMOC)
