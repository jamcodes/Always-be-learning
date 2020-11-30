cmake_minimum_required(VERSION 3.15)
project(Zetcode_EventsSignals)

include(copy_if_different)

# CopyIfDifferent(CopyAssets
#     OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}/assets
#     FILES
#         ${LearningQt5_SOURCE_DIR}/assets/044-open.png
#         ${LearningQt5_SOURCE_DIR}/assets/018-add_file.png
#         ${LearningQt5_SOURCE_DIR}/assets/logout.png
#     NO_DEPENDS
# )

zetcode_executable(click SOURCES click.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(keypress SOURCES keypress.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(move SOURCES move.cpp LIBS Qt5::Core Qt5::Widgets AUTOMOC)
zetcode_executable(disconnect SOURCES disconnect.cpp LIBS Qt5::Core Qt5::Widgets AUTOMOC)
zetcode_executable(timer SOURCES timer.cpp LIBS Qt5::Core Qt5::Widgets AUTOMOC)
