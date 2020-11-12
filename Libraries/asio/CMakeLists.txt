cmake_minimum_required(VERSION 3.15)
project(LearningASIO VERSION 0.1.0)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(options)
DefineOptions(PREFIX LearningASIO)
include(project_settings)
include(output_directories)
include(release)

set(LearningASIO_WERROR ON)
set(LearningASIO_TESTS ON)
ConfigureBuildType(DEFAULT Debug)
ConfigureOutputDirectories(${LearningASIO_SOURCE_DIR})
ConfigureGlobalFlags()
DeclareCompilerConfigTarget(NAMESPACE LearningASIO TARGET CompilerConfig)
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
target_compile_definitions(LearningASIO_CompilerConfig
    INTERFACE
        # Hardcode enabled asserts for now, in the future - configure based on the build type
        "JAM_CONTRACT_LEVEL=${JAM_CONTRACT_LEVEL}"
        "JAM_ASSERT_USE_DEBUGTRAP_HANDLER=1"
        $<IF:$<CONFIG:Debug>,DEBUG,NDEBUG>
)

add_subdirectory(deps)
add_subdirectory(utils)
add_subdirectory(olc)

InstallProject(TARGETS LearningASIO_asio)
