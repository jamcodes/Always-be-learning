cmake_minimum_required(VERSION 3.15)
project(Zetcode_Containers)

zetcode_executable(myvector SOURCES myvector.cpp LIBS Qt5::Core)
zetcode_executable(mylist SOURCES mylist.cpp LIBS Qt5::Core)
zetcode_executable(mystringlist SOURCES mystringlist.cpp LIBS Qt5::Core)
zetcode_executable(myset SOURCES myset.cpp LIBS Qt5::Core)
zetcode_executable(mymap SOURCES mymap.cpp LIBS Qt5::Core)
zetcode_executable(custom_class_sorting SOURCES custom_class_sorting.cpp LIBS Qt5::Core)
