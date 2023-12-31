
#ifndef HTTPRESPONSE_HPP_
#define HTTPRESPONSE_HPP_

#include "HttpRequest.hpp"
#include "webserv.hpp"

class HttpRequest;
class HttpResponse {
    public:
        HttpResponse(const HttpRequest& clientRequest);
        int writeOnSocket(const int& clientSocket);
        void showResponse()const {
            std::cout << this->statusCode<< "  " << body << "  " << std::endl;
        }
    
        //~HttpResponse();

    private:
        std::string statusCode;
        std::map<std::string, std::string> headers;
        std::map<std::string, std::string> httpStatusMap;
        std::string body;
        std::string path;

        //bool isValid;

        int analyseRequest(const HttpRequest& clientRequest);
        bool fileExist(const std::string& filename);
        std::string executeCgiGet(const HttpRequest& clientRequest);
        std::string executeCgiPost(const HttpRequest& clientRequest);
        void analyseCgiOutput(const std::string& output);
        int responseForStatic(const HttpRequest& clientRequest);
        int deleteMethod(const HttpRequest& clientRequest);
        void checkForError(const HttpRequest& clientRequest);
        void generateStatusMap();
        void generateDefaultError();
        void autoListing();
        void generateDirListing(std::vector<std::string> vecList);
        int uploading(const std::map<std::string, std::string> &multiBody, const std::string &path);
        int checkForCustomErrorFiles(const HttpRequest& clientRequest);

};

#endif // HTTPRESPONSE_HPP_


