#include "webserv.hpp"

/* 
	Function that receives request (recv) and send (send()) appropriate based
	on the client's request.
 */
void handleClient(int clientSocket, const ConfigFile& config) {
    std::vector<char> buffer(BUFFER_MAX); 
std::string request;
while (true) {
    int bytesRead = recv(clientSocket, buffer.data(), buffer.size(), 0);
    if (bytesRead <= 0) {
        close(clientSocket);
        if (bytesRead == 0)
            std::cout << "\rConnection was closed by client.\n" << std::endl;
        return;
    }
    std::cout << "The number of bytes read is: " << bytesRead << std::endl;
    request.append(buffer.data(), bytesRead);
    if (request.find("\r\n\r\n") != std::string::npos) {
        break;
    }
    // Je devrai vérifier si le corps n'est pas fragmenté aussi. Je le ferai plus tard en utilisant Content-Length dans l'en-tête.
}
    HttpRequest clientRequest(request, config);
    //clientRequest.showRequest();
    //printMap(clientRequest.headers);
    HttpResponse response(clientRequest);
    response.writeOnSocket(clientSocket);
    //close(clientSocket);  // Temporary removed it , because already in infinte loop
}

void enter_access_log(sockaddr_storage clientAddress, ConfigFile config)
{
	char ip[INET_ADDRSTRLEN]; 
	struct sockaddr_in* ipv4 = (struct sockaddr_in*)&clientAddress;

	inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
	std::string msg = "Connection from: " + std::string(ip); 
	access_logs(msg, config);
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
int	createNewClientSocket(int serverSocket, ConfigFile config)
{
	sockaddr_storage clientAddress = {};				// Fills structure with 0s
    socklen_t clientAddressLength = sizeof(clientAddress);
    
	int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
	if (clientSocket == -1) {
		std::cerr << RED_CL << "Error while receiving connection " << DEFAULT_CL << std::endl;
		close(serverSocket);
		exit(EXIT_FAILURE);
	}
	enter_access_log(clientAddress, config);
	std::cout << "New connection accepted." << std::endl;
	return (clientSocket);
}

/* 
	We call the poll() function which essentially returns the number of client's socket that have the POLLIN
	action (ready to recv)
 */
void	launchSocketMonitoring(std::vector<pollfd> *socketFds, std::vector <int> serverSocket)
{
	int nbrOfSocketsReady = 0;

	nbrOfSocketsReady = poll(socketFds->data(), socketFds->size(), -1);    // -1  Means that the poll "Blocks" indefinitely, until a connection is accepted 
	std::cout << "The number of sockets ready is: " << nbrOfSocketsReady <<  std::endl;
	if (nbrOfSocketsReady == -1) {
		std::cerr << "Error while trying to call the poll function." << std::endl;
		for (size_t i = 0; i < serverSocket.size() ; ++i)
			close(serverSocket[i]);
		exit(EXIT_FAILURE);
	}
	std::cout << "Started monitoring sockets with poll()" << std::endl;
}

/* 
	This function creates a vector of type pollfd necessary for the poll() function. 
	It contains the file descriptor of each socket. 
*/
std::vector<pollfd> createSocketVector(std::vector<int> serverSocket)
{
	std::vector<pollfd> socketFds(serverSocket.size());

	for (size_t i = 0; i < serverSocket.size(); ++i)
	{
		socketFds[i].fd = serverSocket[i];
 		socketFds[i].events = POLLIN;
	}
	return (socketFds);
}

/* 
	This function is the infinite loop, that listen for incoming connections. Function start
	by calling the launchSocketMonitoring to see which sockets are ready to receive content (POLLIN) 
	or to send content(POLLOUT).Once done, new client socket is created, and the dynamic of receiving 
	and sending information between client and server is started 
 */
int monitorServer(std::vector<int> serverSocket, ConfigFile config)
{
	std::vector <pollfd> socketFds = createSocketVector(serverSocket);
	while (true)
	{
		launchSocketMonitoring(&socketFds, serverSocket);
		for (size_t i = 0; i < socketFds.size(); ++i) {
			if (socketFds[i].revents & POLLIN) {						 
				if (socketFds[i].fd == serverSocket[i]){		
					std::cout << "Server is ready to read" << std::endl;			 
					int newSocket = createNewClientSocket(serverSocket[i], config);
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
	// for (size_t i = 0; i < socketFds.size(); i++)
	// 	close (serverSocket[i]);
	return (0);
}