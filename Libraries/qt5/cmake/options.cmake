include(${CMAKE_CURRENT_LIST_DIR}/colors.cmake)

macro(SetOptionNames prefix)
    set(OptWerror_             ${prefix}_WERROR CACHE INTERNAL "")
    set(OptIpo_                ${prefix}_IPO CACHE INTERNAL "")
    set(OptTests_              ${prefix}_TESTS CACHE INTERNAL "")
    set(OptDoxygen_            ${prefix}_DOXYGEN CACHE INTERNAL "")
    set(OptCoverage_           ${prefix}_COVERAGE CACHE INTERNAL "")
    set(OptSanitizeAddress_   ${prefix}_SANITIZE_ADDRESS CACHE INTERNAL "")
    set(OptSanitizeMemory_    ${prefix}_SANITIZE_MEMORY CACHE INTERNAL "")
    set(OptSanitizeThread_    ${prefix}_SANITIZE_THREAD CACHE INTERNAL "")
    set(OptSanitizeUb_        ${prefix}_SANITIZE_UB CACHE INTERNAL "")
    set(OptCppcheck_           ${prefix}_CPPCHECK CACHE INTERNAL "")
    set(OptClangTidy_         ${prefix}_CLANG_TIDY CACHE INTERNAL "")
    set(OptIwyu_               ${prefix}_IWYU CACHE INTERNAL "")
    set(OptLwyu_               ${prefix}_LWYU CACHE INTERNAL "")
endmacro()

macro(ShowOptions)
    message(STATUS "${ColorYellow}${OptWerror_}           = ${${OptWerror_}}  ${ColorReset}")
    message(STATUS "${ColorYellow}${OptIpo_}              = ${${OptIpo_}}  ${ColorReset}")
    message(STATUS "${ColorYellow}${OptTests_}            = ${${OptTests_}}  ${ColorReset}")
    message(STATUS "${ColorYellow}${OptDoxygen_}          = ${${OptDoxygen_}}  ${ColorReset}")
    message(STATUS "${ColorYellow}${OptCoverage_}         = ${${OptCoverage_}}  ${ColorReset}")
    message(STATUS "${ColorYellow}${OptSanitizeAddress_} = ${${OptSanitizeAddress_}}  ${ColorReset}")
    message(STATUS "${ColorYellow}${OptSanitizeMemory_}  = ${${OptSanitizeMemory_}}  ${ColorReset}")
    message(STATUS "${ColorYellow}${OptSanitizeThread_}  = ${${OptSanitizeThread_}}  ${ColorReset}")
    message(STATUS "${ColorYellow}${OptSanitizeUb_}      = ${${OptSanitizeUb_}}  ${ColorReset}")
    message(STATUS "${ColorYellow}${OptCppcheck_}         = ${${OptCppcheck_}}  ${ColorReset}")
    message(STATUS "${ColorYellow}${OptClangTidy_}       = ${${OptClangTidy_}}  ${ColorReset}")
    message(STATUS "${ColorYellow}${OptIwyu_}             = ${${OptIwyu_}}  ${ColorReset}")
    message(STATUS "${ColorYellow}${OptLwyu_}             = ${${OptLwyu_}}  ${ColorReset}")
endmacro()

#[[
SetOptions - adds options for controlling the build process.
    Options names are prefixed with the given PREFIX argument.
    Given a PREFIX = "EXAMPLE", the following options are added:

    EXAMPLE_WERROR
    EXAMPLE_TESTS
    EXAMPLE_COVERAGE
    EXAMPLE_SANITIZE_ADDRESS
    EXAMPLE_SANITIZE_LEAK
    EXAMPLE_SANITIZE_THREAD
    EXAMPLE_SANITIZE_UNDEFINED_BEHAVIOR

    All of the options are set to OFF by default.
]]
macro(DefineOptions)
    set(options)
    set(args PREFIX)
    set(list_args)
    cmake_parse_arguments(defineOptions "${options}" "${args}" "${list_args}" ${ARGN})

    foreach(unparsed_arg IN LISTS arg_UNPARSED_ARGUMENTS)
        message(WARNING "${ColorYellow}Unparsed argument: ${unparsed_arg}${ColorReset}")
    endforeach()

    if(NOT defineOptions_PREFIX)
        message(FATAL_ERROR "${ColorRed}DefineOptions: PREFIX argument required${ColorReset}")
    endif()

    if(PROJECT_NAME)
        set(opt_options_defined_ ${PROJECT_NAME}_OptionsDefined_)
    else()
        set(opt_options_defined_ DefaultProject_OptionsDefined_)
    endif()
    set(${opt_options_defined_} TRUE CACHE INTERNAL "")

    SetOptionNames(${defineOptions_PREFIX})

    option(${OptWerror_} "Enable warning as errors for ${PROJECT_NAME}" OFF)
    option(${OptIpo_} "Enable Interprocedural Optimization, aka Link Time Optimization (LTO)" OFF)
    option(${OptTests_} "Build ${PROJECT_NAME} tests" OFF)
    option(${OptDoxygen_} "Build ${PROJECT_NAME} documentation" OFF)
    option(${OptCoverage_} "Generate ${PROJECT_NAME} code coverage" OFF)
    option(${OptSanitizeAddress_}  "Enable address-sanitizer for ${PROJECT_NAME}" OFF)
    option(${OptSanitizeMemory_} "Enable memory-sanitizer for ${PROJECT_NAME}" OFF)
    option(${OptSanitizeThread_} "Enable thread-sanitizer for ${PROJECT_NAME}" OFF)
    option(${OptSanitizeUb_} "Enable undefined-behavior-sanitizer for ${PROJECT_NAME}" OFF)
    option(${OptCppcheck_} "Enable static analysis with cppcheck" OFF)
    option(${OptClangTidy_} "Enable static analysis with clang-tidy" OFF)
    option(${OptIwyu_} "Enable static analysis with include-what-you-use" OFF)
    option(${OptLwyu_} "Enable static analysis with link-what-you-use" OFF)

    if (NOT CMAKE_BUILD_TYPE STREQUAL Debug)
        if (${${OptCoverage_}})
            colorMessage(FATAL_ERROR
                "${ColorBoldRed}${${OptCoverage_}}=ON valid only for CMAKE_BUILD_TYPE=Debug${ColorReset}")
        endif()
        if (${OptSanitizeAddress_} OR ${OptSanitizeMemory_} OR ${OptSanitizeThread_} OR ${OptSanitizeUb_})
            colorMessage(FATAL_ERROR
                "${ColorBoldRed}Sanitizers can be used only with CMAKE_BUILD_TYPE=Debug${ColorReset}")
        endif()
    endif()
endmacro()

function(AssertOptionsDefined)
if (NOT $CACHE{opt_options_defined_})
    message(FATAL_ERROR "
${ColorGreen}Configure project option names by including
`options.cmake` and calling
    DefineOptions(PREFIX myPrefix)
Before including project_settings.cmake
${ColorReset}")
    endif()
endfunction()
