include(${CMAKE_CURRENT_LIST_DIR}/colors.cmake)

function(ConfigureCoverage config_interface_target)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(${config_interface_target}
        INTERFACE $<$<BOOL:${${OptCoverage_}}>: --coverage -O0 -ggdb3>
    )
    target_link_libraries(${config_interface_target}
        INTERFACE $<$<BOOL:${${OptCoverage_}}>: --coverage>
    )
  endif()
endfunction()
