
#include "HttpResponse.hpp"

 volatile sig_atomic_t alarmReceived = 0;
 void alarmHandler(int) {
            alarmReceived = 1;
        }
HttpResponse::HttpResponse(const HttpRequest& clientRequest){
    generateStatusMap();

    this->path = clientRequest.path;
    if(clientRequest.reponseStatus != ""){
        this->statusCode = clientRequest.reponseStatus;
    }
    else
    {
       analyseRequest(clientRequest);
    checkForError(clientRequest); 
    }
    
}
int HttpResponse::analyseRequest(const HttpRequest& clientRequest){
    if (clientRequest.method != "POST" && clientRequest.method != "GET" && clientRequest.method != "DELETE"){
        this->statusCode = "501";    
        return (1);
    }
    if (!clientRequest.autorizedMethods.empty()) {
        std::vector<std::string>::const_iterator it = std::find(
            clientRequest.autorizedMethods.cbegin(), clientRequest.autorizedMethods.cend(), clientRequest.method);

        if (it == clientRequest.autorizedMethods.cend()) {
            this->statusCode = "405";
            return 1;
        } 
    }
    if(clientRequest.method == "POST" && clientRequest.max_body < clientRequest.contentLength && clientRequest.max_body > 0){
        this->statusCode = "413";
        return(1);
    }
    if(!clientRequest.redir.empty())
    {
            this->statusCode = "301 Moved Permanently";
            std::cout<< "redir var :  " << clientRequest.redir << std::endl;
            return (0);
    }
    
     else if(isDirectory(path)){
        if(clientRequest.method == "POST"  && !clientRequest.boundary.empty()){
            if(!clientRequest.upload){
                this->statusCode = "405";
                return 1;
            }
            else if(uploading(clientRequest.multiBody, clientRequest.path)== 0){   
            this->statusCode = "200 OK";
            this->headers["contentType"] = "text/html";
            this->body = "File(s) were successfully downloaded";
            std::cout << "PATH FOR UPLOADING :   " << path << std::endl;
            return (0);
            }

        }
        else if(!clientRequest.index.empty())
        {   if(isDirectory(this->path) && this->path.back() != '/')
                this->path= this->path + "/";
            this->path += clientRequest.index;
            
            return(responseForStatic(clientRequest));
        }
        else if (clientRequest.autoIndex){
            this->statusCode = "200 OK";
            autoListing();
            return(0);
        }
        else {
            this->statusCode = "403";
            return (1);
        }   
            
    }

   else if (clientRequest.method == "DELETE"){
        if (clientRequest.allow_delete){
            return(deleteMethod(clientRequest));
        }
        else{
            statusCode = "405";
            return 1;
        }
    }

    //check if the  path exist, if not, fill the HttpResponse with the error 404
    else if (!fileExist(clientRequest.path)){
        this->statusCode = "404";
        return (1);
    }
    else if (clientRequest.isCgi){
        std::string output;
        try{
            if(clientRequest.method == "GET"){
                output = executeCgiGet(clientRequest);
            }
            else{
                output = executeCgiPost(clientRequest);
            }
        }
        catch(const std::exception& e){
		std::cerr << e.what() << '\n';
        error_logs( e.what(), clientRequest.config);
        this->statusCode = "500";
        return (1);
	    }
        analyseCgiOutput(output);
        this->statusCode = "200 OK";
        return (0);

    } 
    else{
        return(responseForStatic(clientRequest));
    }
    return (0);
}

 //this fucntion will write on the socket the response to send to the client
int HttpResponse::writeOnSocket(const int& clientSocket){
    std::string response = "HTTP/1.1 " + statusCode + "\r\n";
    if (statusCode != "HTTP/1.1 204 No Content"){
        for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
            response += it->first + ": " + it->second + "\r\n";
        }
        response += "\r\n" + body;
    }
    size_t totalBytesSent =0;
    size_t bytesRemaining = response.length();
    
    std::cout << bytesRemaining << std::endl;   
    while(totalBytesSent < response.length()){
      int bytesSent=send(clientSocket, response.c_str(), response.length(), 0);
       if (bytesSent == -1) {
            std::cerr << "error while sending response to the client" << std::endl;
            return -1;
        }
        totalBytesSent += bytesSent;
        bytesRemaining -= bytesSent;
    }
    
    return (0);
}

