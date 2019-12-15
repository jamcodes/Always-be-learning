function(GetProjectPrefix out)
    string(TOUPPER ${CMAKE_PROJECT_NAME} prefix)
    set(${out} ${prefix} PARENT_SCOPE)
endfunction()
