# cmake_minimum_required(VERSION 3.12)
# project(webserv)

# set(CMAKE_CXX_STANDARD 98)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)   #not sure to know what it does. 
# FetchContent_MakeAvailable(googletest)

# enable_testing()      #enable testing in CMake. 

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
#     # Add more test source files here
# )

# # Add the executable for your program
# # Firt word is the name of the executable you want followed by all sources files or macro that represents all source files 
# add_executable(webserv ${SOURCES})

# # Add any required libraries or include directories
# #Name of program, ???? , path where include folders are
# target_include_directories(webserv PRIVATE inc)

# # Set the output directory for the executable
# # Says that the executable will be at the same place where the cmake file is 
# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

# # Optional: Specify any additional flags or compiler options
# target_compile_options(webserv PRIVATE -Wall -Wextra -Werror)

# #Add the executable for your tests
# add_executable(webservTests ${TEST_SOURCES})

# # Add any required libraries or include directories for tests
# target_include_directories(webservTests PRIVATE inc)

# # Add Google Test as a submodule or use an external package
# add_subdirectory(lib/gtest)  # Assuming you have Google Test in a "gtest" directory

# # Link the necessary libraries to your tests
# target_link_libraries(YourTests PRIVATE gtest gtest_main)


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
  tests/compilationTests.cpp    # Srcs files for first series of tests
  tests/webservTests.cpp        # Srcs files for second series of tests
  src/launchServer.cpp
)

target_include_directories(webservTests PRIVATE inc)

target_link_libraries(
  webservTests                   
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(webservTests)
