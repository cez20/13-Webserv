#pragma once

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <poll.h>
#include <vector>
#include <map>
#include <sstream>
#include <regex>
#include <cerrno>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "ConfigFile.hpp"

#define BLUE_CL "\033[1;36m"
#define WHITE_CL "\033[0;37m"
#define RED_CL "\033[0;31m"
#define YELLOW_CL "\033[0;33m"
#define PINK_CL "\033[38;2;255;192;203m"
#define GREEN_CL "\033[0;32m"
#define DEFAULT_CL "\033[0m"

#define MAX_PENDING_CONNECTIONS	5

int launchServer();
int monitorServer(int serverSocket, ConfigFile config);
struct addrinfo 	*getNetworkInfo();
std::string extractFileContent(const std::string& path);
bool endsWith(const std::string& str, const std::string& suffix);

template <typename Key, typename Value>
void printMap(const std::map<Key, Value>& mapContainer) {
    typedef typename std::map<Key, Value>::const_iterator MapIterator;
    for (MapIterator it = mapContainer.begin(); it != mapContainer.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}
template<typename T>
void printStructure(const T& structure) {
    typedef typename T::value_type ValueType;  // Type des éléments de la structure

    for (typename T::const_iterator it = structure.begin(); it != structure.end(); ++it) {
        const ValueType& element = *it;
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

