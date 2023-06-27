
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
        std::string body;
        //bool isValid;

        int analyseRequest(const HttpRequest& clientRequest);
        bool fileExist(const std::string& filename);
        std::string executeCgi(const HttpRequest& clientRequest);
        void analyseCgiOutput(const std::string& output);
        int getMethod(const HttpRequest& clientRequest);
        int postMethod(const HttpRequest& clientRequest);
        int deleteMethod(const HttpRequest& clientRequest);

};

#endif // HTTPRESPONSE_HPP_


