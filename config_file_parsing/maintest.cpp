#include "./ConfigFile.hpp"

int main() {
    std::string testfile = "./config3.conf";
    try {
		ConfigFile* test = new ConfigFile[2];
    	test[0].set_config(testfile, 0);
    	test[1].set_config(testfile, 1);

		
        std::vector<std::string> temp_listen = test[0].get_listen();
        for (size_t i = 0; i < temp_listen.size(); i++) {
            std::cout << "Key: " << temp_listen[i] << std::endl;
        }
		std::cout << "test" << std::endl;
        std::cout << test[0].get_server_name() << std::endl;
        std::cout << test[0].get_root() << std::endl;
        std::cout << test[0].get_access_log() << std::endl;
        std::cout << test[0].get_methods()[0] << std::endl;
        std::cout << test[0].get_methods()[1] << std::endl;
        std::cout << test[0].get_methods()[2] << std::endl;
        std::map<std::string, std::string>::const_iterator iter;
        std::map<std::string, std::string> temp_error_page = test[0].get_error_page();
        for (iter = temp_error_page.begin(); iter != temp_error_page.end(); ++iter) {
            std::cout << "Key: " << iter->first << ", Value: " << iter->second << std::endl;
        }
        std::vector<ConfigFile::location>::const_iterator it;
        std::vector<ConfigFile::location> temp_location = test[0].get_location();
        for (it = temp_location.begin(); it != temp_location.end(); ++it) {
            std::cout << "Location: " << it->_loc_location << std::endl;
            for (size_t i = 0; i < it->_loc_listen.size(); i++) {
                std::cout << "  Listen: " << it->_loc_listen[i] << std::endl;
            }
            std::cout << "  Server Name: " << it->_loc_server_name << std::endl;
            std::cout << "  Root: " << it->_loc_root << std::endl;
            std::cout << "  Access Log: " << it->_loc_access_log << std::endl;
            std::cout << "  Include Types: " << it->_loc_include_types << std::endl;
            std::cout << "  Index: " << it->_loc_index << std::endl;
            std::cout << "  Methods:" << std::endl;
            for (size_t i = 0; i < it->_loc_methods.size(); i++) {
                std::cout << "    " << it->_loc_methods[i] << std::endl;
            }
            std::cout << "  Error Log:" << std::endl;
            std::map<std::string, std::string>::const_iterator errorLogIt;
            for (errorLogIt = it->_loc_error_page.begin(); errorLogIt != it->_loc_error_page.end(); ++errorLogIt) {
                std::cout << "    Key: " << errorLogIt->first << ", Value: " << errorLogIt->second << std::endl;
            }
        }
        std::cout << test[0].get_include_types() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
