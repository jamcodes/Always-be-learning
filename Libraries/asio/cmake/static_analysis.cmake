include_guard()

function(ConfigureStaticAnalysis)
  AssertOptionsDefined()
  if(${OptCppcheck_})
    find_program(Cppcheck_ cppcheck)
    if(Cppcheck_)
      set(CMAKE_CXX_CPPCHECK ${Cppcheck_} --suppress=missingInclude --enable=all
                            --inline-suppr --inconclusive -i ${CMAKE_SOURCE_DIR}/imgui/lib)
    else()
      message(SEND_ERROR
          "${ColorRed}cppcheck requested but executable not found${ColorReset}")
    endif()
  endif()

  if(${OptClangTidy_})
    find_program(Clangtidy_ clang-tidy)
    if(Clangtidy_)
      set(CMAKE_CXX_CLANG_TIDY ${Clangtidy_} -extra-arg=-Wno-unknown-warning-option)
    else()
      message(SEND_ERROR
          "${ColorRed}clang-tidy requested but executable not found${ColorReset}")
    endif()
  endif()

  if(${OptIwyu_})
    find_program(IncludeWhatYouUse_ include-what-you-use)
    if(IncludeWhatYouUse_)
      set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IncludeWhatYouUse_})
    else()
      message(SEND_ERROR
          "${ColorRed}include-what-you-use requested but executable not found${ColorReset}")
    endif()
  endif()

  if(${OptLwyu_})
      find_program(LinkWhatYouUse_ link-what-you-use)
      if(LinkWhatYouUse_)
          set(CMAKE_CXX_LINK_WHAT_YOU_USE ${LinkWhatYouUse_})
      else()
          message(SEND_ERROR
              "${ColorRed}link-what-you-use requested but executable not found${ColorReset}")
      endif()
  endif()
endfunction()
