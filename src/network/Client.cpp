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

#include "Client.h"
#include "FileReader.h"


#define PORT 8080

void ouba() {
    cout << "Starting new thread to send the file to the server" << endl;
    Client client(9999);

    // Loop while the server is ready
    int maxTries = 10;
    while (-1 == client.initiateConnection() && maxTries > 0) {
        cout << "Cannot connect to the server... Attempting again...";
        maxTries--;
        sleep(100);
    }

    if (maxTries == 0) {
        throw invalid_argument("Cannot connect to the server");
    }

    // Should be ok, but we just check if the sock was properly created
    if (!client.isSocketInitiated()) {
        throw invalid_argument("The socket was not properly created");
    }

    client.uploadFile();
}

int main() {
    // Instantiate a new client
    Client client(PORT);

    if (-1 == client.initiateConnection()) {
        cout << "Error while initiating the connection to the server. Exiting..." << endl;
        return -1;
    }

    // Should be ok, but we just check if the sock was properly created
    if (!client.isSocketInitiated()) {
        cout << "The socket was not properly created. Exiting..." << endl;
        return -1;
    }

    string command;

    // Loop while command is not exit
    do {
        // Read the command
        command = client.readCommand();

        if (command == "put") {
            // Send the command to the server
            client.sendToServer(command);

            // Wait for the server answer
            client.readFromServer();

            // Upload the file
            thread t1(ouba);
            t1.join();
        } else {
            // Send the command to the server
            client.sendToServer(command);
        }
    } while (command != Client::EXIT_CMD);

    cout << "Exiting the client" << endl;

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
    serverAddress.sin_port = htons(this->port);

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
    FileReader fileReader("test.txt");

    // We first read the file
    vector<string> vecOfStr;
    fileReader.readFileVector(vecOfStr);

    string nbrStrings = to_string(vecOfStr.size());

    // Send to the server the # of strings it has to read
    this->sendToServer(nbrStrings);

    // Then we send the lines 1 by 1
    vector<string>::iterator it;
    for (it = vecOfStr.begin(); it != vecOfStr.end(); ++it) {
        this->sendToServer(*it);
    }
}

Client::Client(uint16_t dstPort) {
    this->port = dstPort;
}
