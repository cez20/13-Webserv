#pragma once
//to do : vars private, getter & setter
#include "../inc/webserv.hpp"
class ServerConfiguration;
class HttpRequest {
    public:
        std::string method;
        std::string path;
        std::map<std::string, std::string> headers;
        std::string body;
        std::string querryString;
        bool isValid;
        bool isCgi;
        const ServerConfiguration& config;
        void showRequest(){
            std::cout << method << "  " << path << "  " << std::endl;
        }
        // Add any other necessary members or methods
        
        // Constructor
        HttpRequest(std::string rawRequest, const ServerConfiguration& config): config(config){
            parseRequest(rawRequest);
            checkCgi(config);
        }
        // Destructor
        ~HttpRequest();
    private:
        void parseRequest(std::string rawRequest);
        void validityCheck();
        void checkCgi(const ServerConfiguration& config);
      
};