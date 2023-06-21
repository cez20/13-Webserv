
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
    if (CgiPath + "test.php" ==  clientRequest.path) {
        if (clientRequest.headers.find("Content-Type") != clientRequest.headers.end() && clientRequest.headers.at("Content-Type") == "application/x-www-form-urlencoded"){
            try{
                std::string output  = executeCgi(clientRequest);
                analyseCgiOutput(output);
                return(0);
            }
            catch (const std::exception& e) {
                return(1);
            }
        }
    }
    // else if (clientRequest.method == "POST"){
        
    // }
    // else if (clientRequest.method == "DELETE"){
        
    // }
    else{
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

std::string HttpResponse::executeCgi(const HttpRequest& clientRequest) {
    std::string output;
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        throw std::runtime_error("Erreur lors de la création du tube (pipe).");
    }

    pid_t pid = fork();
    if (pid == -1) {
        throw std::runtime_error("Erreur lors de la création du processus fils.");
    } 
    else if (pid == 0) {
        // Processus fils
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        std::string arguments = clientRequest.querryString;
        std::stringstream ss(arguments);
        std::string argument;
        std::vector<std::string> argumentList;
        while (std::getline(ss, argument, '&')) {
            argumentList.push_back(argument);
        }
        std::vector<char*> argvList;
        argvList.push_back(const_cast<char*>(clientRequest.path.c_str()));
        for (size_t i = 0; i < argumentList.size(); ++i) {
            argvList.push_back(const_cast<char*>(argumentList[i].c_str()));
        }
        argvList.push_back(nullptr);    
        char** argv = argvList.data();
        char* envp[] = {nullptr};
        execve("/usr/bin/php-cgi", argv, envp);
    }
    else{
        // Processus parent
        close(pipefd[1]);
        char buffer[128];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            output += std::string(buffer, bytesRead);
        }

        close(pipefd[0]);

        int status;
        waitpid(pid, &status, 0);

        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            throw std::runtime_error("Le script CGI a retourné une erreur.");
        }

       
    }
    return output;
}
void HttpResponse::analyseCgiOutput(const std::string& output){
    std::size_t headerEnd = output.find("\r\n\r\n");
    if (headerEnd != std::string::npos) {
        std::string responseBody = output.substr(headerEnd + 4);
        this->body = (responseBody);
        return;
    }

    this->body = ("Error: Invalid CGI output format");

 }
    //We will fork, and execute de CGI script in a child process, redirect the output to a fd  and finally store it the variable CgiOut.
    


