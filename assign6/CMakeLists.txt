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
find_package(Boost 1.50 REQUIRED COMPONENTS system log_setup log)
message(STATUS "Boost version: ${Boost_VERSION}")

include_directories(include)
# Force CMake to include the pthread option during compilation
# https://stackoverflow.com/questions/5395309/how-do-i-force-cmake-to-include-pthread-option-during-compilation
find_package(Threads REQUIRED)

# Updated name and srcs
add_library(server_lib src/server.cc)
add_library(session_lib src/session.cc)
add_library(config_parser_lib src/config_parser.cc)
add_library(echo_handler_lib src/request_handler/echo_handler.cc)
add_library(error_hander_lib src/request_handler/error_handler.cc)
add_library(mime_types_lib src/http/mime_types.cc)
add_library(static_handler_lib src/request_handler/static_handler.cc)
add_library(echo_handler_factory_lib src/request_handler_factory/echo_handler_factory.cc)
add_library(static_handler_factory_lib src/request_handler_factory/static_handler_factory.cc)
add_library(util_lib src/util.cc)
add_library(_404_handler_lib src/request_handler/_404_handler.cc)
add_library(_404_handler_factory_lib src/request_handler_factory/_404_handler_factory.cc)

# Updated executable name, srcs, and deps
add_executable(webserver src/server_main.cc)
add_executable(config_parser_test tests/config_parser_test.cc)
target_link_libraries(webserver server_lib session_lib config_parser_lib
                            echo_handler_factory_lib static_handler_factory_lib _404_handler_factory_lib
                            echo_handler_lib _404_handler_lib
                            error_hander_lib static_handler_lib mime_types_lib util_lib
                            Boost::log_setup Boost::log Boost::system Threads::Threads)
target_link_libraries(config_parser_test config_parser_lib 
                            Boost::log_setup Boost::log gtest_main)

# Add for server_test
add_executable(server_test tests/server_test.cc)
target_link_libraries(server_test server_lib session_lib
                            error_hander_lib 
                            echo_handler_factory_lib static_handler_factory_lib _404_handler_factory_lib 
                            echo_handler_lib static_handler_lib _404_handler_lib
                            mime_types_lib util_lib
                            Boost::log_setup Boost::log gmock_main)

# Add for session_test
add_executable(session_test tests/session_test.cc)
target_link_libraries(session_test session_lib echo_handler_factory_lib echo_handler_lib
                            error_hander_lib _404_handler_factory_lib _404_handler_lib
                            static_handler_lib mime_types_lib util_lib
                            Boost::log_setup Boost::log gtest_main)

# Add for echo_handler_test
add_executable(echo_handler_test tests/echo_handler_test.cc)
target_link_libraries(echo_handler_test echo_handler_lib util_lib gtest_main)

# Add for error_handler_test
add_executable(error_handler_test tests/error_handler_test.cc)
target_link_libraries(error_handler_test error_hander_lib util_lib gtest_main)

# Add for mime_types test
add_executable(mime_types_test tests/mime_types_test.cc)
target_link_libraries(mime_types_test mime_types_lib gtest_main)

# Add for static_handler test
add_executable(static_handler_test tests/static_handler_test.cc)
target_link_libraries(static_handler_test static_handler_lib mime_types_lib util_lib Boost::filesystem gtest_main)

# Add for util test
add_executable(util_test tests/util_test.cc)
target_link_libraries(util_test util_lib Boost::filesystem gtest_main)

# Add for echo_handler_factory test
add_executable(echo_handler_factory_test tests/echo_handler_factory_test.cc)
target_link_libraries(echo_handler_factory_test echo_handler_factory_lib echo_handler_lib util_lib gtest_main)

# Add for static_handler_factory test
add_executable(static_handler_factory_test tests/static_handler_factory_test.cc)
target_link_libraries(static_handler_factory_test static_handler_factory_lib static_handler_lib mime_types_lib util_lib Boost::filesystem gtest_main)

# Add for _404_handler test
add_executable(_404_handler_test tests/_404_handler_test.cc)
target_link_libraries(_404_handler_test _404_handler_lib util_lib gtest_main)

# Add for _404_handler_factory test
add_executable(_404_handler_factory_test tests/_404_handler_factory_test.cc)
target_link_libraries(_404_handler_factory_test _404_handler_factory_lib _404_handler_lib util_lib gtest_main)

# Set testing directory
gtest_discover_tests(config_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(server_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(session_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(echo_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(error_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(mime_types_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(static_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(echo_handler_factory_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(static_handler_factory_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(util_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(_404_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(_404_handler_factory_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Add integration test
add_test(NAME integration_tests COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration_tests/integration_tests.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Generate target/test targets
include(cmake/CodeCoverageReportConfig.cmake)
generate_coverage_report(TARGETS config_parser_lib server_lib session_lib echo_handler_lib error_hander_lib mime_types_lib static_handler_lib echo_handler_factory_lib static_handler_factory_lib util_lib _404_handler_lib _404_handler_factory_lib
                         TESTS config_parser_test server_test session_test echo_handler_test error_handler_test mime_types_test static_handler_test echo_handler_factory_test static_handler_factory_test util_test _404_handler_test _404_handler_factory_test)
