
#include "webserv.hpp"

int main (int argc, char **argv){
    if(argc != 2){
        std::cerr << "The servers takes 1 argument, which is the path to a configuration file" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
            ConfigFile config(argv[1]);
            int serverSocket = launchServer();
		    monitorServer(serverSocket, config);
        
        //Launch the serveur
       
    }
    return 0;
}