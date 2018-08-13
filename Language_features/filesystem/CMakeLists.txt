cmake_minimum_required(VERSION 3.1)

project(BoostFilesystemTutorial)

# find a boost install with the libraries filesystem and system
find_package(Boost 1.60 REQUIRED COMPONENTS system filesystem)

# check if boost was found
if(Boost_FOUND)
    message ("boost found")
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message (FATAL_ERROR "Cannot find Boost")
endif()

set( CXX_STANDARD 17 )
set(CMAKE_BUILD_TYPE RelWithDebInfo)

file( GLOB target_sources RELATIVE "${PROJECT_SOURCE_DIR}"
      "${PROJECT_SOURCE_DIR}/*.cpp" )

foreach( target ${target_sources} )
  string(REGEX MATCH "^[^ .]*" fname ${target} )
  MESSAGE( STATUS "Executable: ${fname}" )
  add_executable( ${fname} ${target} )
  target_compile_definitions( ${fname} PUBLIC Wall Wextra Wpedantic )
  target_link_libraries( ${fname} ${Boost_LIBRARIES} )
#   target_include_directories( ${fname} PUBLIC ${PROJECT_SOURCE_DIR} )
endforeach(target)