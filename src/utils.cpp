#include "../inc/webserv.hpp"

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

