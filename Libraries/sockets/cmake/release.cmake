include(GNUInstallDirs)
#[[ ConfigureProjectInstallDirectories
    Declares project-specific global variables used as a point of reference when installing
    custom components.
    This macro needs to be called after declaring the project for which target will be installed,
    and before the call to InstallProject() for this particular project.
]]
macro(ConfigureProjectInstallDirectories)
    set(${PROJECT_NAME}_install_include_dir ${CMAKE_INSTALL_INCLUDEDIR})
    set(${PROJECT_NAME}_install_library_dir ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})
    set(${PROJECT_NAME}_install_runtime_dir ${CMAKE_INSTALL_BINDIR})
    set(${PROJECT_NAME}_export_targets      ${PROJECT_NAME}-targets)
endmacro()

#[[ InstallProject
    Installs targets for the given project and all necessary scripting to support CMake find_package
    along with versioning. Relies on previous call to ConfigureProjectInstallDirectories()
]]
function(InstallProject)
    set(options)
    set(args)
    set(list_args TARGETS)
    cmake_parse_arguments(arg "${options}" "${args}" "${list_args}" ${ARGN})
    foreach(unparsed_arg IN LISTS arg_UNPARSED_ARGUMENTS)
        message(WARNING "${ColorYellow}Unparsed argument: ${unparsed_arg}${ColorReset}")
    endforeach()
    if(NOT arg_TARGETS)
        message(FATAL_ERROR "${ColorRed}InstallProject: at lest one TARGETS argument required${ColorReset}")
    endif()
    set(project_config_install_dir ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
    set(project_config_file ${PROJECT_NAME}Config.cmake)
    set(project_config_template_file ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in)
    set(project_config_version_file ${PROJECT_NAME}ConfigVersion.cmake)
    set(project_config_targets_file ${PROJECT_NAME}Targets.cmake)
    set(project_runtime_component ${PROJECT_NAME}_Runtime)
    set(project_development_component ${PROJECT_NAME}_Development)

    install(
        TARGETS ${arg_TARGETS}
        EXPORT ${${PROJECT_NAME}_export_targets}
        RUNTIME DESTINATION ${${PROJECT_NAME}_install_runtime_dir}
            COMPONENT           ${project_runtime_component}
        LIBRARY DESTINATION ${${PROJECT_NAME}_library_dir}
            COMPONENT           ${project_runtime_component}
            NAMELINK_COMPONENT  ${project_development_component}
        ARCHIVE DESTINATION ${${PROJECT_NAME}_library_dir}
            COMPONENT           ${project_development_component}
    )

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${project_config_version_file}
        VERSION ${CMAKE_PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    configure_package_config_file(
        ${project_config_template_file}
        ${CMAKE_CURRENT_BINARY_DIR}/${project_config_file}
        INSTALL_DESTINATION ${project_config_install_dir}
    )

    install(
        EXPORT ${${PROJECT_NAME}_export_targets}
        FILE ${project_config_targets_file}
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${project_config_install_dir}
    )

    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/${project_config_file}
            ${CMAKE_CURRENT_BINARY_DIR}/${project_config_version_file}
        DESTINATION ${project_config_install_dir}
    )
    export(
        EXPORT ${${PROJECT_NAME}_export_targets}
        FILE ${CMAKE_CURRENT_BINARY_DIR}/${project_config_targets_file}
        NAMESPACE ${PROJECT_NAME}::
    )
    export(PACKAGE ${PROJECT_NAME})
endfunction()
