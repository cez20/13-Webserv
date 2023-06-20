
#include "../inc/ServerConfiguration.hpp"


ServerConfiguration::ServerConfiguration(const std::string& configFile) : port_(0), document_root_(""), log_file_(""),log_level_("")  {
    std::ifstream file(configFile);
    if (!file) {
        std::cerr << "Error opening file: " << configFile << std::endl;
        // Throw une exception
    }
    std::string line;
    while (std::getline(file, line)) {
    // Ignorer les commentaires et les lignes vides, il faudra peut etre gerer les braces, voir le fichier de confifguratoin
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Diviser la ligne en clé et valeur
        std::size_t delimiterPos = line.find(' ');
        if (delimiterPos == std::string::npos) {
            std::cerr << "Invalid line in configuration file: " << line << std::endl;
            // on throw une exception, mais est-ce qu<on ferme le programme pareil si une ligne est mauvaise.
        }
        std::string key = line.substr(0, delimiterPos);
        std::string value = line.substr(delimiterPos + 1);
        // Remplir les donnees du serveur directement dans le constructeur, en attendant, peut etre 
        if (key == "port") {
            port_ = std::stoi(value);
        }
        else if (key == "document_root") {
            document_root_ = value;
        }
        else if (key == "log_file") {
            log_file_ = value;
        } 
        else if (key == "log_level") {
            log_level_ = value;
        }
        else if (key == "cgi_root") {
            cgi_root_ = value;
        }  
    } 
}

void ServerConfiguration:: printConfig() {
        std::cout << "port: " << port_ << std::endl;
        std::cout << "document_root: " << document_root_ << std::endl;
        std::cout << "log_file: " << log_file_ << std::endl;
        std::cout << "log_level: " << log_level_ << std::endl;
        // Autres variables privées...
    }