bool HttpResponse::fileExist(const std::string& filename) {
    std::ifstream file(filename.c_str());
    return file.good();
}

std::string HttpResponse::executeCgiGet(const HttpRequest& clientRequest) {
    std::string output;
    int pipefd[2];
    char currentDir[PATH_MAX];
    std::string scriptPath(currentDir);
    


    if (getcwd(currentDir, sizeof(currentDir)) == nullptr) {
        std::cerr << "Failed to get current working directory" << std::endl;
    }

    if (pipe(pipefd) == -1) {
        throw std::runtime_error("Error when creating pipe");
    }

    pid_t pid = fork();
    if (pid == -1) {
        throw std::runtime_error("Error wheile forking");
    }
    else if (pid == 0) {

        signal(SIGALRM, alarmHandler);

        // Set the alarm for 4 seconds
        alarm(2);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        //separate variable from URL
        std::string arguments = clientRequest.queryString;
        std::stringstream ss(arguments);
        std::string environVariables;
        std::vector<std::string> argumentList;
        while (std::getline(ss, environVariables, '&')) {
            argumentList.push_back(environVariables);
        }
        //set args for execve
        std::vector<char*> argvList;
        argvList.push_back(const_cast<char*>("php"));  // Utilisez le chemin correct vers l'interpréteur PHP
        argvList.push_back(const_cast<char*>(path.c_str()));
        argvList.push_back(nullptr);
        //set envp for execve
        std::vector<char*> envpList;      
        for (size_t i = 0; i < argumentList.size(); ++i) {
            envpList.push_back(const_cast<char*>(argumentList[i].c_str()));  // Définit la variable d'environnement
        }
        envpList.push_back(nullptr);
        char* const* argv = argvList.data();
        char* const* envp = envpList.data();
         int result = chdir(clientRequest.cgiPass.c_str()); //change
        if (result != 0) {
            std::cerr << "Failed to change directory to " << clientRequest.cgiPass << std::endl;
            throw std::runtime_error("Failed to change the directory to the php directory");
        }

        if (execve("php", argv, envp) == -1) {
            std::cerr << "Error with execve" << std::endl;
        }
    }
    else {
        // Processus parent
        close(pipefd[1]);
        if (alarmReceived) {
            kill(pid, SIGTERM);
            alarmReceived = 0;
            throw std::runtime_error("Timeout: CGI script took too long to execute.");
        }
        int status;
        waitpid(pid, &status, 0);

        
        char buffer[5000];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            output += std::string(buffer, bytesRead);
        }
        close(pipefd[0]);

         if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
             throw std::runtime_error("Script returned an error");
         }
        return output;
    }

     return output;
}
std::string HttpResponse::executeCgiPost(const HttpRequest& clientRequest) {
    std::string output;
    char currentDir[PATH_MAX];
    std::string scriptPath(currentDir);
    

    if (getcwd(currentDir, sizeof(currentDir)) == nullptr) {
        std::cerr << "Failed to get current working directory" << std::endl;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        throw std::runtime_error("Error when creating pipe");
    }

    pid_t pid = fork();
    if (pid == -1) {
        throw std::runtime_error("Error while forking");
    }
    else if (pid == 0) {
        signal(SIGALRM, alarmHandler);
        alarm(2);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        //separate variable from URL
        std::string arguments = clientRequest.body;
        std::stringstream ss(arguments);
        std::string environVariables;
        std::vector<std::string> argumentList;
        while (std::getline(ss, environVariables, '&')) {
            argumentList.push_back(environVariables);
        }
        //set args for execve
        std::vector<char*> argvList;
        argvList.push_back(const_cast<char*>("php"));  // Utilisez le chemin correct vers l'interpréteur PHP
        argvList.push_back(const_cast<char*>(path.c_str()));
        argvList.push_back(nullptr);
        //set envp for execve
        std::vector<char*> envpList;      
        for (size_t i = 0; i < argumentList.size(); ++i) {
            envpList.push_back(const_cast<char*>(argumentList[i].c_str()));
        }
        envpList.push_back(nullptr);
        char* const* argv = argvList.data();
        char* const* envp = envpList.data();
        int result = chdir(clientRequest.cgiPass.c_str()); //change
        if (result != 0) {
            std::cerr << "Failed to change directory to " << clientRequest.cgiPass << std::endl;
            throw std::runtime_error("Failed to change the directory to the php directory");
        }
        if (execve("php", argv, envp) == -1) {
            std::cerr << "Error with execve" << std::endl;
            throw std::runtime_error("Script returned an error");
        }
    }
    else {
        close(pipefd[1]);
        if (alarmReceived) {
            kill(pid, SIGTERM);
            alarmReceived = 0;
            throw std::runtime_error("Timeout: CGI script took too long to execute.");
        }

        int status;
        waitpid(pid, &status, 0);
        
        
        char buffer[5000];
        ssize_t bytesRead;
        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            output += std::string(buffer, bytesRead);
        }
        close(pipefd[0]);

         if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
             throw std::runtime_error("Script returned an error");
         }
    }
    int result = chdir(currentDir);
        if (result != 0) {
            std::cerr << "Failed to change directory to " << "/usr/bin/php" << std::endl;
        }
     return output;
}

