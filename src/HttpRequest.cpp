
#include "HttpRequest.hpp"
//This funtion get the method, the headers and the body from the resquest.
void HttpRequest::parseRequest(std::string rawRequest) {
    std::string method, path, line;
    std::map<std::string, std::string> headers;
    std::istringstream request(rawRequest);

    this->boundary = "";
    // Parse the request line to get the method and path
    request >> method >> path;
    if (method == "GET") {
        size_t quePos = path.find("?");
        if (quePos != std::string::npos) {
            this->queryString = path.substr(quePos + 1);
            path = path.substr(0, quePos);
        }
    }
   
    while (std::getline(request, line)) {
       if (line.empty() || line == "--" + this->boundary) {
        // Ligne vide ou correspond à la boundary, nous avons atteint le corps de la requête
        break;
       }
        std::string headerName, headerValue;
        size_t colonPos = line.find(":");
        if (colonPos != std::string::npos) {
            headerName = line.substr(0, colonPos);
            headerValue = line.substr(colonPos + 1);
            headerValue = std::regex_replace(headerValue, std::regex("^\\s+|\\s+$"), "");
            headers[headerName] = headerValue;
            if(headerName == "Content-Length"){
                this->contentLength =  std::stoi(headerValue);
            }
            if(headerName == "Host"){
                this->serverName =  headerValue;
            }
            if (headerName == "Content-Type" && headerValue.find("multipart") != std::string::npos) {
            // Extraire la valeur du paramètre boundary
                size_t boundaryPos = headerValue.find("boundary=");
                if (boundaryPos != std::string::npos) {
                    this->boundary = headerValue.substr(boundaryPos + 9);
                }
            }
        }
    }
    //find the end of the headers and throw the rest into the ody varariable
    size_t separatorPos = rawRequest.find("\r\n\r\n");
    if (separatorPos != std::string::npos) {
        this->body = rawRequest.substr(separatorPos + 4);
    } else {
        this->body = "";
    }
    this->headers = headers;
    this->method = method;
    this->path = path;
    
     //getBoundary();
    if (!this->boundary.empty()){
        parseMultipartFormData();
    }
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
void HttpRequest::checkDownload(ConfigFile& config){
    if(endsWith(this->path, ".pdf")){
        this->toBeDownloaded = true;
    }
     else
        this->toBeDownloaded = false;
    std::string test = config.get_root();

 }
 //Check if the resquest correspond to  location path from de config file and set the variables to reflect those specific configurations
void HttpRequest::checkLocation(ConfigFile& config){
    //checking the if there if a specific locaton for the resquest path
    for (size_t i = 0; i < config.get_location().size(); ++i){
        if (this->path.find(config.get_location()[i]._loc_location) != std::string::npos)
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
    if (!this->locationRequest._loc_index.empty()){
        this->index = this->locationRequest._loc_index;
    }
    if (!this->locationRequest._loc_methods.empty())  
        this->autorizedMethods = this->locationRequest._loc_methods;
    if (!this->locationRequest._loc_return.empty())
        this->redir = this->locationRequest._loc_return;
    else {
        this->redir = "";
    }
    if (this->locationRequest._loc_auto_index == "on")
        this->autoIndex = true;
    else
        this->autoIndex = false;  
    this->allow_delete = this->locationRequest._loc_allow_delete;
    this->upload = this->locationRequest._loc_upload;
    if(this->locationRequest._loc_max_body_size != -1)
        this->max_body= this->locationRequest._loc_max_body_size;
    else
        this->max_body= config.get_max_body_size();
    this->cgiPass = locationRequest._loc_cgi_pass;
   
    std::cout << "LE NOUVEAU PATH : " <<this->path << std::endl;

}
//Check the config file for global parameters et set the variables accordingly 
void HttpRequest::checkGlobal( ConfigFile& config){
    this->path = config.get_root() + this->path;
    this->autorizedMethods = config.get_methods();
    this->index = config.get_index();

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
}


void HttpRequest::parseMultipartFormData() {
    std::string delimiter = "--" + this->boundary;
    std::string endDelimiter = delimiter + "--";

    std::size_t startPos = 0;
    std::size_t endPos = 0;

    while ((startPos = this->body.find(delimiter, endPos)) != std::string::npos) {
        startPos += delimiter.length();
        endPos = this->body.find(delimiter, startPos);
        std::string section = this->body.substr(startPos, endPos - startPos);

        std::size_t filenamePos = section.find("filename=\"");
        if (filenamePos == std::string::npos)
            continue;

        filenamePos += 10; // Move to the starting position of the filename
        std::size_t filenameEndPos = section.find("\"", filenamePos);
        std::string filename = section.substr(filenamePos, filenameEndPos - filenamePos);

        std::size_t contentPos = section.find("\r\n\r\n");
        if (contentPos == std::string::npos)
            continue;

        std::string content = section.substr(contentPos + 4);

        this->multiBody[filename] = content;
    }
}

void  HttpRequest::checkServerName(ConfigFile& config){
    std::string path = config.get_path();
    this->isValid = true;
    int nbS = find_nb_of_server(path);
	this->isValid = true;
     if(nbS > 1){
        for(int i = 0; i< nbS; i++){
            this->config.set_config(path, i);
            for (std::vector<std::string>::iterator it = this->config.get_listen().begin(); it != this->config.get_listen().end(); ++it) {
                if(this->config.get_server_name()+ ":" + *it == this->serverName) {
                 return ;
                }
            }
        }
     }
this->isValid = false;
}

HttpRequest::~HttpRequest() { return; }
