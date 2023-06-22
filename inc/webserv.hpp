#pragma once

#include <iostream>
#include <sys/socket.h>
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
#include "ServerConfiguration.hpp"

#define BLUE_CL "\033[1;36m"
#define WHITE_CL "\033[0;37m"
#define RED_CL "\033[0;31m"
#define YELLOW_CL "\033[0;33m"
#define PINK_CL "\033[38;2;255;192;203m"
#define GREEN_CL "\033[0;32m"
#define DEFAULT_CL "\033[0m"

int server(const ServerConfiguration& config);
std::string extractFileContent(const std::string& path);

template <typename Key, typename Value>
void printMap(const std::map<Key, Value>& mapContainer) {
    typedef typename std::map<Key, Value>::const_iterator MapIterator;
    for (MapIterator it = mapContainer.begin(); it != mapContainer.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}

