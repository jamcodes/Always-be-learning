cmake_minimum_required(VERSION 3.15)
project(LearningSockets VERSION 0.1.0)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(options)
DefineOptions(PREFIX LearningSockets)
include(project_settings)
include(output_directories)
include(release)

set(LearningSockets_WERROR ON)
set(LearningSockets_TESTS ON)
ConfigureBuildType(DEFAULT Debug)
ConfigureOutputDirectories(${LearningSockets_BINARY_DIR})
ConfigureGlobalFlags()
DeclareCompilerConfigTarget(NAMESPACE LearningSockets TARGET CompilerConfig)
ConfigureProjectInstallDirectories()

ShowOptions()

set(validAssertLevels OFF ASSUME IGNORED ENFORCE AUDIT)
if (NOT JAM_CONTRACT_LEVEL IN_LIST validAssertLevels)
    message(STATUS "JAM_CONTRACT_LEVEL unspecified - set to ENFORCE; specify one of ${validAssertLevels}")
    set(JAM_CONTRACT_LEVEL "ENFORCE" CACHE STRING "Specify contract enforcement level")
endif()
if (NOT JAM_CONTRACT_LEVEL IN_LIST validAssertLevels)
    message(FATAL_ERROR "Invalid JAM_CONTRACT_LEVEL specified valid values = ${validAssertLevels}")
endif()
target_compile_definitions(LearningSockets_CompilerConfig
    INTERFACE
        # Hardcode enabled asserts for now, in the future - configure based on the build type
        "JAM_CONTRACT_LEVEL=${JAM_CONTRACT_LEVEL}"
        "JAM_ASSERT_USE_DEBUGTRAP_HANDLER=1"
        $<IF:$<CONFIG:Debug>,DEBUG,NDEBUG>
)

find_package(Threads REQUIRED)
target_link_libraries(LearningSockets_CompilerConfig
    INTERFACE
        Threads::Threads
)

add_subdirectory(deps)
add_subdirectory(utils)
add_subdirectory(low_level)
add_subdirectory(codility)

# InstallProject(TARGETS LearningSockets_Sockets)