void HttpResponse::analyseCgiOutput(const std::string& output){
        this->body = (output);
        //this->headers["contentDispositon"] = "inline";
        this->headers["contentLength"] = std::to_string(this->body.length());
        return;
    }

int HttpResponse::responseForStatic(const HttpRequest& clientRequest){
   if (clientRequest.toBeDownloaded) {
        std::string filePath = this->path;  // Chemin du fichier à télécharger
        std::ifstream fileStream(filePath, std::ios::binary);
        std::string fileName = filePath.substr(filePath.find_last_of('/') + 1);
        std::cout << "to be downloaded!!!" << std::endl;
         std::cout << fileName << std::endl;
        // Définir les en-têtes de la réponse
        this->headers["contentDisposition"] = "attachment; filename=\"" + fileName + "\"";
        this->headers["contentType"] = "application/octet-stream"; // Type MIME pour les fichiers téléchargeables
        this->headers["Cache-Control"] = "no-cache";  

        // Lire le contenu du fichier et l'affecter à la réponse
        std::stringstream fileContent;
        fileContent << fileStream.rdbuf();
        this->body = fileContent.str();

        // Définir la longueur du contenu
        this->headers["contentLength"] = std::to_string(this->body.length());
         this->statusCode = "200 OK";
        return 0;
    }
    else{
        this->statusCode = "200 OK";
        this->headers["contentType"] = "text/html";
        this->body = extractFileContent(this->path);
        this->headers["contentLength"] = std::to_string(this->body.length());
    } 
        return (0);

}


