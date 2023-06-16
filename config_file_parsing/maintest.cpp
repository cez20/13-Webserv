#include "ConfigFile.hpp"

int main(){
	std::string testfile = "./configtest.conf";
	ConfigFile test(testfile);
	std::cout << test.get_config_file_content() << std::endl;
}