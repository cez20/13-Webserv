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
        ConfigFile&                         config;
        bool                                upload;
        std::string                         boundary;
        std::map<std::string, std::string>  multiBody;
        bool                                allow_delete;
        int                                 max_body;
        int                                 contentLength;
        std::string                         serverName;
        std::string                         cgiPass;

    public:
        HttpRequest(std::string rawRequest, ConfigFile& config): config(config){
            parseRequest(rawRequest);
            checkServerName(config);
            std::cout<< "SERVER_NAME ::  " << this->config.get_server_name() << std::endl;;
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
        void checkGlobal( ConfigFile& config);
        void checkLocation( ConfigFile& config);
        void checkDownload(ConfigFile& config);  
        void getBoundary();
        void parseMultipartFormData();
        void checkServerName( ConfigFile& config);
};