#include "../inc/webserv.hpp"

//parse and analyse the resquest on the client socket. After send the appropriate response to the client.

void handleClient(int clientSocket) {
    char buffer[8192];
    int bytesRead = read(clientSocket, buffer, sizeof(buffer));
    if (bytesRead == -1) {
        std::cerr << "Cannot read the client data" << std::endl;
        close(clientSocket);
        return;
    }
    std::string request(buffer, bytesRead);
    HttpRequest clientRequest(request);
    //clientRequest.showRequest();
    HttpResponse response(clientRequest);
    response.writeOnSocket(clientSocket);
    close(clientSocket);
}


//main function. creating the server socket: using a poll to track if we can read the sockets
int server(ServerConfiguration config) {

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error while creating the server socket." << std::endl;
        return 1;
    }

    std::vector<pollfd> pollFds(1);
    pollFds[0].fd = serverSocket;
    pollFds[0].events = POLLIN;
 
    sockaddr_in serverAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(config.getPort()); 
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Errort while trying to bind trhe socket." << std::endl;
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error while trying to listen" << std::endl;
        close(serverSocket);
        return 1;
    }
    std::cout << "SOCKETSERVER VOUS ECOUTE." << std::endl;
    while (true) {
        int ready = poll(pollFds.data(), pollFds.size(), -1);
        if (ready == -1) {
            std::cerr << "Error while trying to call the poll fucntion." << std::endl;
            close(serverSocket);
            return 1;
        }
        for (size_t i = 0; i < pollFds.size(); ++i) {
            if (pollFds[i].revents & POLLIN) {
                if (pollFds[i].fd == serverSocket) {
                    sockaddr_in clientAddress = {};
                    socklen_t clientAddressLength = sizeof(clientAddress);
                    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
                    if (clientSocket == -1) {
                        std::cerr << RED_CL << "Error while receiving connection " << DEFAULT_CL << std::endl;
                        close(serverSocket);
                        return 1;
                    }
                    std::cout << "New connection accepted." << std::endl;
                    pollfd newClientFd;
                    newClientFd.fd = clientSocket;
                    newClientFd.events = POLLIN;
                    pollFds.push_back(newClientFd);
                }
                else {
                    handleClient(pollFds[i].fd);
                    std::cout << "Closing the connection" << std::endl;
                    close(pollFds[i].fd);
                    pollFds.erase(pollFds.begin() + i);
                    --i;
                }
            }
        }
     
    }
    close(serverSocket);
    return 0;
}