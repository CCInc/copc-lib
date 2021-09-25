cmake_minimum_required(VERSION 3.9)

# options to configure the library with.
option(WITH_TESTS "Build test files." ON)
option(WITH_EXAMPLES "Build example files." ON)
option(WITH_PYTHON "Build python bindings." ON)

# Path to additional CMake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# If no build type was specified, set it to Release.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
            "Choose the type of build, options are: None Debug Release."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# If no installation prefix is given manually, install locally.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE STRING
            "The install location"
            FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

add_subdirectory(cpp)

# Build Submodules
if (WITH_TESTS OR WITH_EXAMPLES OR WITH_PYTHON)
    include(helpers NO_POLICY_SCOPE)

    download_test_files("${CMAKE_BINARY_DIR}/autzen-classified.copc.laz")
    set(BaseAutzenFilePath "${CMAKE_BINARY_DIR}/autzen-classified.copc.laz")
endif()

if(WITH_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(WITH_EXAMPLES OR WITH_TESTS)
    add_subdirectory(example)
    if(WITH_TESTS)
        add_test(NAME example_reader COMMAND example_reader)
        add_test(NAME example_writer COMMAND example_writer)
    endif()
endif()

if(WITH_PYTHON)
    add_subdirectory(python)
endif()