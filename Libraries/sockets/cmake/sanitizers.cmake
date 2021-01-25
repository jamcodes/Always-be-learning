include(${CMAKE_CURRENT_LIST_DIR}/colors.cmake)

function(ConfigureSanitizers config_interface_target)
AssertOptionsDefined()
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(Sanitizers_ "")

    if(${OptSanitizeAddress_})
      list(APPEND Sanitizers_ "address")
    endif()

    if(${OptSanitizeMemory_})
      list(APPEND Sanitizers_ "memory")
    endif()

    if(${OptSanitizeThread_})
      list(APPEND Sanitizers_ "undefined")
    endif()

    if(${OptSanitizeUb_})
      list(APPEND Sanitizers_ "thread")
    endif()

    list(JOIN Sanitizers_ "," ListOfSanitizers_)
endif()

  message(STATUS "${ColorGreen}Sanitizers = ${ListOfSanitizers_}${ColorReset}")

  if(ListOfSanitizers_)
    if(NOT "${ListOfSanitizers_}" STREQUAL "")
      target_compile_options(${config_interface_target}
                             INTERFACE -fno-omit-frame-pointer -fsanitize=${ListOfSanitizers_})
      target_link_libraries(${config_interface_target}
                            INTERFACE -fsanitize=${ListOfSanitizers_})
    endif()
  endif()

endfunction()
