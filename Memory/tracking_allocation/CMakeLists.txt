cmake_minimum_required(VERSION 3.15)

add_executable(track_alloc
    new_tracker.h
    leaking_app.cpp
)

add_executable(track_alloc_src_loc
    new_tracker_src_loc.h
    leaking_app_src_loc.cpp
)

add_executable(track_alloc_src_loc_my_alloc
    new_tracker_src_loc_myalloc.h
    leaking_app_src_loc_myalloc.cpp
)
