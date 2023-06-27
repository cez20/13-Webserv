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
	This function launches the server so that it is always running 




 */
int launchServer(const ServerConfiguration& config) 
{
	(void)config; 			// To be removed
	int status;
	struct addrinfo hints;	// struct needed by getaddrinfo()
	struct addrinfo *res = NULL;      // struct needed by getaddrinfo()

	memset(&hints, 0, sizeof(hints));   // sets up all info inside struct to value "0" to be ready to be filled out 
	hints.ai_family = AF_INET;     	    // AF_UNSPEC = Either IPV4 or IPV6. Other flags possible are AF_INET (Ipv4 or AF_INET6 (Ipv6
	hints.ai_socktype = SOCK_STREAM;	// Define socket type as streams (SOCK_STREAM). Specific to HTTP and TCP/IP 
	hints.ai_flags = AI_PASSIVE;		// Flag to assign the address of my localhost (127.0.0.1) to the socket structure.  
	hints.ai_protocol = IPPROTO_TCP;    // Newly added, but can be left to "0" to accept any protocols 
	
	//We can replace the first arg with "localhost" or NULL. Will give the same result.
	// We can replace second argument with PORT we use. All PORT under 1024 will work, but not later in process because protected
	// The servinfo is a pointer to a linked list containing all the IP address associated with host. Multiple IP can be available to deal with load balancing, 
	if ((status = getaddrinfo("localhost", "8080", &hints, &res)) != 0) {
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		return 1;							
	}

    //freeaddrinfo(res); // free the linked list of IP addresse associated with host! Probably needs to be done at the end of program 

	int serverSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol); // hints.ai_protocol is equal to 0.  
	if (serverSocket == -1) {
		std::cerr << "Error while creating the server socket." << std::endl;
		return 1;
	}

	//We bind the serverSocket with PORT NUMBER    WATCH IF WE MUST USE HTONS????
	if (bind(serverSocket, res->ai_addr, res->ai_addrlen) == -1) {
		std::cerr << "Error while trying to bind the socket." << std::endl;
		close(serverSocket);
		return 1;
	}

	//Then, server will be listening for incoming connection to the server IP address and port number
	// Request sent to 127.0.0.1/8080  (localhost address and PORT 8080). "5" represents the maximum connections
	// that can be in queue
	if (listen(serverSocket, 5) == -1) {
		std::cerr << "Error while trying to listen" << std::endl;
		close(serverSocket);
		return 1;
	}
	std::cout << "SOCKETSERVER VOUS ECOUTE." << std::endl;

	//SERVER IS STARTED

	// Client tries to connect to the IP address of server, but all connection remains in queued, until manually accepted by program
	// accept creates a new socket fd from the server socket. In other words, server socket remain always available to listen to incoming connections
	// Maybe use fcntl for non-blocking or  select() instead of accept 
	while (true) // Infinite loop 
	{
		struct sockaddr_storage clientAddress;  // Struct that will be used to stored client info (incoming connection info)
		socklen_t clientAddressLength;
		int clientSocket;

		clientAddressLength = sizeof(clientAddress);
		clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength); // If I do a connection on PORT 8081, it still indicates a connection. 
		//Connection is blocked until an attempt to connect to localhost (even with wrong PORT)is done.  
		if (clientSocket == -1) {
			std::cerr << RED_CL << "Error while receiving connection " << DEFAULT_CL << std::endl;
			close(serverSocket);
			return 1;
		}
		std::cout << "New connection accepted." << std::endl;

		char *msg = (char *)"Beej was here!";
		int len, bytes_sent;

		len = strlen(msg);
		bytes_sent = send(clientSocket, msg, len, 0);
		std::cout << "The number of bytes send is: " << bytes_sent << std::endl;

	


	}
    close(serverSocket);
    return 0;
}