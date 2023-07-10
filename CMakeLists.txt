# cmake_minimum_required(VERSION 3.12)
# project(webserv)

# set(CMAKE_CXX_STANDARD 98)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# enable_testing()

# # Set your main program source files
# set(SOURCES
#     src/ConfigFile.cpp
#     src/HttpRequest.cpp 
#     src/HttpResponse.cpp 
#     src/launchServer.cpp 
#     src/main.cpp 
#     src/monitorServer.cpp 
#     src/utils.cpp
# )

# # Set your test source files
# set(TEST_SOURCES
#     tests/compilationTests.cpp 
#     tests/webservTests.cpp
#     src/launchServer.cpp
#     # Add more test source files here
# )

# # Add the executable for your program
# add_executable(webserv ${SOURCES})

# # Add any required libraries or include directories
# target_include_directories(webserv PRIVATE inc)

# # Set the output directory for the program executable
# set_target_properties(webserv PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..
# )

# # Optional: Specify any additional flags or compiler options
# target_compile_options(webserv PRIVATE -Wall -Wextra -Werror)

# # Add the executable for your tests
# add_executable(webservTests ${TEST_SOURCES})

# # Add any required libraries or include directories for tests
# target_include_directories(webservTests PRIVATE inc)

# # Set the output directory for the test executable
# set_target_properties(webservTests PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..
# )

# # Link the necessary libraries for tests
# target_link_libraries(webservTests GTest::gtest_main)

# # Discover and run tests using GoogleTest
# include(GoogleTest)
# gtest_discover_tests(webservTests)

cmake_minimum_required(VERSION 3.12)
project(webserv)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Set your main program source files
set(SOURCES
    src/ConfigFile.cpp
    src/HttpRequest.cpp 
    src/HttpResponse.cpp 
    src/launchServer.cpp 
    src/main.cpp 
    src/monitorServer.cpp 
    src/utils.cpp
)

set(TEST_SOURCES
    tests/compilationTests.cpp 
    tests/webservTests.cpp
    # Add more test source files here
)

# Add the executable for your program and link it with the library target
add_executable(webserv ${SOURCES})

# Add any required libraries or include directories for the library target
  target_include_directories(webserv PRIVATE inc)

# Set the output directory for the program executable
set_target_properties(webserv PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..
)

# Optional: Specify any additional flags or compiler options
target_compile_options(webserv PRIVATE -Wall -Wextra -Werror)

# Create a static library name webserv_lib taking the sources. However,
# the sources files needs the header files 
add_library(webserv_lib STATIC ${SOURCES})

# Set the output directory for the static library
set_target_properties(webserv_lib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# Mentions the header file needed by my static library above. 
target_include_directories(webserv_lib PRIVATE inc)

# # creates an executable called webservTests taking the TEST souces 
add_executable(webservTests ${TEST_SOURCES})

# Add any required libraries or include directories for tests
#target_include_directories(webservTests PRIVATE inc)

# Set the output directory for the test executable
set_target_properties(webservTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..
)

# Link the test executable with the library target
#target_link_libraries(webservTests PRIVATE webserv_lib GTest::gtest_main)

# Link the test executable with the library target
target_link_libraries(webservTests PRIVATE webserv_lib GTest::gtest_main)

# Discover and run tests using GoogleTest
include(GoogleTest)
gtest_discover_tests(webservTests)
