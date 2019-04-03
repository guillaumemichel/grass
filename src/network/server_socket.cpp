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

void Server::readFromUserSocket(int userSocket) {
    bool stopFlag = false;

    // Run while we are not receiving the "exit" command
    while (!stopFlag) {
        // Buffer where we'll store the data sent by the client
        char buffer[SOCKET_BUFFER_SIZE];
        // Clean the old data in it by setting 0
        memset(buffer, 0, SOCKET_BUFFER_SIZE);

        // Now we can read the data
        // TODO : check if read does not return 0 or -1
        if (0 < read(userSocket, buffer, SOCKET_BUFFER_SIZE)) {
            // TODO : later
            //string command = str(buffer);
            //int permission_level = 2;
            //int i = exec_command(command, permission_level);

            // My command interpreter
            if (0 == strcmp(buffer, "exit")) {
                cout << "Signal to shutdown the server was received..." << endl;
                stopFlag = true;
            } else if (0 == strncmp(buffer, "put", 3)) {
                // Convert the buffer to string
                string command(buffer, SOCKET_BUFFER_SIZE);

                // Get the filename and the size
                string removePut = command.substr(command.find(" ") + 1);
                string filename = removePut.substr(0, removePut.find(" "));
                int size = std::stoi(removePut.substr(removePut.find(" ") + 1));

                // TODO : throw exception if expected values are not present

                // On upload we have to start a new thread and a new socket

                // First we send to the client the port number
                // Generate random port
                // TODO : check if port is usable or nto
                int portNumber = 10000 + (std::rand() % (42420 - 10000 + 1));
                string message = "put port: " + to_string(portNumber);

                // TODO : create a method "send" in the class
                if (-1 == send(userSocket, message.c_str(), message.size(), 0)) {
                    throw invalid_argument("Cannot send the port to the client");
                }

                // Then we start a new thread to receive it
                thread t1(Server::receiveFileUpload, filename, size, portNumber);
                t1.join();
            } else {
                cout << "Command received : " << buffer << endl;
            }
        } else {
            cout << "Error while reading from the socket" << endl;
        }
    }
}

void Server::receiveFileUpload(string filename, int size, int port) {
    cout << "Starting a new thread for the receiving server on port " << port << ". The size of the file is : " << size
         << endl;

    Server receivingServer(port);

    if (-1 == receivingServer.initiateConnection()) {
        throw invalid_argument("Cannot start the receiving server");
    }

    cout << "New thread instantiated, waiting for the client to connect..." << endl;

    int receivingSocket;
    struct sockaddr_in sockaddrIn;
    int addrlen = sizeof(sockaddrIn);
    if ((receivingSocket = accept(receivingServer.getSocket(), (struct sockaddr *) &receivingServer.address,
                                  (socklen_t * ) & addrlen)) < 0) {
        perror("accept");
        throw invalid_argument("Cannot listen for sockets");
    }

    cout << "File transfer started" << endl;

    // ==== READ THE FILE ==== //

    // Rewrite the filename to the "uploaded" directory
    filename = "uploaded/" + filename;

    // Create a file writer to write the file
    FileWriter fw(filename);

    // Clear the file in case of all data was there
    fw.clearFile();

    // Buffer where we'll store the data sent by the client
    char *buffer;

    // Allocating the memory to the buffer
    buffer = (char *) malloc(size);

    // Check if buffer was correctly allocated
    if (buffer == nullptr) {
        throw invalid_argument("Cannot allocate the buffer");
    } else {
        memset(buffer, 0, size);
        // Now we can read the data
        // TODO : check if read does not return 0 or -1
        read(receivingSocket, buffer, size);

        // Create the string and write it to the file
        string line(buffer, size);
        cout << "File received : " << line << endl;
        fw.writeLine(line);

        // Finally we clean and free the buffer
        memset(buffer, 0, size);
        free(buffer);
    }

    // Once the file transfer is done, we close the socket
    close(receivingSocket);

    cout << "File transfer done" << endl;
}

