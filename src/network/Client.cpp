#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

#include "Client.h"


#define PORT 8080

int main() {
    // Instantiate a new client
    Client client;

    if (-1 == client.initiateConnection()) {
        cout << "Error while initiating the connection to the server. Exiting..." << endl;
        return -1;
    }

    // Should be ok, but we just check if the sock was properly created
    if (!client.isSocketInitiated()) {
        cout << "The socket was not properly created. Exiting..." << endl;
        return -1;
    }

    // Read the command
    string command = client.readCommand();

    // Send the command to the server
    client.sendToServer(command);

    // Send the command to the server
    client.sendToServer(client.readCommand());

    // Read the data sent by the server
    //client.readFromServer();


    return 0;
}

int Client::initiateConnection() {
    // Create the object in which we'll be storing the server address
    struct sockaddr_in serverAddress{};

    // Create the socket
    if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Socket creation error" << endl;
        return -1;
    }

    // Set the serverAddress object to 0 as initial values;
    memset(&serverAddress, 0, sizeof(serverAddress));

    // Configuration for our protocol
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
        cout << "Invalid address/ Address not supported" << endl;
        return -1;
    }

    // Tries to connect to the server
    if (connect(this->sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        cout << "Connection Failed" << endl;
        return -1;
    }

    return 0;
}

string Client::readCommand() {
    // Read the command of the user
    string command;

    cout << "Enter your command : ";
    getline(cin, command);

    return command;
}

int Client::getSocket() {
    return this->sock;
}

bool Client::isSocketInitiated() {
    return this->sock > 0;
}

void Client::sendToServer(string toSend) {
    // Send the size of the data to the server
    // Thanks to that he can allocate the right amout of memory to read the string
    const size_t buf[1] = {toSend.size()};
    if (-1 == send(this->getSocket(), buf, 1, 0)) {
        cout << "Error : cannot send the size...";
    } else {
        // Then we send the actual command
        if (-1 == send(this->getSocket(), toSend.data(), toSend.size(), 0)) {
            cout << "Error : cannot send the data to the server..." << endl;
        }
    }
}

void Client::readFromServer() {
    if (this->isSocketInitiated()) {
        char buffer[1024] = {0};

        // Read data from the server
        ssize_t valRead = read(this->getSocket(), buffer, 1024);
        if (-1 == valRead) {
            cout << "Error while reading data from server" << endl;
        } else {
            cout << "Received from server : " << buffer << endl;
        }
    } else {
        cout << "Cannot read from server, the socket was not initiated" << endl;
    }
}
