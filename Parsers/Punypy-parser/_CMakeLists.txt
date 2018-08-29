cmake_minimum_required( VERSION 3.1 )

set( Project puny_interpreter )
project( ${Project} )


###############################################################################
# Prepare external dependencies
###############################################################################
# If using any external libraries create a 'external'
# subdir with an appropriate CMakeLists and add the following
# for each library
# add_subdirectory( external/some_lib )
add_subdirectory( external/google-test )


# Find any external libraries via find_backage
# see cmake --help-module-list and cmake --help-module ModuleName
# for details on a specific module
# If using boost
# find_package( Boost 1.65.0
#   REQUIRED COMPONENTS
#   filesystem system
#   )

# if( Boost_FOUND )
#   include_directories( ${Boost_INCLUDE_DIRS} )
# else()
#   message( FATAL_ERROR "Cannot find Boost" )
# endif()


###############################################################################
# Prepare source files for build
###############################################################################
# Create a Sources variable to all the cpp files necessary
# to build the target
set( MainSources
    ${PROJECT_SOURCE_DIR}/src/main.cpp
   )

# Add all of the remaining source files to be build as a static library
# linked against the main executable
# set( LibrarySources
#      ${PROJECT_SOURCE_DIR}/src/small_test.cpp
#    )
# Or let all of the source files be globed, minus the one given in MainSources
file( GLOB LibrarySources
      "${PROJECT_SOURCE_DIR}/src/*.cpp"
    )
list( REMOVE_ITEM LibrarySources ${MainSources} )


###############################################################################
# Configure build
###############################################################################
# Set required C++ standard
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
# Set build type
if( NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'Debug' as none was specified.")
  set( CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Export compile_commands.json for use with cppcheck
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( CMAKE_VERBOSE_MAKEFILE OFF )

find_program(Cppcheck NAMES cppcheck)
if (Cppcheck)
    list(
        APPEND Cppcheck 
            "--enable=all"
            "--inconclusive"
            "--force"
            "--verbose"
            "--language=c++"
            "--inline-suppr"
            "${CMAKE_SOURCE_DIR}/include/*.h"
            "${CMAKE_SOURCE_DIR}/src/*.cpp"
            "${CMAKE_SOURCE_DIR}/test/*.cpp"
    )
    message("${Cppcheck}")
endif()

find_program(Clangtidy NAMES clang-tidy)
if (Clangtidy)
    list(
        APPEND Clangtidy 
            "-checks='*, -clang-diagnostic-c++98-compat, -readability-implicit-bool-conversion,\
             -google-readability-todo, -fuchsia-overloaded-operator, -readability-named-parameter'"
            "-header-filter='.*'"
    )
endif()

###############################################################################
# Build target
###############################################################################
# Internal library first

# link_libraries( wsock32 ws2_32)

if( LibrarySources )
set( InternalLibrary "${Project}_" ) # cmake will add a "lib" prefix to the name

# link_libraries( wsock32 ws2_32 )
add_compile_options(-fconcepts )

add_library( ${InternalLibrary}
STATIC
${LibrarySources}
)
add_library( Lib::InternalLibrary
ALIAS ${InternalLibrary}
)
target_include_directories( ${InternalLibrary}
PUBLIC ${PROJECT_SOURCE_DIR}/include
)
set_target_properties( ${InternalLibrary}
    PROPERTIES
    CXX_CLANG_TIDY "${Clangtidy}"
    CXX_CPPCHECK "${Cppcheck}"
)
if( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
target_compile_options( ${InternalLibrary} PUBLIC
  -Wall -Wextra -pedantic -Werror
  -fconcepts )
endif()
if( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
target_compile_options( ${InternalLibrary} PRIVATE /W4 /WX )
endif()
target_link_libraries(
  ${InternalLibrary}
  # ${Boost_LIBRARIES}
)
endif()

# Then the executable
add_executable( ${PROJECT_NAME}
${MainSources}
)
target_include_directories( ${PROJECT_NAME}
PUBLIC ${PROJECT_SOURCE_DIR}/include
)
set_target_properties( ${PROJECT_NAME}
    PROPERTIES
    CXX_CLANG_TIDY "${Clangtidy}"
    CXX_CPPCHECK "${Cppcheck}"
)
if( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
target_compile_options( ${PROJECT_NAME} PUBLIC
  -Wall -Wextra -pedantic -Werror
  -fconcepts
  )
endif()
if( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
target_compile_options( ${PROJECT_NAME} PRIVATE /W4 /WX )
endif()


# If using any external libraries include them here after the Internal Library.
# They need to be properly found first. See find_package section
if( LibrarySources )
target_link_libraries( ${PROJECT_NAME}
    Lib::InternalLibrary
    )
endif()
target_link_libraries( ${PROJECT_NAME}
    # Clara::Clara
    # ${Boost_LIBRARIES}
    # wsock32 ws2_32
    )

set( IncludesDir ${PROJECT_SOURCE_DIR}/include )

###############################################################################
# Unit Tests
###############################################################################
add_subdirectory( tests )
