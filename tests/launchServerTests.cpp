#include <gtest/gtest.h>
#include <iostream>
#include <filesystem>
#include "../inc/webserv.hpp"
#include "../inc/HttpResponse.hpp"
#include "../inc/HttpRequest.hpp"
#include "../inc/ConfigFile.hpp"
#include "../inc/color.h"


// TESTING GETNETWORKINFO() FUNCTION 
TEST(GetNetworkInfoTest, WrongPort) {

    struct addrinfo* result = getNetworkInfo("-1");
    EXPECT_EQ(result, nullptr);
}


// TESTING THE SERVERSOCKETSETUP FUNCTION 
TEST(serverSocketSetupTest, PassingNULL) {
   
	int result = serverSocketSetup(nullptr);
	EXPECT_EQ(result, -1);
}

TEST(serverSocketSetupTest, WrongAddressFamily) {
   
  	struct addrinfo server;
  
  	memset(&server, 0, sizeof(server));
  	server.ai_family 	= 42;  // Error 
	server.ai_socktype 	= SOCK_STREAM;		
	server.ai_flags 	= AI_PASSIVE;		

   	int result = serverSocketSetup(&server);	
	EXPECT_EQ(result, -1);
}

TEST(serverSocketSetupTest, WrongSocketType) {
   
  	struct addrinfo server;
  
  	memset(&server, 0, sizeof(server));
  	server.ai_family 	= AF_INET ;			
	server.ai_socktype 	= SOCK_DGRAM;		// Error, wrong type of socket  
	server.ai_flags 	= AI_PASSIVE;		
	server.ai_protocol 	= IPPROTO_TCP;	

   	int result = serverSocketSetup(&server);	
	EXPECT_EQ(result, -1);
}

TEST(serverSocketSetupTest, ProtocolTooHigh) {
   
  	struct addrinfo server;
  
  	memset(&server, 0, sizeof(server));
  	server.ai_family 	= AF_INET ;			
	server.ai_socktype 	= SOCK_DGRAM;		
	server.ai_flags 	= AI_PASSIVE;		
	server.ai_protocol 	= 259;	          // ERROR, wrong protocol 

   	int result = serverSocketSetup(&server);	
	EXPECT_EQ(result, -1);
}

TEST(serverSocketSetupTest, BindingError1) {
   
  	struct addrinfo *server = getNetworkInfo("80");		
   	int result = serverSocketSetup(server);	

	EXPECT_EQ(result, -1);
}



int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}