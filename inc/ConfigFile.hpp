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
#include<cctype>
#include<fstream>
#include<iostream>
#include<regex>
#include<map>
#include<stdexcept>
#include<vector>
#include<sstream>
#include<utility>
#include<unistd.h>
#include"color.h"

class ConfigFile{
	public:
	ConfigFile();
	ConfigFile(std::string configPath);
	ConfigFile(std::string configPath, int index);
	~ConfigFile();
		//LOCATION STRUCTURE
		struct location{
			std::string								_loc_location;
			std::map<std::string, std::string>		_loc_error_page;
			std::vector<std::string>				_loc_listen;
			std::string								_loc_server_name;
			std::string								_loc_root;
			std::string								_loc_access_log;
			std::string								_loc_error_log;
			std::string								_loc_include_types;
			std::string								_loc_index;
			std::string								_loc_auto_index;
			std::string								_loc_return;
			std::string								_loc_cgi_pass;
			std::string								_loc_cgi_pass2;
			std::vector<std::string>				_loc_methods;
			bool									_loc_upload;
			bool									_loc_allow_delete;
			int										_loc_max_body_size;
		};

		std::string 								get_server_name()const {return (this->_server_name);};
		std::string 								get_root()const {return (this->_root);};
		std::string 								get_access_log()const {return (this->_access_log);};
		std::string 								get_error_log()const {return (this->_error_log);};
		std::string 								get_include_types()const {return (this->_include_types);};
		const std::vector<std::string>&				get_methods()const {return (this->_methods);};
		std::vector<std::string>& 					get_listen(){return (this->_listen);};
		std::map<std::string, std::string>&			get_error_page(){return (this->_error_page);};
		const std::vector<ConfigFile::location>&	get_location()const{return (this->_location);};
		std::string 								get_index()const {return (this->_index);};
		int 										get_max_body_size()const {return (this->_max_body_size);};

		void										set_config(std::string configPath, int nb_of_server);
		int											find_nb_of_server(std::string path);
		void 										extract_config_file();
		void										parse_listen(std::string str);
		std::vector<std::string>					parse_location_listen(std::string str);
		std::string									parse_found_line(std::string charset, std::string found_line);
		std::string									parse_found_location(std::string charset, std::string found_line);
		std::vector<std::string>					split_vectors(std::string str, char delimiter);
		std::pair<std::string, std::string>			split_on_space(std::string str);

		class EmptyFd: public std::exception{
			public:
				const char* what() const throw(){return ("Empty or missing fd");};
		};
		class BadFormat: public std::exception{
			public:
				const char* what() const throw(){return ("Formating error, check your configuration file");};
		};



	
	private:
		//CONFIG FILE VARIABLES
		std::vector<location>					_location;
		std::map<std::string, std::string>		_error_page;
		std::vector<std::string>				_listen;
		std::vector<std::string>				_methods;
		std::string								_server_name;
		std::string								_root;
		std::string								_index;
		std::string								_access_log;
		std::string								_error_log;
		std::string								_include_types;
		int										_max_body_size;

		//PATH TO THE CONFIG FILE
		std::string 							_fd_path;
		int										_server_nb;
		int										_max_server_nb;
};

#endif