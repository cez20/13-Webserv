cmake_minimum_required(VERSION 3.12)
project(webserv)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_SOURCES
    #tests/compilationTests.cpp 
    tests/googletests/launchServerTests.cpp
	src/ConfigFile.cpp
    src/HttpRequest.cpp 
    src/HttpResponse.cpp 
    src/launchServer.cpp 
    #src/main.cpp         Not necessary because Test files already posses its own main 
    src/monitorServer.cpp 
    src/utils.cpp
	src/log.cpp
)

# # creates an executable called webservTests taking the TEST souces 
add_executable(webservTests ${TEST_SOURCES})

# Add any required libraries or include directories for tests
target_include_directories(webservTests PRIVATE inc)

# Set the output directory for the test executable
set_target_properties(webservTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..
)

# Link the test executable with the library target
target_link_libraries(webservTests PRIVATE GTest::gtest_main)

# Discover and run tests using GoogleTest
include(GoogleTest)
gtest_discover_tests(webservTests)
