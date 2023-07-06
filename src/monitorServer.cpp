#include "webserv.hpp"

/* 
	Function that receives request (recv) and send (send()) appropriate based
	on the client's request.
 */
void handleClient(int clientSocket, const ConfigFile& config) {
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

/* 
	Function that take new client socket created and add its into our vector of socketfds
	so that we can start monitoring it. 
*/
void addSocketToVector(std::vector<pollfd> *socketFds, int newClientSocket)
{
	pollfd newClientFd;

	newClientFd.fd = newClientSocket;
	newClientFd.events = POLLIN;
	socketFds->push_back(newClientFd);
}

/* 
	Server receives incoming connection that are put on a queue. Once socket is ready to receive
	request, he "accepts" it to allow client/server communication. 
*/
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

/* 
	We call the poll() function which essentially returns the number of client's socket that have the POLLIN
	action (ready to recv)
 */
void	launchSocketMonitoring(std::vector<pollfd> *socketFds, int *serverSocket)
{
	int nbrOfSocketsReady = 0;

	for (int i = 0; i < 3; i++)
	{
		nbrOfSocketsReady = poll(socketFds->data(), socketFds->size(), -1);    // -1  Means that the poll "Blocks" indefinitely, until a connection is accepted 
	}
	if (nbrOfSocketsReady == -1) {
		std::cerr << "Error while trying to call the poll fucntion." << std::endl;
		for (int i = 0; i < 3; i++)
			close(serverSocket[i]);
		exit(EXIT_FAILURE);
	}
	std::cout << "Started monitoring sockets with poll()" << std::endl;
}

/* 
	This function is the infinite loop, that listen for incoming connections. Function start
	by calling the launchSocketMonitoring to see which sockets are ready to receive content (POLLIN) 
	or to send content(POLLOUT).Once done, new client socket is created, and the dynamic of receiving 
	and sending information between client and server is started 
 */
int monitorServer(int *serverSocket, ConfigFile config)
{
	int size = config.get_listen().size();
	std::vector<pollfd> socketFds(size);
	// This loop creates all server sockets 
	for (int i = 0; i < size; i++)
	{
		socketFds[i].fd = serverSocket[i];
    	socketFds[i].events = POLLIN;
	}
	while (true)
	{
		launchSocketMonitoring(&socketFds, serverSocket);
		for (size_t i = 0; i < socketFds.size(); ++i) {
			if (socketFds[i].revents & POLLIN) {						 
				if (socketFds[i].fd == serverSocket[i]){		
					std::cout << "Server is ready to read" << std::endl;			 
					int newSocket = createNewClientSocket(serverSocket[i]);
					addSocketToVector(&socketFds, newSocket);			
				}
				else {
					std::cout << "Client " << i << " is ready to read";
					handleClient(socketFds[i].fd, config);
					std::cout << "Closing the connection" << std::endl;
					close(socketFds[i].fd); 
					socketFds.erase(socketFds.begin() + i);
					--i;
				}
			}
		}
	}
	for (int i = 0; i < size; i++)
	{
		close (serverSocket[i]);
	}
	return (0);
}