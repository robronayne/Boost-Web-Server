cmake_minimum_required(VERSION 3.10.0)
project(ngineers)

# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.
    Please make a new directory (called a build directory) and run CMake from there.
    You may need to remove CMakeCache.txt." )
endif()

# Turn on debug builds if we are building in a devel shell
if (CMAKE_BUILD_TYPE STREQUAL "" AND "$ENV{DEVEL_SHELL}" STREQUAL "1")
    message(STATUS "Setting debug build type by default in devel shell")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Output binaries to a sub directory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable cmake testing
include(CTest)
enable_testing()

# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)

# Enable Boost
# Use static libraries so binaries can be deployed without a full boost install
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 REQUIRED COMPONENTS system)
message(STATUS "Boost version: ${Boost_VERSION}")

include_directories(include)
# Force CMake to include the pthread option during compilation
# https://stackoverflow.com/questions/5395309/how-do-i-force-cmake-to-include-pthread-option-during-compilation
find_package(Threads REQUIRED)

# Updated name and srcs
add_library(server_lib src/server.cc)
add_library(session_lib src/session.cc)
add_library(config_parser_lib src/config_parser.cc)

# -- add for reply test
add_library(reply_lib src/reply.cc)


# Updated executable name, srcs, and deps
add_executable(webserver src/server_main.cc)
add_executable(config_parser_test tests/config_parser_test.cc)
target_link_libraries(webserver server_lib session_lib config_parser_lib reply_lib Boost::system Threads::Threads)
target_link_libraries(config_parser_test config_parser_lib gtest_main)

# Updated reply test name, srcs, and deps
add_executable(reply_test tests/reply_test.cc)
target_link_libraries(reply_test reply_lib gtest_main gmock_main)

# TODO(!): Update with test binary
gtest_discover_tests(config_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
# gtest_discover_tests(example_lib_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(reply_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)



# TODO(!): Update with target/test targets
include(cmake/CodeCoverageReportConfig.cmake)
# generate_coverage_report(TARGETS example_server example_lib TESTS example_lib_test)
generate_coverage_report(TARGETS config_parser_lib TESTS config_parser_test)
# include(cmake/CodeCoverageReportConfig.cmake)
generate_coverage_report(TARGETS reply_lib TESTS reply_test)
