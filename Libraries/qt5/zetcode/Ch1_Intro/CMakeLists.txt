cmake_minimum_required(VERSION 3.15)

zetcode_executable(version SOURCES version.cpp LIBS Qt5::Core)
zetcode_executable(simple SOURCES simple.cpp LIBS Qt5::Core Qt5::Widgets)
