#include <gtest/gtest.h>
#include <iostream>
#include <filesystem>

TEST(CompilationTest, ProgramCompilesAndCreatesExecutable)
{
    // Get the current working directory
  	std::__fs::filesystem::path currentPath = std::__fs::filesystem::current_path();

    // Move one level above
    std::__fs::filesystem::path parentPath = currentPath.parent_path();

    // Change the working directory to the parent folder
    std::__fs::filesystem::current_path(parentPath);

    // Assert that the make command executes successfully
    EXPECT_EQ(0, system("make"));

    // Check if the executable file exists
    EXPECT_TRUE(std::__fs::filesystem::exists("webserv"));
}
