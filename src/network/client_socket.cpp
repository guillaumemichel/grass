using namespace std;

#include "../../include/client_socket.h"

Client::Client(unsigned int dstPort) : NetworkSocket(dstPort) {
}

const string Client::EXIT_CMD = "exit";

void Client::initiateConnection() {
    // Common settings to create the socket
    this->commonInitiateConnection();

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &(this->address).sin_addr) <= 0) {
        throw Exception(ERR_NETWORK_BAD_ADDRESS);
    }

    // Tries to connect to the server
    if (connect(this->sock, (struct sockaddr *) &(this->address), sizeof(this->address)) < 0) {
        throw Exception(ERR_NETWORK_CONNECTION_SERVER_FAILED);
    }
}

string Client::readCommand() {
    // Read the command of the user
    string command = "";

    cout << "Enter your command : ";
    getline(cin, command);

    return command;
}

void Client::sendToServer(string toSend) {
    this->sendTo(this->getSocket(), toSend);
}

string Client::readFromServer() {
    if (this->isSocketInitiated()) {
        // TODO : replace this by dynamic size
        char buffer[Client::SOCKET_BUFFER_SIZE] = {0};

        // Read data from the server
        ssize_t valRead = read(this->getSocket(), buffer, Client::SOCKET_BUFFER_SIZE);
        if (-1 == valRead) {
            throw Exception(ERR_NETWORK_READ_SOCKET);
        } else {
            return string(buffer, Client::SOCKET_BUFFER_SIZE);
        }
    } else {
        throw Exception(ERR_NETWORK_SOCKET_NOT_CREATED);
    }
}

void Client::uploadFile(string filename) {
    filename = BASEPATH + filename;
    FileReader fileReader(filename);

    // We first read the file
    vector <string> vecOfStr;
    fileReader.readFileVector(vecOfStr);

    // Then we send the lines 1 by 1
    vector<string>::iterator it;
    for (it = vecOfStr.begin(); it != vecOfStr.end(); ++it) {

        // Mandatory appending \n
        string toSend = *it + "\n";
        this->sendToServer(toSend);
    }

    cout << "File uploaded!" << endl;
}

void Client::downloadFile(string filename, unsigned int size) {
    // Rewrite the filename to the download directory
    filename = DOWNLOAD_BASEPATH + filename;

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
        throw Exception(ERR_MEMORY_MALLOC);
    } else {
        memset(buffer, 0, size);
        // Now we can read the data
        // TODO : check if read does not return 0 or -1
        if (read(this->sock, buffer, size) > 0) {
            // Create the string and write it to the file
            string line(buffer, size);
            fw.writeLine(line);
        } else {
            throw Exception(ERR_NETWORK_READ_SOCKET);
        }

        // Finally we clean and free the buffer
        memset(buffer, 0, size);
        free(buffer);
    }
}
