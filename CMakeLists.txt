cmake_minimum_required(VERSION 3.12)
project(althas
        VERSION 1.0
        DESCRIPTION "Header-only C++ library provides auto-management & table-driven testing facilities for Verilator simulaus."
        HOMEPAGE_URL "https://github.com/Ray-Eldath/althas")

set(CMAKE_CXX_STANDARD 17)

#
# althas
#
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

#
# example
#
option(${PROJECT_NAME}_BUILD_EXAMPLE "Build example for ${PROJECT_NAME}." OFF)

if (${PROJECT_NAME}_BUILD_EXAMPLE)
    add_subdirectory(example/pc)
    add_subdirectory(example/termcolor)
endif ()

#
# install & export
#
include(CMakePackageConfigHelpers)

configure_package_config_file(
        cmake/config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/generated/${PROJECT_NAME}-config.cmake
        INSTALL_DESTINATION lib/cmake/${PROJECT_NAME})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/generated/${PROJECT_NAME}-config.cmake
        DESTINATION lib/cmake/${PROJECT_NAME})

install(DIRECTORY include/
        DESTINATION include)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-targets
        INCLUDES DESTINATION include)

install(EXPORT ${PROJECT_NAME}-targets
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION lib/cmake/${PROJECT_NAME})