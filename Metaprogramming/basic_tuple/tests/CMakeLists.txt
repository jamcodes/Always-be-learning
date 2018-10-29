cmake_minimum_required( VERSION 3.1 )

set( TestProject "test_${Project}" )
project( ${TestProject} )


###############################################################################
# Prepare test sources

# These are all sources EXCLUDING the main.cpp which contains main()
# function or the appropriate Catch2 define.
# Naming convention is assumed - test_someFeatureUnderTest.cpp
file( GLOB TestSources
      "${PROJECT_SOURCE_DIR}/test_*.cpp"
    )


###############################################################################
# Build executable

add_executable( ${PROJECT_NAME}
    main.cpp
    )
target_sources( ${PROJECT_NAME}
    PUBLIC ${TestSources}
    )
# target_compile_options( ${PROJECT_NAME}
#     PRIVATE
#     # -fconcepts
#     # -lstdc++fs
#     )
target_link_libraries( ${PROJECT_NAME}
    Project_config
    Lib::${Project}
    GTest::GTest
    GTest::Main
    )

###############################################################################
# CTest

enable_testing()
# gtest_discover_tests(${PROJECT_NAME})
add_test(${PROJECT_NAME} ${PROJECT_NAME})
