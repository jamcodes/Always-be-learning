cmake_minimum_required(VERSION 3.15)

zetcode_executable(basic SOURCES basic.cpp LIBS Qt5::Core)
zetcode_executable(init SOURCES init.cpp LIBS Qt5::Core)
zetcode_executable(access SOURCES access.cpp LIBS Qt5::Core)
zetcode_executable(length SOURCES length.cpp LIBS Qt5::Core)
zetcode_executable(building SOURCES building.cpp LIBS Qt5::Core)
zetcode_executable(substrings SOURCES substrings.cpp LIBS Qt5::Core)
zetcode_executable(looping SOURCES looping.cpp LIBS Qt5::Core)
zetcode_executable(comparison SOURCES comparison.cpp LIBS Qt5::Core)
zetcode_executable(converting SOURCES converting.cpp LIBS Qt5::Core)
zetcode_executable(qchar_types SOURCES qchar_types.cpp LIBS Qt5::Core)
zetcode_executable(justification SOURCES justification.cpp LIBS Qt5::Core)
zetcode_executable(html_escape SOURCES html_escape.cpp LIBS Qt5::Core)

set_target_properties(${PROJECT_NAME}_html_escape
    PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
