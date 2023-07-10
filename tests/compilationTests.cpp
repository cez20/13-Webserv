#include <gtest/gtest.h>
#include <iostream>
#include <filesystem>

TEST(CompilationTest, MakefileCompiles) {
  std::string directory = "../";  
  int chdirStatus = chdir(directory.c_str());
  EXPECT_EQ(chdirStatus, 0) << "Failed to change directory.";

  // Execute the Makefile command and check for return status
  int returnStatus = system("make");
  EXPECT_EQ(returnStatus, 0) << "Makefile compilation failed.";
}