
#include "../inc/HttpRequest.hpp"

void HttpRequest::parseRequest(std::string rawRequest){
    std::string method, path, body, line;
    std::map<std::string, std::string> headers;
    std::istringstream request(rawRequest);
    request >> method >> path;
    if (method == "GET"){
        size_t quePos = path.find("?");  
        if (quePos != std::string::npos){
                this->queryString =path.substr(quePos +1);
                path = path.substr(0, quePos);
        }
    }
    while(std::getline(request, line) && !line.empty()){
        std::string headerName, headerValue;
        size_t colonPos = line.find(":");
        if(colonPos != std::string::npos){
            headerName = line.substr(0, colonPos);
            headerValue = line.substr(colonPos + 1);
            //the following line delete whitespaces at the begining of the string headerValue and at the end
            headerValue = std::regex_replace(headerValue, std::regex("^\\s+|\\s+$"), ""); 
            headers[headerName] = headerValue; 
        }
    }
    if (path.empty() || path == "/") {
        path = "/index.html";
    }
    //extract the reminder of the resquest into the variable body
    body = request.str();
    this->method = method;
    //we will get the info via the configutation file
    this->path = "/Users/slord/Desktop/13-WEBSERVER/html" + path;
    this->body = body;
    this->headers = headers;
}
void HttpRequest::validityCheck(){
    //this function will check if the resquest sent by the client is a valid one.
}
// Default destructor
HttpRequest::~HttpRequest() { return; }
