cmake_minimum_required(VERSION 3.9)
project(copc-lib VERSION 1.2.0)

# global constants
set (COPC_NAMESPACE COPCLIB)
set (COPC_SHARED_LIB copc-lib)
set (COPC_STATIC_LIB copc-lib-s)

# Here we configure variables specifying the target directories.
set(INSTALL_RUNTIME_DIR ${CMAKE_INSTALL_BINDIR})
set(INSTALL_CONFIG_DIR  ${CMAKE_INSTALL_LIBDIR}/${COPC_NAMESPACE}/cmake)
set(INSTALL_LIBRARY_DIR ${CMAKE_INSTALL_LIBDIR}/${COPC_NAMESPACE}/${CMAKE_BUILD_TYPE})
set(INSTALL_ARCHIVE_DIR ${CMAKE_INSTALL_LIBDIR}/${COPC_NAMESPACE}/${CMAKE_BUILD_TYPE}/static)
set(INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/${COPC_NAMESPACE}/${COPC_SHARED_LIB})

set(PROJECT_CONFIG_FILE "${PROJECT_BINARY_DIR}/${COPC_SHARED_LIB}-config.cmake")

# Compilation options
set(WITH_TESTS TRUE CACHE BOOL "Choose if unit tests should be built")
set(WITH_EXAMPLES TRUE CACHE BOOL "Choose if examples should be built")
set(WITH_PYTHON TRUE CACHE BOOL "Choose if python bindings should be built")

# compile settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# If no build type was specified, set it to Release.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
            "Choose the type of build, options are: None Debug Release."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# includes
find_package(lazperf 2.1.0 REQUIRED)
include(CMakePackageConfigHelpers)
include_directories(include)

# Only public header files go here.
set(HEADER_FILES
        include/copc/file.hpp
        include/hierarchy/entry.hpp
        include/hierarchy/key.hpp
        include/hierarchy/node.hpp
        include/hierarchy/page.hpp
        include/io/base_io.hpp
        include/io/reader.hpp
        include/io/writer.hpp
        include/las/file.hpp
        include/las/header.hpp
        include/las/point.hpp
        include/las/points.hpp
        include/las/utils.hpp
        include/las/vlr.hpp
        include/laz/compressor.hpp
        include/laz/decompressor.hpp
)

# All source files and private header files go here.
set(SOURCE_FILES
        include/hierarchy/internal/page.hpp
        include/hierarchy/internal/hierarchy.hpp
        include/io/internal/writer_internal.hpp
        src/hierarchy/key.cpp
        src/hierarchy/page.cpp
        src/io/base_io.cpp
        src/io/reader.cpp
        src/io/writer_internal.cpp
        src/io/writer_public.cpp
        src/las/header.cpp
        src/las/point.cpp
        src/las/points.cpp
        src/las/utils.cpp
)

# libraries
add_library(${COPC_SHARED_LIB} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${COPC_SHARED_LIB} PUBLIC LAZPERF::lazperf)
target_include_directories(${COPC_SHARED_LIB} INTERFACE $<INSTALL_INTERFACE:include>)

add_library(${COPC_STATIC_LIB} STATIC ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${COPC_STATIC_LIB} PUBLIC LAZPERF::lazperf)

add_library(${COPC_NAMESPACE}::${COPC_SHARED_LIB} ALIAS ${COPC_SHARED_LIB})

# create package config
configure_package_config_file(cmake/${COPC_SHARED_LIB}-config.cmake.in
        ${PROJECT_CONFIG_FILE}
        INSTALL_DESTINATION ${INSTALL_CONFIG_DIR})


# The INSTALL(TARGET) command specifies where to install specific targets.
# By specifying EXPORT here, we add these targets to an export group.
install(TARGETS ${COPC_SHARED_LIB}
        EXPORT ${COPC_SHARED_LIB}-targets
        RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
        ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR})

# The INSTALL(FILES) command specifies where to install specific files, such as headers.
install(FILES
        ${PROJECT_CONFIG_FILE}
        NAMESPACE ${COPC_NAMESPACE}::
        DESTINATION ${INSTALL_CONFIG_DIR})

# install header files, keeping directory structure
foreach(file ${HEADER_FILES})
    get_filename_component(FILE_DIR ${file} DIRECTORY)
    install(FILES ${file} DESTINATION "include/${dir}")
endforeach()

# The INSTALL(EXPORT) command installs a previously defined export group to a script.
install(EXPORT ${COPC_SHARED_LIB}-targets
        FILE ${COPC_SHARED_LIB}-targets.cmake
        NAMESPACE ${COPC_NAMESPACE}::
        DESTINATION ${INSTALL_CONFIG_DIR})

# Register the package in the user's package registry.
# https://cmake.org/cmake/help/v3.11/command/export.html
export(PACKAGE ${COPC_SHARED_LIB})