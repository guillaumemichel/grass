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
            throw invalid_argument("Cannot allocate the buffer");
        } else {
            // Allocate the buffer with 0
            for (int i = 0; i < sizeToRead[0]; i++) {
                buffer[i] = 0;
            }

            // Now we can read the data
            // TODO : check if read does not return 0 or -1
            if (0 < read(userSocket, buffer, sizeToRead[0])) {
                // TODO : later
                //string command = str(buffer);
                //int permission_level = 2;
                //int i = exec_command(command, permission_level);

                // My command interpreter
                if (0 == strcmp(buffer, "exit")) {
                    cout << "Signal to shutdown the server was received..." << endl;
                    free(buffer);
                    stopFlag = true;
                } else if (0 == strncmp(buffer, "put", 3)) {
                    // Convert the buffer to string
                    string command(buffer, sizeToRead[0]);

                    // Get the filename and the size
                    string removePut = command.substr(command.find(" ") + 1);
                    string filename = removePut.substr(0, removePut.find(" "));
                    string size = removePut.substr(removePut.find(" ") + 1); // TODO : what to do with the size?

                    // TODO : throw exception if expected values are not present

                    // On upload we have to start a new thread and a new socket

                    // First we send to the client the port number
                    // Generate random port
                    // TODO : check if port is usable or nto
                    int portNumber = 10000 + (std::rand() % (42420 - 10000 + 1));
                    string message = "put port: " + to_string(portNumber);

                    if (-1 == send(userSocket, message.c_str(), message.size(), 0)) {
                        throw invalid_argument("Cannot send the port to the client");
                    }

                    // Then we start a new thread to receive it
                    thread t1(Server::receiveFileUpload, filename, portNumber);
                    t1.join();
                } else {
                    cout << "Command received : " << buffer << endl;
                }
            }

            // Finally we clean and free the buffer
            memset(buffer, 0, sizeToRead[0]);
            free(buffer);
        }

        sizeToRead[0] = {0};
    }
}

void Server::receiveFileUpload(string filename, int port) {
    cout << "Starting a new thread for the receiving server on port " << port << endl;

    Server receivingServer(port);

    if (-1 == receivingServer.initiateConnection()) {
        throw invalid_argument("Cannot start the receiving server");
    }

    cout << "New thread instantiated, waiting for the client to connect..." << endl;

    int userSocket;
    struct sockaddr_in sockaddrIn;
    int addrlen = sizeof(sockaddrIn);
    if ((userSocket = accept(receivingServer.getSocket(), (struct sockaddr *) &receivingServer.address,
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

    // Buffer to get the size the file and of each line
    size_t sizeToRead[1] = {0};

    // Read the # of lines
    // TODO : check if read returns 0 or -1
    read(userSocket, sizeToRead, 1);

    // Converting from byte to int
    int nbrLines = (int) sizeToRead[0] - '0';

    // Reinitialise the buffer to read lines
    sizeToRead[0] = {0};

    // Read each line successively
    for (int i = 0; i < nbrLines; i++) {
        // Get the size of the line
        read(userSocket, sizeToRead, 1);
        cout << " ~~~~~ Next line size : " << sizeToRead[0] << endl;

        // Buffer where we'll store the data sent by the client
        char *buffer;

        // Allocating the memory to the buffer
        buffer = (char *) malloc(sizeToRead[0]);

        // Check if buffer was correctly allocated
        if (buffer == nullptr) {
            throw invalid_argument("Cannot allocate the buffer");
        } else {
            // Now we can read the data
            // TODO : check if read does not return 0 or -1
            read(userSocket, buffer, sizeToRead[0]);

            // Create the string and write it to the file
            string line(buffer, sizeToRead[0]);
            fw.writeLine(line);

            cout << " ~~~~~ Line n°" << i << " received : " << line << endl;

            // Finally we clean and free the buffer
            memset(buffer, 0, sizeToRead[0]);
            free(buffer);
        }
        sizeToRead[0] = {0};
    }

    // Once the file transfer is done, we close the socket
    close(userSocket);

    cout << "File transfer done" << endl;
}

