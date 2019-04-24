/**
 * ServerSocket.cpp
 * Implementation of the ServerSocketSocket.h file.
 * Manages all the network stuff related to the server.
 *
 * @author Alexandre Chambet
 */

using namespace std;

#include "../../include/ServerSocket.h"
#include "../../include/Configuration.h"

ServerSocket::ServerSocket(unsigned int port): NetworkSocket(port) {}

void ServerSocket::initiateConnection() {
    int opt = 1;

    // Common settings to create the socket
    this->commonInitiateConnection();

    // Forcefully attaching NetworkSocket to the port
    if (setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        throw Exception(ERR_NETWORK_SOCKET_CONFIGURATION);
    }

    // Setting up the server socket
    (this->address).sin_addr.s_addr = INADDR_ANY;

    // Forcefully attaching NetworkSocket to the port provided by the user
    if (bind(this->sock, (struct sockaddr *) &(this->address), sizeof(this->address)) < 0) {
        throw Exception(ERR_NETWORK_SOCKET_CONFIGURATION);
    }

    // Prepare to lister for incoming connections.
    if (listen(this->sock, 3) < 0) {
        throw Exception(ERR_NETWORK_SOCKET_CONFIGURATION);
    }
}

void ServerSocket::readFromUserSocket(int userSocket, Commands commands) {

    bool stopFlag = false;

    // Number of errors while reading the socket
    int wrongRead = 0;

    // Run while we are not receiving the "exit" command
    while (!stopFlag) {
        // Buffer where we'll store the data sent by the client
        char buffer[SOCKET_BUFFER_SIZE];
        // Clean the old data in it by setting 0
        memset(buffer, 0, SOCKET_BUFFER_SIZE);

        // Now we can read the data
        if (0 < read(userSocket, buffer, SOCKET_BUFFER_SIZE)) {
            // First, if the read is correct, we reset the wrong read counter to 0
            wrongRead = 0;

            // Convert the buffer to string
            size_t len = (strlen(buffer)>SOCKET_BUFFER_SIZE) ? SOCKET_BUFFER_SIZE : strlen(buffer);
            string command(buffer, len);

            // My command interpreter
            /*if (0 == strcmp((str_exit).c_str(), buffer)) {
                cout << "Exiting the client..." << endl;
                stopFlag = true;
            } else */if (0 == strncmp(buffer, "put", 3)) {
                // Get the filename and the size
                string removePut = command.substr(command.find(" ") + 1);
                string filename = removePut.substr(0, removePut.find(" "));
                int size = std::stoi(removePut.substr(removePut.find(" ") + 1));

                // TODO : throw exception if expected values are not present

                // On upload we have to start a new thread and a new NetworkSocket

                // First we send to the client the port number
                int portNumber = this->getRandomPort();
                string message = "put port: " + to_string(portNumber);

                // Send it to the client
                sendToClient(userSocket, message);

                // Then we start a new thread to receive it
                thread t1(ServerSocket::receiveFileUpload, filename, size, portNumber);
                t1.detach();
            } else if (0 == strncmp(buffer, "get", 3)) {
                // Get the filename and the size
                string removePut = command.substr(command.find(" ") + 1);

                // The file must be taken from the upload basepath
                string filename = UPLOAD_BASEPATH + removePut.substr(0, removePut.find(" "));

                // Check if the file exists
                try {
                    FileReader fileReader(filename);
                } catch (exception &e) {
                    // If the file does not exist, we send to the client an error message
                    string errorMessage = "File does not exist";
                    sendToClient(userSocket, errorMessage);
                }

                // TODO : how to not redefine it???
                FileReader fileReader(filename);

                // On download we have to start a new thread and a new NetworkSocket

                // First we send to the client the port number
                int portNumber = this->getRandomPort();
                string message = "get port: " + to_string(portNumber) + " size: " + to_string(fileReader.fileSize());

                // Send it to the client
                sendToClient(userSocket, message);

                // Then we start a new thread to receive it
                thread t1(ServerSocket::sendFile, filename, portNumber);
                t1.detach();
            } else {
                cout << "Command received : " << buffer << endl;
                // Execute the command
                string response = commands.exec(command, userSocket);
                if (response==str_bye) stopFlag=true;

                try{
                  this->sendToClient(userSocket, response);
                } catch(Exception e){
                  e.print_error();
                }
                cout << "Response sent to client" << endl;
                //cout << "Response : " << i <<endl;
            }
        } else {
            // Increase the wrong read
            wrongRead += 1;

            // If the number of errors while reading from the socket is superior to the threshold we stop the connection
            // and assume the client crashed or didn't exit properly
            if (wrongRead >= MAX_WRONG_READ) {
                stopFlag = true;
            }
        }
    }
}

int ServerSocket::getRandomPort() {
    // TODO : check if the port is free (or we assume lmao)
    // 42420 blaze it
    int portNumber = 10000 + (std::rand() % (42420 - 10000 + 1));

    return portNumber;
}

void ServerSocket::receiveFileUpload(string filename, unsigned int size, unsigned int port) {
    cout << "Starting a new thread for the receiving server on port " << port << ". The size of the file is : " << size
         << endl;

    ServerSocket receivingServerSocket(port);

    receivingServerSocket.initiateConnection();

    cout << "New thread instantiated, waiting for the client to connect..." << endl;

    int receivingSocket = receivingServerSocket.allocateSocketClient();

    cout << "File transfer started" << endl;

    // ==== READ THE FILE ==== //

    // Rewrite the filename to the upload directory
    filename = UPLOAD_BASEPATH + filename;

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
        // Clean the buffer
        memset(buffer, 0, size);

        // Now we can read the data
        if (read(receivingSocket, buffer, size) <= 0) {
            throw new Exception(ERR_NETWORK_READ_SOCKET);
        }

        // Create the string and write it to the file
        string line(buffer, size);
        fw.writeLine(line);

        // Finally we clean and free the buffer
        memset(buffer, 0, size);
        free(buffer);
    }

    // Once the file transfer is done, we close the NetworkSocket
    close(receivingSocket);

    cout << "File transfer done" << endl;
}

void ServerSocket::sendToClient(int socket, string message) {
    this->sendTo(socket, message);
}

void ServerSocket::sendFile(string filename, unsigned int port) {
    cout << "Starting new thread to send the file to the client" << endl;
    ServerSocket server(port);

    server.initiateConnection();

    // Should be ok, but we just check if the sock was properly created
    if (!server.isSocketInitiated()) {
        throw Exception(ERR_NETWORK_SOCKET_NOT_CREATED);
    }

    // Wait for the client to connect
    int userSocket = server.allocateSocketClient();

    FileReader fileReader(filename);

    // We first read the file
    vector <string> vecOfStr;
    fileReader.readFileVector(vecOfStr);

    // Then we send the lines 1 by 1
    vector<string>::iterator it;
    for (it = vecOfStr.begin(); it != vecOfStr.end(); ++it) {
        // Mandatory appending \n
        string toSend = *it + "\n";
        server.sendToClient(userSocket, toSend);
    }

    cout << "File send to the client!" << endl;

    // Close the NetworkSocket
    server.closeConnection();

    cout << "Closing the server thread" << endl;
}

int ServerSocket::allocateSocketClient() {
    // The client socket
    int userSocket;

    // Store the length in a variable
    int addrlen = sizeof(this->address);

    // The socket will be created when a client connect
    if ((userSocket = accept(this->sock, (struct sockaddr *) &(this->address), (socklen_t * ) & addrlen)) < 0) {
        throw Exception(ERR_NETWORK_ACCEPT_SOCKET);
    }

    // Returns the socket
    return userSocket;
}
