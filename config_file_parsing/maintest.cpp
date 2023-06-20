#include "ConfigFile.hpp"

int main(){
	std::string testfile = "./configtest.conf";
	try{
		ConfigFile test(testfile);
		std::cout << test.get_listen() << std::endl;
		std::cout << test.get_server_name() << std::endl;
	}
	catch(const std::exception& e){
		std::cerr << e.what() << '\n';
	}
}