#include "ConfigFile.hpp"

int main(){
	std::string testfile = "./config.conf";
	try{
		ConfigFile test(testfile);
		std::cout << test.get_listen() << std::endl;
		std::cout << test.get_server_name() << std::endl;
		std::cout << test.get_root() << std::endl;
		std::cout << test.get_access_log() << std::endl;
		std::cout << test.get_error_log() << std::endl;
		std::cout << test.get_include_types() << std::endl;
	}
	catch(const std::exception& e){
		std::cerr << e.what() << '\n';
	}
}