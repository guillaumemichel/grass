#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>


using namespace std;

#include "../../include/client_socket.h"
#include "../../include/FileReader.h"

void Client::initiateConnection() {
    // Create the object in which we'll be storing the server address
    struct sockaddr_in serverAddress{};

    // Create the socket
    if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw invalid_argument("Cannot create the socket");
    }

    // Set the serverAddress object to 0 as initial values;
    memset(&serverAddress, 0, sizeof(serverAddress));

    // Configuration for our protocol
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
        throw invalid_argument("Invalid address");
    }

    // Tries to connect to the server
    if (connect(this->sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        throw invalid_argument("Connection to server failed");
    }
}

string Client::readCommand() {
    // Read the command of the user
    string command = "";

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
    // Thanks to that he can allocate the right amount of memory to read the string
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

void Client::sendToServerWithoutSize(string toSend) {
    // Same as above function but doesn't send the size as a preamble
    if (-1 == send(this->getSocket(), toSend.data(), toSend.size(), 0)) {
        cout << "Error : cannot send the data";
    }
}

void Client::readFromServer() {
    if (this->isSocketInitiated()) {

        char buffer[14] = {0};

        // Read data from the server
        ssize_t valRead = read(this->getSocket(), buffer, 14);
        if (-1 == valRead) {
            cout << "Error while reading data from server" << endl;
        } else {
            cout << "Received from server : " << buffer << endl;
        }
    } else {
        cout << "Cannot read from server, the socket was not initiated" << endl;
    }
}


void Client::uploadFile() {
    // Upload dummy file for test purpose
    FileReader fileReader("/home/alex/Documents/EPFL/SoftSec/Project/ass_on_the_grass/test.txt");

    // We first read the file
    vector<string> vecOfStr;
    fileReader.readFileVector(vecOfStr);

    string nbrStrings = to_string(vecOfStr.size());

    // Send to the server the # of strings it has to read
    this->sendToServerWithoutSize(nbrStrings);

    // Then we send the lines 1 by 1
    vector<string>::iterator it;
    for (it = vecOfStr.begin(); it != vecOfStr.end(); ++it) {
        this->sendToServer(*it);
    }

    cout << "File uploaded!" << endl;
}

Client::Client(uint16_t dstPort) {
    this->port = dstPort;
}
