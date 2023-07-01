
#include "HttpRequest.hpp"

void HttpRequest::parseRequest(std::string rawRequest, const ConfigFile& config) {
    std::string method, path, line;
    std::map<std::string, std::string> headers;
    std::istringstream request(rawRequest);
    // Parse the request line to get the method and path
    request >> method >> path;
    if (method == "GET") {
        size_t quePos = path.find("?");
        if (quePos != std::string::npos) {
            this->queryString = path.substr(quePos + 1);
            path = path.substr(0, quePos);
        }
    }
    //Je vais devoir rajouter une fonction pour modifier le path en en vertu des locations que je recois. Et consquemment, en fonction de la configuration specifique de cette location, modifier les variables de ma resquest.
    // Parse the headers
    while (std::getline(request, line) && !line.empty()) {
        std::string headerName, headerValue;
        size_t colonPos = line.find(":");
        if (colonPos != std::string::npos) {
            headerName = line.substr(0, colonPos);
            headerValue = line.substr(colonPos + 1);
            headerValue = std::regex_replace(headerValue, std::regex("^\\s+|\\s+$"), "");
            headers[headerName] = headerValue;
        }
    }
    //find the end of the headers and throw the rest into the ody varariable
    size_t separatorPos = rawRequest.find("\r\n\r\n");
    if (separatorPos != std::string::npos) {
        this->body = rawRequest.substr(separatorPos + 4);
    } else {
        this->body = "";
    }
    if (path.empty() || path == "/") {
        path = "/index.html";
    }
    this->headers = headers;
    this->method = method;
    this->path = config.get_root() + path;
}

void HttpRequest::validityCheck(){
}
void HttpRequest::checkCgi(const ConfigFile& config){
    if(this->path == config.get_root() + "/test.php"){
        this->isCgi = true;
    }
     else
        this->isCgi = false;
 }
void HttpRequest::checkDownload(const ConfigFile& config){
    if(endsWith(this->path, ".pdf")){
        this->toBeDownloaded = true;
    }
     else
        this->toBeDownloaded = false;
    std::string test = config.get_root();

 }
void HttpRequest::cleanPath(const ConfigFile& config){
    //checking the if there if a specific locaton for the resquest path
    for (size_t i = 0; i < config.get_location().size(); ++i){
        if (config.get_location()[i]._loc_location.find(this->path))
            this->locationRequest = config.get_location()[i];
    }
    //change the resquest path with the new location
    std::string to_replace = this->locationRequest._loc_location; 
    size_t pos = this->path.find(to_replace);
    if (pos != std::string::npos)
        this->path.replace(pos, to_replace.length(), this->locationRequest._loc_root);
    this->index = this->locationRequest._loc_index;
    this->autorizedMethods = this->locationRequest._loc_methods;



    
    //verifier le repertoire par default "/, mias je pense que cva va se faire tout seul"
    // if (!location->root.empty())
    //     this->path.replace(0, this->location->name.size(), this->location->root);
    // this->index = location->index;
    // if (!location->limit_except.empty())
    //     this->limit_except = location->limit_except;
    // if (!location->return.empty())
    //     this->redirection = location.return;
    //     //je vais devoir prend la map dans la struct po
}

HttpRequest::~HttpRequest() { return; }
