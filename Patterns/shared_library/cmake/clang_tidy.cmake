include(${CMAKE_CURRENT_LIST_DIR}/colors.cmake)

function(ClangTidyTarget)
    set(options)
    set(args)
    set(list_args PATHS PATH_SUFFIXES)
    cmake_parse_arguments(arg "${options}" "${args}" "${list_args}" ${ARGN})
    foreach(unparsed_arg IN LISTS arg_UNPARSED_ARGUMENTS)
        message(WARNING "${ColorYellow}Unparsed argument: ${unparsed_arg}${ColorReset}")
    endforeach()

    find_program(run_clang_tidy
        NAMES
            run-clang-tidy.py
            run-clang-tidy-10.py
            run-clang-tidy-9.py
            run-clang-tidy-8.py
            run-clang-tidy-7.py
            run-clang-tidy-6.py
        PATHS ${arg_PATHS}
        PATH_SUFFIXES ${arg_PATH_SUFFIXES}
        )
    if (NOT run_clang_tidy)
        message(WARNING
            "${ColorYellow}run-clang-tidy.py not found; clang-tidy target unavailable${ColorReset}"
        )
    endif()
    set(run_clang_tidy_command
        ${run_clang_tidy}
            -j 4
            -extra-arg=-std=c++${CMAKE_CXX_STANDARD}
            # \"-header-filter=${CMAKE_SOURCE_DIR}/\(include|src|tests|examples|benchmarks\).*\"
            -p ${CMAKE_BINARY_DIR}
        )
    add_custom_target(
        clang-tidy
            COMMAND ${run_clang_tidy_command}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        USES_TERMINAL
        )
    add_custom_target(
        clang-tidy-fix
            COMMAND ${run_clang_tidy_command} -fix
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        VERBATIM
        USES_TERMINAL
        )
endfunction()
