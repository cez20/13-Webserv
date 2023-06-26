
#include "webserv.hpp"
int main (int argc, char **argv){
    if(argc != 2){
        std::cerr << "The servers takes 1 argument, which is the path to a configuration file" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        // to be replaced with real parsing (Tristan)
        ServerConfiguration config(argv[1]);
        config.printConfig();
        //
        //Launch the serveur
        server(config);
        
    }
    return 0;
}