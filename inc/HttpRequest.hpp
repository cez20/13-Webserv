#pragma once
#include "webserv.hpp"
#include "ConfigFile.hpp"
class ConfigFile;

//struct location;
class HttpRequest {
    public:
        std::string                         method;
        std::string                         path;
        std::map<std::string, std::string>  headers;
        std::string                         body;
        std::string                         queryString;
        bool                                isValid;
        bool                                toBeDownloaded;
        bool                                isCgi;
        ConfigFile::location                locationRequest;
        std::string							index;
		std::vector<std::string>			autorizedMethods;
        std::string                         redir;
        bool                                autoIndex;
        std::string                         reponseStatus;
        const ConfigFile&                   config;
        bool                                upload;
        std::string                         boundary;
        std::map<std::string, std::string>  multiBody;
        bool                                allow_delete;
        int                                 max_body;
        int                                 contentLength;

    public:
        HttpRequest(std::string rawRequest, const ConfigFile& config): config(config){
            parseRequest(rawRequest);
            checkGlobal(this->config);
            if(!config.get_location().empty())
                checkLocation(this->config);
            checkCgi();
            checkDownload(this->config);
        }

        void showRequest()const {
            std::cout << method << "  " << path << "  " <<  std::endl << body << std::endl;
        }
        ~HttpRequest();

    private:
        void parseRequest(std::string rawRequest);
        void checkCgi();
        void checkGlobal(const ConfigFile& config);
        void checkLocation(const ConfigFile& config);
        void checkDownload(const ConfigFile& config);  
        void getBoundary();
        void parseMultipartFormData();
};