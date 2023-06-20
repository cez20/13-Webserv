//#*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*#
//#*                                 88                                                          *# 
//#*                                 88                                                          *# 
//#* 8b      db      d8   ,adPPYba,  88,dPPYba,   ,adPPYba,   ,adPPYba,  8b,dPPYba,  8b       d8 *# 
//#* `8b    d88b    d8'  a8P_____88  88P'    "8a  I8[    ""  a8P_____88  88P'   "Y8  `8b     d8' *# 
//#*  `8b  d8'`8b  d8'   8PP"""""""  88       d8   `"Y8ba,   8PP"""""""  88           `8b   d8'  *# 
//#*   `8bd8'  `8bd8'    "8b,   ,aa  88b,   ,a8"  aa    ]8I  "8b,   ,aa  88            `8b,d8'   *# 
//#*     YP      YP       `"Ybbd8"'  8Y"Ybbd8"'   `"YbbdP"'   `"Ybbd8"'  88              "8"     *# 
//#*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*#

#include "ConfigFile.hpp"

ConfigFile::ConfigFile(std::string configPath): _fd_path(configPath){
	extract_config_file();
}

ConfigFile::~ConfigFile(){}

//const char* ConfigFile::EmptyFd::what() const throw() {return ("Empty or missing fd");}

std::string	ConfigFile::parse_found_line(std::string charset, std::string found_line){
	std::string ret;

	size_t i = found_line.find(charset);
	if (i != std::string::npos){
		i += charset.length();
		while (std::isspace(found_line[i]))
			++i;
		size_t end = found_line.find(';', i);
		if (end != std::string::npos) {
			ret = found_line.substr(i, end - i);
		}
	}

	return (ret);
}

void	ConfigFile::extract_config_file(){
	std::ifstream infile(_fd_path);
	if (!infile){
		throw EmptyFd();
		return ;
	}
	// int			match_index = 0;
	std::string buffer;

	std::smatch	matches;
	std::regex 	listen("listen");
	std::regex 	server_name("server_name");
	std::regex 	root("root");
	std::regex 	error_404("error_page 404");
	//std::regex 

	if (infile.is_open()){
		while(getline(infile, buffer)){
			if (std::regex_search(buffer, matches, listen))				
				_listen = parse_found_line(matches[0], buffer);
			if (std::regex_search(buffer, matches, server_name))				
				_server_name = parse_found_line(matches[0], buffer);
			if (std::regex_search(buffer, matches, root))				
				_root = parse_found_line(matches[0], buffer);
			// if (std::regex_search(buffer, matches, error_404))				
			// 	_root = parse_found_line(matches[0], buffer);
		}
		
	}
}
