cmake_minimum_required(VERSION 3.15)
project(LearningQt5 VERSION 0.1.0)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(options)
DefineOptions(PREFIX LearningQt5)
include(project_settings)
include(output_directories)
include(release)

set(LearningQt5_WERROR ON)
set(LearningQt5_TESTS ON)
ConfigureBuildType(DEFAULT Debug)
ConfigureOutputDirectories(${LearningQt5_BINARY_DIR})
ConfigureGlobalFlags()
DeclareCompilerConfigTarget(NAMESPACE LearningQt5 TARGET CompilerConfig)
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
target_compile_definitions(LearningQt5_CompilerConfig
    INTERFACE
        # Hardcode enabled asserts for now, in the future - configure based on the build type
        "JAM_CONTRACT_LEVEL=${JAM_CONTRACT_LEVEL}"
        "JAM_ASSERT_USE_DEBUGTRAP_HANDLER=1"
        $<IF:$<CONFIG:Debug>,DEBUG,NDEBUG>
)

add_subdirectory(deps)
add_subdirectory(utils)

find_package(Qt5 5.15 REQUIRED
    COMPONENTS
        Core
        Widgets
)

add_subdirectory(zetcode)


# InstallProject(TARGETS LearningQt5)
