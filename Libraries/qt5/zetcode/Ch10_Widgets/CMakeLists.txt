cmake_minimum_required(VERSION 3.15)
project(Zetcode_Widgets)

include(copy_if_different)

# CopyIfDifferent(CopyAssets
#     OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}/assets
#     FILES
#         ${LearningQt5_SOURCE_DIR}/assets/044-open.png
#         ${LearningQt5_SOURCE_DIR}/assets/018-add_file.png
#         ${LearningQt5_SOURCE_DIR}/assets/logout.png
#     NO_DEPENDS
# )

zetcode_executable(label SOURCES label.cpp LIBS Qt5::Core Qt5::Widgets)
