find_package(lazperf REQUIRED)
find_package(copc-lib REQUIRED)

include(helpers NO_POLICY_SCOPE)
download_test_files("${CMAKE_CURRENT_BINARY_DIR}/autzen-classified.copc.laz")

add_executable(example_reader "example-reader.cpp")
target_link_libraries(example_reader COPCLIB::copc-lib LAZPERF::lazperf)
add_executable(example_writer "example-writer.cpp")
target_link_libraries(example_writer COPCLIB::copc-lib LAZPERF::lazperf)