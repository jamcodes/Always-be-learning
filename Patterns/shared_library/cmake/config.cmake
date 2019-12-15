include(${CMAKE_CURRENT_LIST_DIR}/colors.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/prefix.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/options.cmake)

macro(SetDefaultBuildType)
    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        message(STATUS "${ColorYellow}Setting build type to 'Debug' as none was specified.${ColorReset}")
        set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    endif()
endmacro()

function(DeclareCompilerConfigTarget target)
    SetOptionNames()
    add_library(${target} INTERFACE)
    target_compile_features(${target}
        INTERFACE
            cxx_std_17
    )
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        target_compile_options(${target} INTERFACE
            /W4
            /permissive-
            # $<$<BOOL:${opt_werror}>:/WX>
        )
    else()
        target_compile_options(${target} INTERFACE
        -Wall
        -Wdouble-promotion
        -Weffc++
        -Wextra
        -Wformat=2
        -Wnon-virtual-dtor
        -Wnull-dereference
        -Wold-style-cast
        -Woverloaded-virtual
        -Wpedantic
        -Wshadow
        # -Wsign-conversion
        -Wunused
        -Wcast-align
        -pedantic-errors

        -Wimplicit-fallthrough
        -Wsuggest-attribute=pure
        -Wsuggest-attribute=const
        -Wsuggest-attribute=cold
        -Wsuggest-final-types
        -Wsuggest-final-methods
        -Wduplicated-branches
        -Wduplicated-cond

        $<$<BOOL:${opt_werror}>:-Werror>
        $<$<CONFIG:Debug>:-Og>
        $<$<CONFIG:Debug>:-ggdb3>
        # -fuse-ld=gold
        )
        target_link_options(${target} INTERFACE
            # -fuse-ld=gold
            $<$<BOOL:${${opt_coverage}}>:--coverage>
            $<$<BOOL:${${opt_coverage}}>:-ftest-coverage>
            $<$<BOOL:${${opt_coverage}}>:-fprofile-arcs>
            $<$<BOOL:${${opt_sanitize_address}}>:-fno-omit-frame-pointer>
            $<$<BOOL:${${opt_sanitize_address}}>:-fsanitize=address>
            $<$<BOOL:${${opt_sanitize_leak}}>:-fsanitize=leak>
            $<$<BOOL:${${opt_sanitize_thread}}>:-fsanitize=thread>
            $<$<BOOL:${${opt_sanitize_ub}}>:-fsanitize=undefined>
        )
    endif()
endfunction()
