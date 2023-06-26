
#include "../inc/HttpRequest.hpp"

void HttpRequest::parseRequest(std::string rawRequest, const ServerConfiguration& config){
    std::string method, path, body, line;
    std::map<std::string, std::string> headers;
    std::istringstream request(rawRequest);
    request >> method >> path;
    if (method == "GET"){
        size_t quePos = path.find("?");  
        if (quePos != std::string::npos){
                this->querryString =path.substr(quePos +1);
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
    this->path = config.getDocumentRoot() + path;
    this->body = body;
    this->headers = headers;
}
void HttpRequest::validityCheck(){
}
void HttpRequest::checkCgi(const ServerConfiguration& config){
    if(this->path == config.getCgiRoot() + "/test.php"){
        this->isCgi = true;
    }
     else
        this->isCgi = false;
 }
 void HttpRequest::checkDownload(const ServerConfiguration& config){
    if(endsWith(this->path, ".pdf")){
        this->toBeDownloaded = true;
    }
     else
        this->toBeDownloaded = false;
    std::string test = config.getCgiRoot();

 }
// Default destructor
HttpRequest::~HttpRequest() { return; }
