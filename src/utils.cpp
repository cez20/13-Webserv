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


