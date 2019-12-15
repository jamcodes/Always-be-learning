include(${CMAKE_CURRENT_LIST_DIR}/colors.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/prefix.cmake)

function(CoverageTargets)
    GetProjectPrefix(prefix)
    if(${opt_coverage})
        set(coverage_dir ${CMAKE_BINARY_DIR}/coverage)
        file(MAKE_DIRECTORY ${coverage_dir})

        find_program(gcov NAMES gcov gcov-9 gcov-8)
        if (NOT gcov)
            message(FATAL_ERROR "${ColorRed}Failed to find gcov${ColorReset}")
        endif()
        find_program(GCOVR gcovr)
        if (NOT GCOVR)
            message(FATAL_ERROR "${ColorRed}Failed to find gcovr${ColorReset}")
        endif()

        set(coverage_command
            ${GCOVR}
                --gcov-executable ${gcov}
                --root ${CMAKE_SOURCE_DIR}
                --exclude-directories "build.*"
                --print-summary
                --exclude-unreachable-branches
                --object-directory ${CMAKE_BINARY_DIR}
        )
        set(coverage_command_html
            ${coverage_command}
            --html
            --html-details
            --output ${coverage_dir}/index.html
        )
        set(coverage_command_xml
            ${coverage_command}
                --xml-pretty
                --output ${coverage_dir}/coverage.xml
        )
        add_custom_target(coverage
            COMMAND ${coverage_command_html}
            USES_TERMINAL
        )
        add_custom_target(coverage-xml
            COMMAND ${coverage_command_xml}
            USES_TERMINAL
        )
        CoverageClearCommand(coverage_command_clear)
        add_custom_target(coverage-clear
            COMMAND ${coverage_command_clear}
            USES_TERMINAL
        )
    endif()
endfunction()

function(CoverageClearCommand out)
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        message(STATUS "${ColorYellow}MSVC toolchain detected: coverage-clear target is no-op${ColorReset}")
        set(${out} "")
    else()
        set(${out}
            find ${CMAKE_BINARY_DIR}/ -name "*.gcda" -delete
            PARENT_SCOPE
        )
    endif()
endfunction()
