#include "webserv.hpp"

//extract a string from a file
std::string extractFileContent(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return "";
    }
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + '\n';
    } 
    file.close();
    return content;
}
bool endsWith(const std::string& str, const std::string& suffix) {
    if (str.length() >= suffix.length()) {
        return (str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0);
    }
    return false;
}

bool isDirectory(const std::string& path) {
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0)
        return false;
    return S_ISDIR(statbuf.st_mode);
}


int	find_nb_of_server(std::string path){
	std::ifstream	infile(path);
	if (!infile){
		throw std::runtime_error("Trying to create more configuration file object than the number of server block in the file.");
		return (1);
	}
	std::string buffer;
	std::regex 	server("server \\{");
	int			count = 0;

	if (infile.is_open()){
		while(getline(infile, buffer)){
			if (std::regex_search(buffer, server))
				count++;
		}
	}

	return (count);
}