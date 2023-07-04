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
	int yes= 1;

	int serverSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (serverSocket == -1) {
		std::cerr << "Error while creating the server socket." << std::endl;
		exit(EXIT_FAILURE);
	}

	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		std::cerr << "Error with setsockopt: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (bind(serverSocket, res->ai_addr, res->ai_addrlen) == -1) {
		std::cerr << "Error while trying to bind the socket." << std::endl;
		close(serverSocket);
		exit(EXIT_FAILURE);
	}
	if (listen(serverSocket, MAX_PENDING_CONNECTIONS) == -1) {
		std::cerr << "Error while trying to listen" << std::endl;
		close(serverSocket);
		exit(EXIT_FAILURE);
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
	
	memset(&hints, 0, sizeof(hints));		
	hints.ai_family 	= AF_UNSPEC;			
	hints.ai_socktype 	= SOCK_STREAM;		
	hints.ai_flags 		= AI_PASSIVE;		// Flag to assign the address of my localhost (127.0.0.1) to the socket structure.  
	hints.ai_protocol 	= IPPROTO_TCP;		

	if ((status = getaddrinfo(NULL, "8080", &hints, &res)) != 0) {
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		exit(EXIT_FAILURE);							
	}
	//printNetworkInfo(res);
	return (res);
}

/* 
	Launch server essentially is a summary of all actions requires to launch the server. WE
	must first get the network information and then binds the IPaddress and PORT together, so
	that we can start listening to incoming request. 
 */
int launchServer() 
{
	struct addrinfo *ipAddressList;

	ipAddressList = getNetworkInfo();
	int serverSocket = serverSocketSetup(ipAddressList);
	freeaddrinfo(ipAddressList);

	return (serverSocket);
}
