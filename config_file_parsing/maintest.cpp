#include "./ConfigFile.hpp"

int main(){
	std::string testfile = "./config3.conf";
	try{
		ConfigFile test(testfile);
		std::cout << test.get_listen_pre_parsed() << std::endl;
		std::cout << test.get_server_name() << std::endl;
		std::cout << test.get_root() << std::endl;
		std::cout << test.get_access_log() << std::endl;
		std::map<std::string, std::string>::const_iterator iter;
		for (iter = test._error_log.begin(); iter != test._error_log.end(); ++iter) {
       		std::cout << "Key: " << iter->first << ", Value: " << iter->second << std::endl;
    	}
		std::map<std::string, ConfigFile::location>::iterator it;
		for (it = test._location.begin(); it != test._location.end(); ++it) {
   			std::cout << "Key: " << it->first << std::endl;
			std::cout << "  Listen: " << it->second._loc_listen << std::endl;
			std::cout << "  Server Name: " << it->second._loc_server_name << std::endl;
			std::cout << "  Root: " << it->second._loc_root << std::endl;
			std::cout << "  Index: " << it->second._loc_index << std::endl;
			std::cout << "  Access Log: " << it->second._loc_access_log << std::endl;
			std::cout << "  Include Types: " << it->second._loc_include_types << std::endl;
	    	std::cout << "  Error Log:" << std::endl;
    		std::map<std::string, std::string>::const_iterator errorLogIt;
   				for (errorLogIt = it->second._loc_error_log.begin(); errorLogIt != it->second._loc_error_log.end(); ++errorLogIt) {
       				std::cout << "    Key: " << errorLogIt->first << ", Value: " << errorLogIt->second << std::endl;
   				}
		}	
		std::cout << test.get_include_types() << std::endl;
	}
	catch(const std::exception& e){
		std::cerr << e.what() << '\n';
	}
	return (0);
}