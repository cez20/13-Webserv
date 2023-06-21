
#include "../inc/HttpResponse.hpp"
// constructor
HttpResponse::HttpResponse(const HttpRequest& clientRequest){
    analyseRequest(clientRequest);
}

int HttpResponse::analyseRequest(const HttpRequest& clientRequest){
    if (!fileExist(clientRequest.path)){
        this->statusCode = "404";
        this->body=extractFileContent("/Users/slord/Desktop/13-WEBSERVER/html/404.html");
        this->headers["contentType"] = "text/html";
        return (1);
    }
    else if (clientRequest.method == "GET"){
        this->statusCode = "200";
        this->body = extractFileContent(clientRequest.path);
        this->headers["contentType"] = "text/html";
        this->headers["contentLength"] = std::to_string(this->body.length());
        return (0);

    }
    std::string CgiPath = clientRequest.config.getCgiRoot();
    if (clientRequest.path.substr(0, CgiPath.length()) == CgiPath) {
        if (clientRequest.headers.find("Content-Type") != clientRequest.headers.end() && clientRequest.headers.at("Content-Type") == "application/json"){
            executeCgi(clientRequest.path, clientRequest.body);
    }
        else{
            this->statusCode = "501";
            this->headers["contentType"] = " text/html";
            this->body = extractFileContent("/Users/slord/Desktop/13-WEBSERVER/html/501.html");       
            return (1);
        }

    }
    // else if (clientRequest.method == "POST"){
        
    // }
    // else if (clientRequest.method == "DELETE"){
        
    // }
    else
    {
        this->statusCode = "501";
        this->headers["contentType"] = " text/html";
        this->body = extractFileContent("/Users/slord/Desktop/13-WEBSERVER/html/501.html");       
        return (1);
    }
    return (0);
    
}

 //this fucntion will write on the socket the response to send to the client
int HttpResponse::writeOnSocket(const int& clientSocket){
    std::string response = "HTTP/1.1 " + statusCode + "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
        response += it->first + ": " + it->second + "\r\n";
    }
    response += "\r\n" + body;
    send(clientSocket, response.c_str(), response.length(), 0);
    return (0);
}

bool HttpResponse::fileExist(const std::string& filename) {
    std::ifstream file(filename.c_str());
    return file.good();
}
void executeCgi(const std::string& cgiPath, const std::string env)  {
    //We will fork, and execute de CGI script in a child process, redirect the output to a fd  and finally store it the variable CgiOut.
    
}

