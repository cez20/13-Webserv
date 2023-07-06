#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>


// Define the URL of your server
const std::string serverUrl = "http://localhost:8080"; // Replace with your server's URL or IP address

// Function to send a request to the server
size_t sendRequest(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    return fwrite(ptr, size, nmemb, stream);
}

int main() {
    while (true) {
        FILE* pipe = popen(("curl " + serverUrl).c_str(), "r");
        if (pipe) {
            char buffer[128];
            std::string result;
            while (!feof(pipe)) {
                if (fgets(buffer, 128, pipe) != NULL)
                    result += buffer;
            }
            pclose(pipe);
            std::cout << "Request successful! Response: " << result << std::endl;
        } else {
            std::cout << "Request failed" << std::endl;
        }
    }

    return 0;
}