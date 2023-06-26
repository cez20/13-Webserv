
#include "../inc/HttpResponse.hpp"
// constructor
HttpResponse::HttpResponse(const HttpRequest& clientRequest){
    analyseRequest(clientRequest);
}

int HttpResponse::analyseRequest(const HttpRequest& clientRequest){
    if (!fileExist(clientRequest.path)){
        this->statusCode = "404";
        this->body=extractFileContent(clientRequest.config.getDocumentRoot() + "/404.html");
        this->headers["contentType"] = "text/html";
        return (1);
    }
    else if (clientRequest.method == "GET"){
        this->statusCode = "200";
        if (clientRequest.isCgi) {
            std::string output  = executeCgi(clientRequest);
            analyseCgiOutput(output);
            return(0);
        }
        else if (clientRequest.toBeDownloaded) {
            std::string filePath = clientRequest.path;  // Chemin du fichier à télécharger
            std::ifstream fileStream(filePath, std::ios::binary);
            std::string fileName = filePath.substr(filePath.find_last_of('/') + 1);

            // Définir les en-têtes de la réponse
            this->headers["contentDisposition"] = "attachment; filename=\"" + fileName + "\"";

            this->headers["contentType"] = "application/octet-stream";  // Type MIME pour les fichiers téléchargeables

            // Lire le contenu du fichier et l'affecter à la réponse
            std::stringstream fileContent;
            fileContent << fileStream.rdbuf();
            this->body = fileContent.str();

            // Définir la longueur du contenu
            this->headers["contentLength"] = std::to_string(this->body.length());
            return 0;
        }
        else{
            this->headers["contentType"] = "text/html";
            this->body = extractFileContent(clientRequest.path);
            this->headers["contentLength"] = std::to_string(this->body.length());
        } 
        return (0);
    }
    
    // else if (clientRequest.method == "POST"){
        
    // }
    // else if (clientRequest.method == "DELETE"){
        
    // }
    else{
        this->statusCode = "501";
        this->headers["contentType"] = " text/html";
        this->body=extractFileContent(clientRequest.config.getDocumentRoot() + "/501.html");      
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
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        //separate variable from URL
        std::string arguments = clientRequest.querryString;
        std::stringstream ss(arguments);
        std::string argument;
        std::vector<std::string> argumentList;
        while (std::getline(ss, argument, '&')) {
            argumentList.push_back(argument);
        }
        //set args for execve
        std::vector<char*> argvList;
        argvList.push_back(const_cast<char*>("/usr/bin/php"));  // Utilisez le chemin correct vers l'interpréteur PHP
        argvList.push_back(const_cast<char*>("/Users/slord/Desktop/13-WEBSERVER/html/test.php"));
        argvList.push_back(nullptr);
        //set envp for execve
        std::vector<char*> envpList;      
        for (size_t i = 0; i < argumentList.size(); ++i) {
            envpList.push_back(const_cast<char*>(argumentList[i].c_str()));  // Définit la variable d'environnement NAME avec la valeur "John"
        }
        envpList.push_back(nullptr);
        char* const* argv = argvList.data();
        char* const* envp = envpList.data();
        if (execve("/usr/bin/php", argv, envp) == -1) {
            std::cerr << "Erreur lors de l'exécution de execve: " << strerror(errno) << std::endl;
            std::cerr << "Problème avec execve" << std::endl;
        }
    }
    else {
        // Processus parent
        close(pipefd[1]);

        int status;
        waitpid(pid, &status, 0);

        char buffer[5000];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            output += std::string(buffer, bytesRead);
        }
        close(pipefd[0]);

         if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
             throw std::runtime_error("Le script CGI a retourné une erreur.");
         }
        return output;
    }

     return output;
}

void HttpResponse::analyseCgiOutput(const std::string& output){
        this->body = (output);
        //this->headers["contentDispositon"] = "inline";
        this->headers["contentLength"] = std::to_string(this->body.length());
        return;
    }

    

    //We will fork, and execute de CGI script in a child process, redirect the output to a fd  and finally store it the variable CgiOut.
    


