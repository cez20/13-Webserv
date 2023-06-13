cmake_minimum_required(VERSION 3.14)
project(webserv)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)   #not sure to know what it does. 
FetchContent_MakeAvailable(googletest)

enable_testing()      #enable testing in CMake. 

add_executable(
  webservTests         			# Name of the executable that will be created
  tests/webservTests.cpp        # Srcs files that is available 
)

target_link_libraries(
  webservTests                   
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(webservTests)