if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(
        /W4
        /permissive-
    )
else()
    add_compile_options(
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
        -Wsign-conversion
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

        $<$<CONFIG:Debug>:-Og>
        $<$<CONFIG:Debug>:-ggdb3>
    )
    # add_link_options(-fuse-ld=gold)
endif()
