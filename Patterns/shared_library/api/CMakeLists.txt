add_library(${CMAKE_PROJECT_NAME}_Api INTERFACE)
add_library(${CMAKE_PROJECT_NAME}::Api ALIAS ${CMAKE_PROJECT_NAME}_Api)
target_include_directories(${CMAKE_PROJECT_NAME}_Api
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${project_install_include_dir}>
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/foobar
        DESTINATION ${project_install_include_dir}
)
install(
    TARGETS ${CMAKE_PROJECT_NAME}_Api
    EXPORT ${project_export_targets}
)
