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

int main(int argc, char const *argv[]) {
    // Create a server object
    Server server;

    // Create the socket
    if (-1 == server.initiateConnection()) {
        cout << "aze";
        return -1;
    }

    cout << "Server socket initiated" << endl;
    cout << "Listening for incoming connections..." << endl;

    // TODO : refactor this
    int userSocket;
    struct sockaddr_in caca;
    int addrlen = sizeof(caca);
    if ((userSocket = accept(server.getSocket(), (struct sockaddr *) &server.address,
                             (socklen_t *) &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    // Accept a new user socket
   /* int userSocket = server.acceptSocket();

    if (-1 == userSocket) {
        cout << "azeaze";
        return -1;
    }*/


    cout << "A new user connected" << endl;

    if (0 == server.readFromUserSocket(userSocket)) {
        cout << "Safely exiting the server" << endl;
    }

    //send(new_socket, hello, strlen(hello), 0);

    return 0;
}

int Server::initiateConnection() {
    int opt = 1;

    // Creating socket file descriptor
    if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
        return -1;
    }

    // Forcefully attaching socket to the port
    if (setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        return -1;
    }

    // Setting up the socket
    (this->address).sin_family = AF_INET;
    (this->address).sin_addr.s_addr = INADDR_ANY;
    (this->address).sin_port = htons(this->PORT);

    // Forcefully attaching socket to the port provided by the user
    if (bind(this->sock, (struct sockaddr *) &(this->address), sizeof(this->address)) < 0) {
        perror("bind");
        return -1;
    }

    // Prepare to lister for incoming connections.
    if (listen(this->sock, 3) < 0) {
        perror("listen");
        return -1;
    }

    return 0;
}

int Server::getSocket() {
    return this->sock;
}

bool Server::isSocketInitiated() {
    return this->sock > 0;
}

// TODO : marche pas...
int Server::acceptSocket() {
    // Socket of a new connecting user
    int userSocket;
    struct sockaddr_in caca;
    int addressLength = sizeof(caca);

    // Check if the socket was properly created
    if (!this->isSocketInitiated()) {
        cout << "Error : cannot accept a new socket, the socket has not been created.";
        return -1;
    }

    // Accepting the new socket
    if ((userSocket = accept(this->sock, (struct sockaddr *) &(this->address),
                            (socklen_t *) &addressLength) < 0)) {
        perror("accept");
        return -1;
    }

    cout << "Socket accepted" << endl;
    cout << userSocket << endl;

    return userSocket;
}

int Server::readFromUserSocket(int userSocket) {
    // Buffer to get the size of the command
    size_t sizeToRead[1] = {0};
    bool stopFlag = false;

    // Get the size of the command
    while (!stopFlag && 0 < read(userSocket, sizeToRead, 1)) {
        // Buffer where we'll store the data sent by the client
        char *buffer;

        // Allocating the memory to the buffer
        buffer = (char *) malloc(sizeToRead[0]);

        // Check if buffer was correctly allocated
        if (buffer == nullptr) {
            cout << "Error while allocating the buffer...";
            // Exiting
            return -1;
        } else {
            // Allocate the buffer with 0
            for (int i = 0; i < sizeToRead[0]; i++) {
                buffer[i] = 0;
            }

            // Now we can read the data
            // TODO : check if read does not return 0 or -1
            if (0 < read(userSocket, buffer, sizeToRead[0])) {
                cout << "Command received : " << buffer << endl;

                if (0 == strcmp(buffer, "exit")) {
                    cout << "Signal to shutdown the server was received..." << endl;
                    free(buffer);
                    stopFlag = true;
                }
            }

            // Finally we free the buffer
            free(buffer);
        }

        sizeToRead[0] = {0};
    }

    return 0;
}
