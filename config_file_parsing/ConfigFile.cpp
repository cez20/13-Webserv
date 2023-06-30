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
		if (end != std::string::npos) 
			ret = found_line.substr(i, end - i);
		else
			throw BadFormat();
	}

	return (ret);
}

std::string	ConfigFile::parse_found_location(std::string charset, std::string found_line){
	std::string ret;

	size_t i = found_line.find(charset);
	if (i != std::string::npos){
		i += charset.length();
		while (std::isspace(found_line[i]))
			++i;
		size_t end = found_line.find('{', i);
		if (end != std::string::npos) 
			ret = found_line.substr(i, end - i);
		else
			throw BadFormat();
		
	}

	return (ret);
}

std::pair<std::string, std::string>	ConfigFile::split_on_space(std::string str){
	std::string ret[2];
	size_t i = 0;

	i = str.find_first_not_of(" \t\n");
	while(str[i] != ' ' && str[i] != '\t'){
		ret[0] += str[i];
		i++;
	}
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (i == std::string::npos){
		return (std::make_pair(ret[0], ret[0]));
	}
	while (str[i] && str[i] != ';'){
		ret[1] += str[i];
		i++;
	}
	return (std::make_pair(ret[0], ret[1]));
}

std::vector<std::string>	ConfigFile::split_vectors(std::string str, char delimiter){
	std::vector<std::string>	ret;
	std::stringstream ss(str);
	std::string token;

	while(std::getline(ss, token, delimiter)){
		ret.push_back(token);
	}

	return (ret);
}

void	ConfigFile::parse_listen(std::string str){
	if (!str.empty() && str.back() == ';')
		str.pop_back();
	std::vector<std::string> values;
	std::vector<std::string> temp;
	values = split_vectors(str, ' ');
	for (size_t i = 0; i < values.size(); i++){
		size_t j = values[i].find_first_of(":");
		if (j == std::string::npos){
			try{
				_listen[values[i]] = std::atoi(values[i].c_str());
			}
			catch(const std::exception& e){
				throw BadFormat();
			}
		}
		else{
			temp = split_vectors(str, ':');
			_listen[temp[0]] = std::atoi(temp[1].c_str());
		}
	}
}

void	ConfigFile::extract_config_file(){
	std::ifstream infile(_fd_path);
	if (!infile){
		throw EmptyFd();
		return ;
	}
	std::string 						buffer;
	std::string 						temp;
	std::vector<std::string> 			temp_methods;
	std::pair<std::string, std::string> temp_tab;
	ConfigFile::location				temp_struct;
 
	size_t		non_blank;
	bool		location_flag = false;

	std::smatch	matches;
	std::regex 	listen("listen");
	std::regex 	server_name("server_name");
	std::regex 	root("root");
	std::regex 	index("index");
	std::regex 	error_log("error_page");
	std::regex 	access_log("access_log");
	std::regex	include("include");
	std::regex	location("location");
	std::regex	close("\\s*\\}");
	std::regex	methods("methods");
	//std::regex 

	if (infile.is_open()){
		while(getline(infile, buffer)){
			non_blank = buffer.find_first_not_of(" \t\n");
			if(buffer[non_blank] == '#')
				continue ;
			else if (location_flag == true){
				if (std::regex_search(buffer, matches, close)){
					location_flag = false;
					_location.push_back(temp_struct);
				}
				else{
					if (std::regex_search(buffer, matches, listen)){

						temp_struct._loc_listen = parse_found_line(matches.str(), buffer);
					}			
					else if (std::regex_search(buffer, matches, server_name))				
						temp_struct._loc_server_name = parse_found_line(matches.str(), buffer);
					else if (std::regex_search(buffer, matches, root))				
						temp_struct._loc_root = parse_found_line(matches.str(), buffer);
					else if (std::regex_search(buffer, matches, access_log))				
						temp_struct._loc_access_log = parse_found_line(matches.str(), buffer);
					else if (std::regex_search(buffer, matches, index))				
						temp_struct._loc_index = parse_found_line(matches.str(), buffer);
					else if (std::regex_search(buffer, matches, error_log)){
						temp = parse_found_line(matches.str(), buffer);
						temp_tab = split_on_space(temp);
						temp_struct._loc_error_log[temp_tab.first] = temp_tab.second;
					}
					else if (std::regex_search(buffer, matches, methods)){
						temp = parse_found_line(matches.str(), buffer);
						temp_methods = split_vectors(temp, ' ');
						for(size_t i = 0; i < temp_methods.size(); i++)
							temp_struct._loc_methods.push_back(temp_methods[i]);
					}				
				}
			}
			else if (std::regex_search(buffer, matches, location)){
				location_flag = true;
				temp = parse_found_location(matches.str(), buffer);
				temp_struct._loc_location = temp;
			}
			else if (std::regex_search(buffer, matches, include))			
				_include_types = parse_found_line(matches.str(), buffer);
			else if (std::regex_search(buffer, matches, listen)){
				// parse_found_line(matches.str(), buffer);
				parse_listen(parse_found_line(matches.str(), buffer));
			}				
			else if (std::regex_search(buffer, matches, server_name))				
				_server_name = parse_found_line(matches.str(), buffer);
			else if (std::regex_search(buffer, matches, root))				
				_root = parse_found_line(matches.str(), buffer);
			else if (std::regex_search(buffer, matches, index))				
				_index = parse_found_line(matches.str(), buffer);
			else if (std::regex_search(buffer, matches, access_log))				
				_access_log = parse_found_line(matches.str(), buffer);
			else if (std::regex_search(buffer, matches, methods)){
				temp = parse_found_line(matches.str(), buffer);
				temp_methods = split_vectors(temp, ' ');
				for(size_t i = 0; i < temp_methods.size(); i++)
					_methods.push_back(temp_methods[i]);
			}				
			else if (std::regex_search(buffer, matches, error_log)){
				temp = parse_found_line(matches.str(), buffer);
				temp_tab = split_on_space(temp);
				_error_log[temp_tab.first] = temp_tab.second;
			}
			else if (std::regex_search(buffer, matches, include))				
				_include_types = parse_found_line(matches.str(), buffer);
		}
		
	}
}

bool is_string_digit(const std::string& str){
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
        if (!std::isdigit(*it)) {
            return false;
        }
    }
    return true;
}