//#*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*#
//#*                                 88                                                          *# 
//#*                                 88                                                          *# 
//#* 8b      db      d8   ,adPPYba,  88,dPPYba,   ,adPPYba,   ,adPPYba,  8b,dPPYba,  8b       d8 *# 
//#* `8b    d88b    d8'  a8P_____88  88P'    "8a  I8[    ""  a8P_____88  88P'   "Y8  `8b     d8' *# 
//#*  `8b  d8'`8b  d8'   8PP"""""""  88       d8   `"Y8ba,   8PP"""""""  88           `8b   d8'  *# 
//#*   `8bd8'  `8bd8'    "8b,   ,aa  88b,   ,a8"  aa    ]8I  "8b,   ,aa  88            `8b,d8'   *# 
//#*     YP      YP       `"Ybbd8"'  8Y"Ybbd8"'   `"YbbdP"'   `"Ybbd8"'  88              "8"     *# 
//#*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*#

#include<fstream>
// #include<sstream>

#include"../inc/webserv.hpp"

int	error_logs(std::string msg, const ConfigFile& config){
	std::time_t currentTime = std::time(nullptr);
	std::tm* localTime = std::localtime(&currentTime);
	std::string timeString = std::asctime(localTime);
	if (timeString.back() == '\n')
		timeString.pop_back();

	std::string error_log = config.get_error_log();

	if (error_log.empty())
		for (size_t i = 0; i < config.get_location().size(); ++i){
    		if (!config.get_location()[i]._loc_error_log.empty()){
            	error_log = config.get_location()[i]._loc_error_log;
				break ;
			}
    }
	if (error_log.empty())
		error_log = "./logs/error_log.txt";

	std::ofstream log(error_log.c_str(), std::ios::app);
	if (!log){
		std::cerr << "Error: failed to open error log" << std::endl;
		return (1);
	}
	
	log << timeString << ": " << msg  << std::endl;
	log.close();
	return (0);
}

int	access_logs(std::string msg, const ConfigFile& config){
	std::time_t currentTime = std::time(nullptr);
	std::tm* localTime = std::localtime(&currentTime);
	std::string timeString = std::asctime(localTime);
	if (timeString.back() == '\n')
		timeString.pop_back();

	std::string access_log = config.get_access_log();

	if (access_log.empty())
		for (size_t i = 0; i < config.get_location().size(); ++i){
    		if (!config.get_location()[i]._loc_access_log.empty()){
            	access_log = config.get_location()[i]._loc_access_log;
				break ;
			}
    }
	if (access_log.empty())
		access_log = "./logs/access_log.txt";

	std::ofstream log(access_log.c_str(), std::ios::app);
	if (!log){
		std::cerr << "Error: failed to open access log" << std::endl;
		return (1);
	}
	
	log << timeString << ": " << msg  << std::endl;
	log.close();
	return (0);
}
