#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>


class ServerConfiguration {
public:
    ServerConfiguration(const std::string& configFile); 
    // Méthodes d'accès aux données du serveur
    int getPort() const {
        return port_;
    }

    std::string getDocumentRoot() const {
        return document_root_;
    }

    std::string getLogFile() const {
        return log_file_;
    }

    std::string getLogLevel() const {
        return log_level_;
    }
    std::string getCgiRoot() const {
        return cgi_root_;
    }

    void printConfig();

private: 
    //eventuellement port sera un vecteur qui contient plusieurs ports.
    int port_;
    std::string document_root_;
    std::string log_file_;
    std::string log_level_;
    std::string cgi_root_;
};