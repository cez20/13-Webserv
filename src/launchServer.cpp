#include "webserv.hpp"

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
	if (!res)
		return (-1);
	
	int serverSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (serverSocket == -1) {
		std::cerr << "Socket creation error: " << strerror(errno) << std::endl;  // ERROR LOG 
		return (-1);
	}

	int yes= 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		std::cerr << "Set socket option error: " << strerror(errno) << std::endl;  // ERROR LOG 
		return (-1);
	}

	if (bind(serverSocket, res->ai_addr, res->ai_addrlen) == -1) {
		std::cerr << "Bind error: " << strerror(errno) << std::endl;    // ERROR LOG 
		close(serverSocket);
		return (-1);
	}

	if (listen(serverSocket, MAX_PENDING_CONNECTIONS) == -1) {
		std::cerr << "Listen error: " << strerror(errno) << std::endl;  // ERROR LOG
		close(serverSocket);
		return (-1);
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
struct addrinfo 	*getNetworkInfo(const char *port)
{
	struct addrinfo hints;
	struct addrinfo *res;
	int				status;
	
	memset(&hints, 0, sizeof(hints));		
	hints.ai_family 	= AF_UNSPEC;			
	hints.ai_socktype 	= SOCK_STREAM;		
	hints.ai_flags 		= AI_PASSIVE;	
	hints.ai_protocol 	= IPPROTO_TCP;		

	if ((status = getaddrinfo("localhost", port, &hints, &res)) != 0) {
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;  // ERROR LOG 
		return (NULL);						
	}
	//printNetworkInfo(res);
	return (res);
}




/* 
	Launch server essentially is a summary of all actions requires to launch the server. WE
	must first get the network information and then binds the IPaddress and PORT together, so
	that we can start listening to incoming request. Also, if socketFds contains only value -1,
	I exit the program. 
 */
int *launchServer(ConfigFile config)
{
	struct addrinfo *ipAddressList;
	std::vector<std::string> ArrayPorts = config.get_listen();
	size_t invalidSockets = 0;

	int *socketFds =  new int[ArrayPorts.size()];
	for (size_t i = 0; i < ArrayPorts.size(); i++) {
		const char* charPtr = ArrayPorts[i].c_str();
		ipAddressList = getNetworkInfo(charPtr);
		socketFds[i] = serverSocketSetup(ipAddressList);
		freeaddrinfo(ipAddressList);
		if (socketFds[i] == -1)
			invalidSockets++;
	}
	if (invalidSockets == ArrayPorts.size()){
		std::cout << "Closing the program because no valid ports" << std::endl;  // ERROR LOG 
		exit(EXIT_FAILURE);
	}
	return (socketFds);
}
