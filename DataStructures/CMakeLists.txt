cmake_minimum_required(VERSION 3.15)
project(DataStructures)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(options)
DefineOptions(PREFIX DataStructures)
include(project_settings)
include(output_directories)

set(DataStructures_WERROR ON)
set(DataStructures_TESTS ON)
ConfigureBuildType(DEFAULT Debug)
ConfigureOutputDirectories(${DataStructures_SOURCE_DIR})
ConfigureGlobalFlags()
DeclareCompilerConfigTarget(NAMESPACE DataStructures TARGET CompilerConfig)

ShowOptions()

set(validAssertLevels OFF ASSUME IGNORED ENFORCE AUDIT)
if (NOT JAM_CONTRACT_LEVEL IN_LIST validAssertLevels)
    message(STATUS "JAM_CONTRACT_LEVEL unspecified - set to ENFORCE; specify one of ${validAssertLevels}")
    set(JAM_CONTRACT_LEVEL "ENFORCE" CACHE STRING "Specify contract enforcement level")
endif()
if (NOT JAM_CONTRACT_LEVEL IN_LIST validAssertLevels)
    message(FATAL_ERROR "Invalid JAM_CONTRACT_LEVEL specified valid values = ${validAssertLevels}")
endif()
target_compile_definitions(DataStructures_CompilerConfig
    INTERFACE
        # Hardcode enabled asserts for now, in the future - configure based on the build type
        "JAM_CONTRACT_LEVEL=${JAM_CONTRACT_LEVEL}"
        "JAM_ASSERT_USE_DEBUGTRAP_HANDLER=1"
        $<IF:$<CONFIG:Debug>,DEBUG,NDEBUG>
)

add_subdirectory(depends)
add_subdirectory(utils)

add_subdirectory(SingleLinkedList)
add_subdirectory(BinaryTree)
add_subdirectory(HashMap)
