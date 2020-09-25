cmake_minimum_required(VERSION 3.15)

include(FetchContent)

# Build all dependencies in release mode ?
# set(CMAKE_BUILD_TYPE "Release")

FetchContent_Declare(fmtlib
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 6.2.1
)
FetchContent_Declare(gsl
    GIT_REPOSITORY https://github.com/microsoft/GSL.git
    GIT_TAG master
)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0
)
message(STATUS "${ColorGreen}Fetching gsl${ColorReset}")
FetchContent_MakeAvailable(gsl)
message(STATUS "${ColorGreen}Fetching FMT${ColorReset}")
FetchContent_MakeAvailable(fmtlib)
message(STATUS "${ColorGreen}Fetching googletest${ColorReset}")
FetchContent_MakeAvailable(googletest)

# After the content has been populated initially - speed up the configure stage by disabling
# updates
# set(FETCHCONTENT_FULLY_DISCONNECTED ON CACHE BOOL "Disable FetchContent download stage")
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "Disable FetchContent updates" FORCE)
