cmake_minimum_required( VERSION 3.1 )

###############################################################################
# It is assumed that the the Catch2 library header is made available
# in the form of a cmake INTERFACE library with ALIAS Catch::Test
###############################################################################

set( TestProject "test_${Project}" )
project( ${TestProject} )


###############################################################################
# Prepare test sources

# These are all sources EXCLUDING the tests_main.cpp which contains main()
# function or the appropriate Catch2 define.
# Naming convention is assumed - test_someFeatureUnderTest.cpp
file( GLOB TestSources
      RELATIVE "${PROJECT_SOURCE_DIR}"
      "${PROJECT_SOURCE_DIR}/test_*.cpp"
    )


###############################################################################
# Build executable

# set( TestExecutable "test_${Executable}" )
# test_main.cpp assumed to contain the main() function or the appropriate
# Catch2 define.
add_executable( ${PROJECT_NAME}
    main.cpp
    )
target_sources( ${PROJECT_NAME}
    PUBLIC ${TestSources}
    )
target_compile_options( ${PROJECT_NAME}
    PRIVATE
    -fconcepts
    # -lstdc++fs
    )
if( LibrarySources )
target_link_libraries( ${PROJECT_NAME}
    Lib::InternalLibrary
    )
endif()
target_link_libraries( ${PROJECT_NAME}
    Project_config
    GTest::GTest
    GTest::Main
    )


###############################################################################
# CTest
enable_testing()
# gtest_discover_tests(${PROJECT_NAME})
add_test(${PROJECT_NAME} ${PROJECT_NAME})