int HttpResponse::deleteMethod(const HttpRequest& clientRequest){
   const char* file_path = clientRequest.path.c_str();
   int result = std::remove(file_path);

    if (result != 0) {
        // La suppression a échoué.
        error_logs("Erreur lors de la suppression du fichier", clientRequest.config);
        std::perror("Erreur lors de la suppression du fichier");
        this->statusCode = "500";
        return 1;
    } else {
        // La suppression a réussi.
        std::cout << "Fichier supprimé avec succès !" << std::endl;
        this->statusCode = "200 OK";
        this->headers["contentType"] = "text/html";
        this->body = "DELETE request handled successfully.";
        return(0);

    }
    
    
  
}
void HttpResponse::checkForError(const HttpRequest& clientRequest){
    if(this->statusCode != "200 OK" && this->statusCode != "301 Moved Permanently"){
       if(!checkForCustomErrorFiles(clientRequest))
            generateDefaultError();
        error_logs("Error Http : " + this->statusCode, clientRequest.config);
    }
    
}
int HttpResponse::checkForCustomErrorFiles(const HttpRequest& clientRequest){
    std::map<std::string, std::string>errorMap = clientRequest.config.get_error_page();
    std::string root= clientRequest.config.get_root();
    for(std::map<std::string, std::string>::iterator it = errorMap.begin(); it!= errorMap.end(); it++){
        if (this->statusCode == it->first && !it->second.empty()){
            this->body = extractFileContent(root + it->second);
            return (1);
        }
    } 
    return 0;
}
void HttpResponse::generateStatusMap(){
    httpStatusMap["100"] = "Continue";
    httpStatusMap["101"] = "Switching Protocols";
    httpStatusMap["200"] = "OK";
    httpStatusMap["201"] = "Created";
    httpStatusMap["202"] = "Accepted";
    httpStatusMap["203"] = "Non-Authoritative Information";
    httpStatusMap["204"] = "No Content";
    httpStatusMap["205"] = "Reset Content";
    httpStatusMap["206"] = "Partial Content";
    httpStatusMap["300"] = "Multiple Choices";
    httpStatusMap["301"] = "Moved Permanently";
    httpStatusMap["302"] = "Found";
    httpStatusMap["303"] = "See Other";
    httpStatusMap["304"] = "Not Modified";
    httpStatusMap["305"] = "Use Proxy";
    httpStatusMap["307"] = "Temporary Redirect";
    httpStatusMap["400"] = "Bad Request";
    httpStatusMap["401"] = "Unauthorized";
    httpStatusMap["402"] = "Payment Required";
    httpStatusMap["403"] = "Forbidden";
    httpStatusMap["404"] = "Not Found";
    httpStatusMap["405"] = "Method Not Allowed";
    httpStatusMap["413"] = "Request Entity Too Large";
    httpStatusMap["500"] = "Internal Server Error";
    httpStatusMap["501"] = "Not Implemented";
    
}
void HttpResponse::generateDefaultError(){

    std::string errorPage;


        errorPage += "<html><head><title>Error " + statusCode + " : " + httpStatusMap[statusCode] + "</title></head>";
        errorPage += "<body><h1>Error " + statusCode  + " : " + httpStatusMap[statusCode] + "</h1>";
        errorPage += "<p>Sorry, the requested page could not be found.</p>";
        errorPage += "</body></html>";


        this->headers["contentType"] = " text/html";
        this->body = errorPage;
}

void HttpResponse::generateDirListing(std::vector<std::string> vecList){
    std::string html;

    html += "<html><body><ul>";

    for (std::vector<std::string>::const_iterator it = vecList.begin(); it != vecList.end(); ++it) {
        html += "<li><a href=\"";
        html += *it;
        html += "\">";
        html += *it;
        html += "</a></li>";
    }

    html += "</ul></body></html>";
    this->body= html;
    this->statusCode = "200 OK";
    this->headers["contentType"] = "text/html";
    this->headers["contentLength"] = std::to_string(this->body.length());
        
    }
void HttpResponse::autoListing(){
    std::vector<std::string> vecList;

    DIR* dir = opendir(path.c_str());
    if (dir != NULL) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            std::string name = entry->d_name;
            vecList.push_back(name);
        }
        closedir(dir);
    }
    generateDirListing(vecList);
}

int HttpResponse::uploading(const std::map<std::string, std::string>& multiBody, const std::string& path) {
    for (std::map<std::string, std::string>::const_iterator it = multiBody.begin(); it != multiBody.end(); ++it) {
        const std::string& filename = it->first;
        const std::string& content = it->second;
        std::cout << "filename =    " << path + filename <<std::endl;
        std::ofstream file(path + filename, std::ios::out | std::ios::binary);
        if (file) {
            file.write(content.data(), content.size());
            file.close();
            std::cout << "File uploaded and saved: " << filename << std::endl;
        } else {
            std::cerr << "Error opening file: " << filename << std::endl;
            this->statusCode = "500";
            return 1;
        }
    }
    return 0;
}




    




