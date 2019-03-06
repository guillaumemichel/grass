#include "Server.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

using namespace std;

#include "Server.h"

#define PORT 8080

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    ssize_t valread = 0;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    //char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *) &address,
             sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                             (socklen_t *) &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Buffer to get the size of the command
    size_t sizeToRead[1] = {0};
    bool stopFlag = false;

    // Get the size of the command
    while (!stopFlag && 0 < read(new_socket, sizeToRead, 1)) {
        // Buffer where we'll store the data sent by the client
        char *buffer;

        // Allocating the memory to the buffer
        buffer = (char *) malloc(sizeToRead[0]);

        // Check if buffer was correctly allocated
        if (buffer == nullptr) {
            cout << "Error while allocating the buffer...";
        } else {
            // Allocate the buffer with 0
            for (int i = 0; i < sizeToRead[0]; i++) {
                buffer[i] = 0;
            }

            // Now we can read the data
            // TODO : check if read does not return 0 or -1
            if (0 < read(new_socket, buffer, sizeToRead[0])) {
                cout << "Command received : " << buffer << endl;

                if (0 == strcmp(buffer, "exit")) {
                    free(buffer);
                    stopFlag = true;
                }
            }

            // Finally we free the buffer
            free(buffer);
        }

        sizeToRead[0] = {0};
    }
    //send(new_socket, hello, strlen(hello), 0);

    cout << "Server has shut down" << endl;

    return 0;
}
