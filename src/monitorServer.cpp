#include "webserv.hpp"

//parse and analyse the resquest on the client socket. After send the appropriate response to the client.

void handleClient(int clientSocket, const ServerConfiguration& config) {
    char buffer[8192];
    std::string request;
    while (true) {
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            // Erreur de lecture ou fin de la connexion
            close(clientSocket);
            if (bytesRead == 0)
                std::cout << "\rConnection was closed by client.\n" << std::endl;
            return;
        }
		std::cout << "The number of bytes read is: " << bytesRead << std::endl;
        request.append(buffer, bytesRead);
        // check is the request header is complete
        if (request.find("\r\n\r\n") != std::string::npos) {
            break;
        }
        //I will have to check if the body is not fragmented too.  Will do later, I will havwe to use conent-length in the header
    }
    HttpRequest clientRequest(request, config);
    //clientRequest.showRequest();
    //printMap(clientRequest.headers);
    HttpResponse response(clientRequest);
    response.writeOnSocket(clientSocket);
    close(clientSocket);
}

void addSocketToVector(std::vector<pollfd> *socketFds, int newClientSocket)
{
	pollfd newClientFd;

	newClientFd.fd = newClientSocket;
	newClientFd.events = POLLIN;
	socketFds->push_back(newClientFd);
}

void	launchSocketMonitoring(std::vector <pollfd> *socketFds, int serverSocket)
{
	int nbrOfSocketsReady = poll(socketFds->data(), socketFds->size(), -1);    // -1  Means that the poll "Blocks" indefinitely, until a connection is accepted 
	if (nbrOfSocketsReady == -1) {
		std::cerr << "Error while trying to call the poll fucntion." << std::endl;
		close(serverSocket);
		exit(EXIT_FAILURE);
	}
	std::cout << "Started monitoring sockets with poll()" << std::endl;
}

int	createNewClientSocket(int serverSocket)
{
	sockaddr_storage clientAddress = {};				// Fills structure with 0s
    socklen_t clientAddressLength = sizeof(clientAddress);
    
	int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
	if (clientSocket == -1) {
		std::cerr << RED_CL << "Error while receiving connection " << DEFAULT_CL << std::endl;
		close(serverSocket);
		exit(EXIT_FAILURE);
	}
	std::cout << "New connection accepted." << std::endl;
	return (clientSocket);
}

int monitorServer(int serverSocket, ServerConfiguration config)
{
	std::vector<pollfd> socketFds(1);
    socketFds[0].fd = serverSocket;
    socketFds[0].events = POLLIN;
	
	while (true)
	{
		launchSocketMonitoring(&socketFds, serverSocket);
		for (size_t i = 0; i < socketFds.size(); ++i) {
			if (socketFds[i].revents & POLLIN) {						 
				if (socketFds[i].fd == serverSocket){					 
					int newSocket = createNewClientSocket(serverSocket);
					addSocketToVector(&socketFds, newSocket);			
				}
				else {
					handleClient(socketFds[i].fd, config);
					std::cout << "Closing the connection" << std::endl;
					close(socketFds[i].fd); 
					socketFds.erase(socketFds.begin() + i);
					--i;
				}
			}
		}
	}
	close (serverSocket);
	return (0);
}