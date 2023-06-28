#include "webserv.hpp"

#define MAX_PENDING_CONNECTIONS	5

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
        request.append(buffer, bytesRead);
        // check is the reques header is complete
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
	This function prints out all the information inside the struct addrinfo. Essentially,
	IP address type(Ipv4, Ipv6), socket type (stream or datagram), protocol (TCP, UDP,etc)
	length of socket Ip address, etc.
*/
void	printNetworkInfo(struct addrinfo *res)
{
	struct addrinfo *p;
	int i = 1;

	for (p = res; p != NULL; p = p->ai_next) {

	
		struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
        struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;

		std::cout << "\nPRINTING OUT IP ADDRESS [" << i << "] RELATED TO PROVIDED DOMAIN" << std::endl;
		std::cout << "ai_flags is: " << p->ai_flags << std::endl;  // 
		std::cout << "ai_family is: " << p->ai_family << std::endl;  // 2 = AF_INET (IPV4)
		std::cout << "Socket type is: " << p->ai_socktype << std::endl; // 1 = SOCK_STREAM
		std::cout << "Number associated to protocol is: " << p->ai_protocol << std::endl; // 6 = TCP 
		std::cout << "Length of socket address in bytes is: " << p->ai_addrlen << std::endl; // 16 is the addr_len 
		std::cout << "Address of pointer to a struct sockaddr is: " << p->ai_addr << std::endl;
		std::cout << "Address of next element in linked list is: " << p->ai_next << std::endl;

		if (p->ai_family == AF_INET) 
		{
			char ip[16]; // Maximum lenght of IP address including dot and /0 of the end.  
			inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
			printf("IPv4 Address: %s\n", ip);
			printf("Port: %d\n", ntohs(ipv4->sin_port));
		}else if (p->ai_family == AF_INET6) { // IPv6
			char ip[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip, INET6_ADDRSTRLEN);
			printf("IPv6 Address: %s\n", ip);
			printf("Port: %d\n", ntohs(ipv6->sin6_port));
		}
		i++;
	}
}

/* This function creates a socket and binds it with a PORT number (ex:8080). It then
   launches the server and therefore officially puts it in a LISTENING mode that
   wait for requests/connection to the server IP and PORT.
*/
int serverSocketSetup(struct addrinfo *res)
{
	int serverSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (serverSocket == -1) {
		std::cerr << "Error while creating the server socket." << std::endl;
		return 1;
	}
	if (bind(serverSocket, res->ai_addr, res->ai_addrlen) == -1) {
		std::cerr << "Error while trying to bind the socket." << std::endl;
		close(serverSocket);
		return 1;
	}
	if (listen(serverSocket, MAX_PENDING_CONNECTIONS) == -1) {
		std::cerr << "Error while trying to listen" << std::endl;
		close(serverSocket);
		return 1;
	}
	std::cout << "SERVERSOCKET IS SETUP AND IS LISTENING TO INCOMING CONNECTIONS!" << std::endl;
	return (serverSocket);
}

/* 
	This function gets all the network information related to a specific domain name (ex: localhost) with
	the help of getaddrinfo() function. "Hints" struct is filled out with information (directives) related
	to the type of sockets we which for our server. We are specifying here that we wish a stream socket using
	the TCP/IP protocol, etc. When successful, getaddrinfo() returns a pointer to a linked list of struct addrinfo
	which each contain an IP address and PORT that can be used for the creation of sockets. 
 */
struct addrinfo 	*getNetworkInfo()
{
	struct addrinfo hints;
	struct addrinfo *res;
	int				status;
	
	memset(&hints, 0, sizeof(hints));		// sets up all info inside struct to value "0" to be ready to be filled out 
	hints.ai_family 	= AF_UNSPEC;			// AF_UNSPEC = Either IPV4 or IPV6. Other flags possible are AF_INET (Ipv4 or AF_INET6 (Ipv6
	hints.ai_socktype 	= SOCK_STREAM;		// Define socket type as streams (SOCK_STREAM). Specific to HTTP and TCP/IP 
	hints.ai_flags 		= AI_PASSIVE;		// Flag to assign the address of my localhost (127.0.0.1) to the socket structure.  
	hints.ai_protocol 	= IPPROTO_TCP;		// Newly added, but can be left to "0" to accept any protocols 

	if ((status = getaddrinfo("localhost", "8080", &hints, &res)) != 0) {
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		exit(EXIT_FAILURE);							
	}
	//printNetworkInfo(res);
	return (res);
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

void addSocketToArray(std::vector<pollfd> *socketFds, int newClientSocket)
{
	pollfd newClientFd;

	newClientFd.fd = newClientSocket;
	newClientFd.events = POLLIN;
	socketFds->push_back(newClientFd);
}

int launchServer(const ServerConfiguration& config) 
{
	(void)config;
	struct addrinfo *ipAddressList;

	ipAddressList = getNetworkInfo();
	int serverSocket = serverSocketSetup(ipAddressList);
	freeaddrinfo(ipAddressList);

	std::vector<pollfd> socketFds(1);
    socketFds[0].fd = serverSocket;
    socketFds[0].events = POLLIN;

	while (true)
	{
		int nbrOfSocketsReady = poll(socketFds.data(), socketFds.size(), -1);    // -1  Means that the poll "Blocks" indefinitely, until a connection is accepted 
		if (nbrOfSocketsReady == -1) {
			std::cerr << "Error while trying to call the poll fucntion." << std::endl;
			close(serverSocket);
			return 1;
		}
		for (size_t i = 0; i < socketFds.size(); ++i) {
             if (socketFds[i].revents & POLLIN) {						 // If one socket is ready to read. 
                if (socketFds[i].fd == serverSocket){					 // If the socket is the server socket and is ready to read (connection is pending), create a new socket.
                   int newSocket = createNewClientSocket(serverSocket);
				   addSocketToArray(&socketFds, newSocket);			
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

// std::vector<pollfd> pollFds(1);
//     pollFds[0].fd = serverSocket;
//     pollFds[0].events = POLLIN;

// 	   while (true) {
//         int ready = poll(pollFds.data(), pollFds.size(), -1);
//         if (ready == -1) {
//             std::cerr << "Error while trying to call the poll fucntion." << std::endl;
//             close(serverSocket);
//             return 1;
//         }
//         for (size_t i = 0; i < pollFds.size(); ++i) {
//             if (pollFds[i].revents & POLLIN) {
//                 if (pollFds[i].fd == serverSocket) {
//                     sockaddr_in clientAddress = {};
//                     socklen_t clientAddressLength = sizeof(clientAddress);
//                     int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
//                     if (clientSocket == -1) {
//                         std::cerr << RED_CL << "Error while receiving connection " << DEFAULT_CL << std::endl;
//                         close(serverSocket);
//                         return 1;
//                     }
//                     std::cout << "New connection accepted." << std::endl;
//                     pollfd newClientFd;
//                     newClientFd.fd = clientSocket;
//                     newClientFd.events = POLLIN;
//                     pollFds.push_back(newClientFd);
//                 }
//                 else {
//                     handleClient(pollFds[i].fd, config);
//                     std::cout << "Closing the connection" << std::endl;
//                     close(pollFds[i].fd); 
//                     pollFds.erase(pollFds.begin() + i);
//                     --i;
//                 }
//             }
//         }
//     }
// 	close (serverSocket);