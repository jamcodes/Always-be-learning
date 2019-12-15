cmake_minimum_required(VERSION 3.14)

project(Foobar VERSION 0.1.0 LANGUAGES CXX)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(config)
include(options)
include(release)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_VISIBILITY_PRESET     hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
SetOptions()
SetDefaultBuildType()
DeclareCompilerConfigTarget(${CMAKE_PROJECT_NAME}_flags)
DeclareInstallDirs()
add_library(${CMAKE_PROJECT_NAME}::Flags ALIAS ${CMAKE_PROJECT_NAME}_flags)

# Tell the linker to fill DT_RUNPATH with the directory where defined targets will be installed
# Could put this in SetOptions() or DeclareInstallDirs()
set(CMAKE_INSTALL_RPATH $ORIGIN)    # $ORIGIN is not a CMake thing - it's a linker/system setting

add_subdirectory(api)
add_subdirectory(src)

InstallProject()
