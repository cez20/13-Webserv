#include <gtest/gtest.h>
#include <iostream>
#include <filesystem>
#include "../inc/webserv.hpp"
// #include "../inc/HttpResponse.hpp"
// #include "../inc/HttpRequest.hpp"
// #include "../inc/ConfigFile.hpp"
// #include "../inc/color.h"



TEST(GetNetworkInfoTest, ReturnsNonNullPointer) {
    // Call the getNetworkInfo function
    struct addrinfo* result = getNetworkInfo("8080");

    // Assert that the result is not null
    EXPECT_NE(result, nullptr);

    // Clean up any allocated memory if necessary
    // ...

    // Add more assertions or tests as needed
}

TEST(SetupServer, Test1) {
   
	int result = serverSocketSetup(nullptr);

	EXPECT_EQ(result, -1);

}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}