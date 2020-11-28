include(${CMAKE_CURRENT_LIST_DIR}/colors.cmake)

function(ConfigureBuildType)
    set(arg_options)
    set(arg_kwargs DEFAULT)
    set(arg_list_args)
    cmake_parse_arguments(arg "${arg_options}" "${arg_kwargs}" "${arg_list_args}" ${ARGN})
    foreach(unparsed_arg IN LISTS arg_UNPARSED_ARGUMENTS)
        message(WARNING "${ColorYellow}Unparsed argument: ${unparsed_arg}${ColorReset}")
    endforeach()
    if(NOT arg_DEFAULT)
        message(FATAL_ERROR "${ColorRed}arg: DEFAULT argument required")
    endif()

    get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if(isMultiConfig)
        message(STATUS
    "${ColorGreen}Multiconfig generator. Configurations: ${CMAKE_CONFIGURATION_TYPES}${ColorReset}"
        )
    else()
        set(allowedBuildTypes Debug RelWithDebInfo MinSizeRel Release)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${allowedBuildTypes}")
        if(NOT CMAKE_BUILD_TYPE)
            message(STATUS
        "${ColorYellow}Setting build type to '${arg_DEFAULT}' as none was specified${ColorReset}"
            )
            set(CMAKE_BUILD_TYPE ${arg_DEFAULT} CACHE STRING "Choose the type of build." FORCE)
        elseif(NOT CMAKE_BUILD_TYPE IN_LIST allowedBuildTypes)
            message(FATAL_ERROR "${ColorRed}Invalid build type: ${CMAKE_BUILD_TYPE}${ColorReset}")
        endif()
    endif()
endfunction()
