
#include "HttpRequest.hpp"
//This funtion get the method, the headers and the body from the resquest.
void HttpRequest::parseRequest(std::string rawRequest) {
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
    this->path = path;
    this->boundary = "";
    getBoundary();
    if (!this->boundary.empty()){
        parseMultipartFormData();
        printMap(multiBody);
    }
        std::cout << this->boundary << std::endl;

}



//Check if the path is the CGI
void HttpRequest::checkCgi() {
    std::ifstream file(this->path);
    if (file.good() && endsWith(this->path, "/test.php")) {
        this->isCgi = true;
    } else {
        this->isCgi = false;
    }
}
void HttpRequest::checkDownload(const ConfigFile& config){
    if(endsWith(this->path, ".pdf")){
        this->toBeDownloaded = true;
    }
     else
        this->toBeDownloaded = false;
    std::string test = config.get_root();

 }
 //Check if the resquest correspond to  location path from de config file and set the variables to reflect those specific configurations
void HttpRequest::checkLocation(const ConfigFile& config){
    //checking the if there if a specific locaton for the resquest path
    for (size_t i = 0; i < config.get_location().size(); ++i){
        if (config.get_location()[i]._loc_location.find(this->path))
            this->locationRequest = config.get_location()[i];
    }
    //change the resquest path with the new location
    if (this->locationRequest._loc_location == "/"  && !this->locationRequest._loc_root.empty())
    {
        std::string newRoot = locationRequest._loc_root + "/";
        std::size_t last_slash = this->path.find_last_of('/');
        if (last_slash != std::string::npos)
            this->path.replace(last_slash, 1, newRoot);
    }
    else if (!this->locationRequest._loc_root.empty()){
        std::string to_replace = this->locationRequest._loc_location; 
        size_t pos = this->path.find(to_replace);
        if (pos != std::string::npos)
            this->path.replace(pos, to_replace.length(), this->locationRequest._loc_root);
    }
    if (!this->locationRequest._loc_index.empty())
        this->index = this->locationRequest._loc_index;
    if (!this->locationRequest._loc_methods.empty())  
        this->autorizedMethods = this->locationRequest._loc_methods;
    if (!this->locationRequest._loc_return.empty())
        this->redir = this->locationRequest._loc_return;
    if (this->locationRequest._loc_auto_index == "on")
        this->autoIndex = true;
    else
        this->autoIndex = false;  
    if (!this->redir.empty())
        this->reponseStatus = "301";
    
    std::cout << "LE NOUVEAU PATH" <<this->path << std::endl;
    // if (!location->limit_except.empty())
    //     this->limit_except = location->limit_except;
}
//Check the config file for global parameters et set the variables accordingly 
void HttpRequest::checkGlobal(const ConfigFile& config){
    this->path = config.get_root() + this->path;
    this->autorizedMethods = config.get_methods();

}
void HttpRequest::getBoundary() {
    std::string boundary;

    std::map<std::string, std::string>::iterator it = headers.find("Content-Type");
    if (it != headers.end()) {
        std::string contentType = it->second;

        // Vérifier si le type de contenu est multipart/form-data
        std::string multipartKeyword = "multipart/form-data";
        size_t pos = contentType.find(multipartKeyword);
        if (pos != std::string::npos) {
            // Extraire le délimiteur (boundary)
            std::string boundaryKeyword = "boundary=";
            pos = contentType.find(boundaryKeyword);
            if (pos != std::string::npos) {
                boundary = contentType.substr(pos + boundaryKeyword.length());
            }
        }
    }
  
    this->boundary = boundary;
      std::cout << "this is the boundary" <<boundary << std::endl;
      std::cout << "this is the body" <<body << std::endl;
}

void HttpRequest::parseMultipartFormData() {
    std::string delimiter = "--" + this->boundary;
    std::string endDelimiter = delimiter + "--";

    size_t pos = body.find(delimiter);
    while (pos != std::string::npos) {
        size_t startPos = pos + delimiter.length() + 2; // +2 pour sauter les retours à la ligne
        size_t endPos = body.find(delimiter, startPos);
        if (endPos == std::string::npos) {
            break;
        }

        std::string part = body.substr(startPos, endPos - startPos);
        size_t filenamePos = part.find("filename=\"");
        if (filenamePos != std::string::npos) {
            filenamePos += 10; // "fileName=" = 10
            size_t filenameEndPos = part.find("\"", filenamePos);
            if (filenameEndPos != std::string::npos) {
                std::string filename = part.substr(filenamePos, filenameEndPos - filenamePos);

                
                size_t contentPos = part.find("\r\n\r\n") + 4; // +4 pour sauter endoffile
                std::string fileContent = part.substr(contentPos);

                // Stocker le fichier dans la map, pour linstant je vais jsute chercher le filename. Je vais peut etre avoir besoin de autres choses.
                multiBody[filename] = fileContent;
            }
        }

        pos = body.find(delimiter, endPos);
    }
}





void HttpRequest::validityCheck(){
}
HttpRequest::~HttpRequest() { return; }
