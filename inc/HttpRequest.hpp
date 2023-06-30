#pragma once
//to do : vars private, getter & setter
#include "webserv.hpp"
class ServerConfiguration;
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
        const ServerConfiguration& config;
        void showRequest()const {
            std::cout << method << "  " << path << "  " <<  std::endl << body << std::endl;
        }
        // Add any other necessary members or methods
        
        // Constructor
        HttpRequest(std::string rawRequest, const ServerConfiguration& config): config(config){
            parseRequest(rawRequest, config);
            checkCgi(config);
            checkDownload(config);
        }
        // Destructor
        ~HttpRequest();
    private:
        void parseRequest(std::string rawRequest, const ServerConfiguration& config);
        void validityCheck();
        void checkCgi(const ServerConfiguration& config);
       // void cleanPath(const ServerConfiguration& config);
        void checkDownload(const ServerConfiguration& config);   
};