//#*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*#
//#*                                 88                                                          *# 
//#*                                 88                                                          *# 
//#* 8b      db      d8   ,adPPYba,  88,dPPYba,   ,adPPYba,   ,adPPYba,  8b,dPPYba,  8b       d8 *# 
//#* `8b    d88b    d8'  a8P_____88  88P'    "8a  I8[    ""  a8P_____88  88P'   "Y8  `8b     d8' *# 
//#*  `8b  d8'`8b  d8'   8PP"""""""  88       d8   `"Y8ba,   8PP"""""""  88           `8b   d8'  *# 
//#*   `8bd8'  `8bd8'    "8b,   ,aa  88b,   ,a8"  aa    ]8I  "8b,   ,aa  88            `8b,d8'   *# 
//#*     YP      YP       `"Ybbd8"'  8Y"Ybbd8"'   `"YbbdP"'   `"Ybbd8"'  88              "8"     *# 
//#*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*#


#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include<cstring>
#include<fstream>
#include<iostream>
#include<regex>
#include<map>
#include<utility>
#include"color.h"


class ConfigFile{
	public:
	ConfigFile(std::string configPath);
	~ConfigFile();

		// std::string get_listen(){return (_listen);};
		std::string get_listen_pre_parsed(){return (_listen_pre_parsed);};
		std::string get_server_name(){return (_server_name);};
		std::string get_root(){return (_root);};
		std::string get_access_log(){return (_access_log);};
		std::map<std::string, std::string>& get_error_log(){return (_error_log);};
		std::string get_include_types(){return (_include_types);};
		std::string get_config_file_content(){return (_config_file_content);};
		// std::string get_(){return (_);};

		void 		extract_config_file();
		std::string	parse_found_line(std::string charset, std::string found_line);
		std::string	parse_found_location(std::string charset, std::string found_line);
		std::pair<std::string, std::string>	split_on_space(std::string str);

		class EmptyFd: public std::exception{
			public:
				const char* what() const throw(){return ("Empty or missing fd");};
		};
		class BadFormat: public std::exception{
			public:
				const char* what() const throw(){return ("Formating error, check your configuration file");};
		};
		std::map<std::string, std::string>		_error_log;
		struct location{
			std::map<std::string, std::string>	_loc_error_log;
			std::string							_loc_listen;
			std::string							_loc_server_name;
			std::string							_loc_root;
			std::string							_loc_access_log;
			std::string							_loc_include_types;
			std::string							_loc_index;
		};
		std::map<std::string, location>			_location; 

	private:
		std::string 							_fd_path;
		std::string 							_config_file_content;
		std::string								_listen_pre_parsed;
		std::vector<std::string>				_listen;
		std::string								_server_name;
		std::string								_root;
		std::string								_index;
		std::string								_access_log;
		std::string								_include_types;

};

#endif