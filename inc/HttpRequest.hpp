#pragma once
//to do : vars private, getter & setter
#include "webserv.hpp"
class ConfigFile;
//struct location;
class HttpRequest {
    public:
        std::string method;
        std::string path;
        std::map<std::string, std::string> headers;
        std::string body;
        std::string queryString;
        bool isValid;
        bool toBeDownloaded;
        bool isCgi;
        //const location*  locationRequest; 
        const ConfigFile& config;
        void showRequest()const {
            std::cout << method << "  " << path << "  " <<  std::endl << body << std::endl;
        }
        // Add any other necessary members or methods
        
        // Constructor
        HttpRequest(std::string rawRequest, const ConfigFile& config): config(config){
            parseRequest(rawRequest, config);
            checkCgi(config);
            checkDownload(config);
        }
        // Destructor
        ~HttpRequest();
    private:
        void parseRequest(std::string rawRequest, const ConfigFile& config);
        void validityCheck();
        void checkCgi(const ConfigFile& config);
       // void cleanPath(const ConfigFile& config);
        void checkDownload(const ConfigFile& config);   
};