cmake_minimum_required(VERSION 3.15)
project(Zetcode_Painting)

include(copy_if_different)

# CopyIfDifferent(${PROJECT_NAME}_CopyAssets
#     OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}/assets
#     FILES
#         ${LearningQt5_SOURCE_DIR}/assets/kiwi.png
#     NO_DEPENDS
# )

zetcode_executable(lines SOURCES lines.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(colors SOURCES colors.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(patterns SOURCES patterns.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(transparency SOURCES transparency.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(donut SOURCES donut.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(shapes SOURCES shapes.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(linear_gradients SOURCES linear_gradients.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(radial_gradients SOURCES radial_gradients.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(puff SOURCES puff.cpp LIBS Qt5::Core Qt5::Widgets)
