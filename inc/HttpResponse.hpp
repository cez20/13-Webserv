
#ifndef HTTPRESPONSE_HPP_
#define HTTPRESPONSE_HPP_

#include "HttpRequest.hpp"
#include "webserv.hpp"

class HttpRequest;
class HttpResponse {
    public:
        HttpResponse(const HttpRequest& clientRequest);
        int writeOnSocket(const int& clientSocket);
    
        //~HttpResponse();

    private:
        std::string statusCode;
        std::map<std::string, std::string> headers;
        std::string body;
        //bool isValid;

        int analyseRequest(const HttpRequest& clientRequest);
        bool fileExist(const std::string& filename);

};

#endif // HTTPRESPONSE_HPP_
