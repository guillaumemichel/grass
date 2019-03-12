#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <thread>

using namespace std;

#include "../../include/server_socket.h"

int main2(int argc, char const *argv[]) {
    // Create a server object
    Server server(8080);

    // Create the socket
    if (-1 == server.initiateConnection()) {
        cout << "aze";
        return -1;
    }

    cout << "Server socket initiated" << endl;
    cout << "Listening for incoming connections..." << endl;

    // TODO : refactor this
    int userSocket;
    struct sockaddr_in sockaddrIn;
    int addrlen = sizeof(sockaddrIn);
    if ((userSocket = accept(server.getSocket(), (struct sockaddr *) &server.address,
                             (socklen_t *) &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    cout << "A new user connected" << endl;

    if (0 == server.readFromUserSocket(userSocket)) {
        cout << "Safely exiting the server" << endl;
    }

    return 0;
}

Server::Server(uint16_t port) {
    this->port = port;
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
    (this->address).sin_port = htons(this->port);

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

int Server::readFromUserSocket(int userSocket) {
    // TODO : WHAT IF A USER SETS A BIIIIIG NUMBER AND THE COMMAND IS WAY SMALLER THAN THAT ?
    // --> Maybe put a size limit or whatever ?

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
                if (0 == strcmp(buffer, "exit")) {
                    cout << "Signal to shutdown the server was received..." << endl;
                    free(buffer);
                    stopFlag = true;
                } else if (0 == strcmp(buffer, "put")) {
                    // On upload we have to start a new thread and a new socket

                    // First we send to the client the port number
                    string message = "put port: 9999";
                    if (-1 == send(userSocket, message.c_str(), message.size(), 0)) {
                        throw invalid_argument("Cannot send the port to the client");
                    }

                    // Then we start a new thread to receive it
                    thread t1(Server::receiveFileUpload);
                    t1.join();
                } else {
                    cout << "Command received : " << buffer << endl;
                }
            }

            // Finally we free the buffer
            free(buffer);
        }

        sizeToRead[0] = {0};
    }

    return 0;
}

void Server::receiveFileUpload() {
    cout << "Starting a new thread for the receiving server" << endl;

    Server receivingServer(9999);

    if (-1 == receivingServer.initiateConnection()) {
        throw invalid_argument("Cannot start the receiving server");
    }

    int userSocket;
    struct sockaddr_in sockaddrIn;
    int addrlen = sizeof(sockaddrIn);
    if ((userSocket = accept(receivingServer.getSocket(), (struct sockaddr *) &receivingServer.address,
                             (socklen_t *) &addrlen)) < 0) {
        perror("accept");
        throw invalid_argument("Cannot listen for sockets");
    }

    cout << "File transfer started" << endl;

    receivingServer.readFromUserSocket(userSocket);
}
