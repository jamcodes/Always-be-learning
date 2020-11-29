cmake_minimum_required(VERSION 3.15)
project(Zetcode_Layouts)

include(copy_if_different)

# CopyIfDifferent(CopyAssets
#     OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}/assets
#     FILES
#         ${LearningQt5_SOURCE_DIR}/assets/044-open.png
#         ${LearningQt5_SOURCE_DIR}/assets/018-add_file.png
#         ${LearningQt5_SOURCE_DIR}/assets/logout.png
#     NO_DEPENDS
# )

zetcode_executable(absolute SOURCES absolute.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(vboxlayout SOURCES vboxlayout.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(buttons SOURCES buttons.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(nesting SOURCES nesting.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(form SOURCES form.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(grid SOURCES grid.cpp LIBS Qt5::Core Qt5::Widgets)
zetcode_executable(review SOURCES review.cpp LIBS Qt5::Core Qt5::Widgets)
