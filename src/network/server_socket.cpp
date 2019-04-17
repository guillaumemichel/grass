using namespace std;

#include "../../include/server_socket.h"

Server::Server(uint16_t port) : NetworkSocket(port) {
}

void Server::initiateConnection() {
    int opt = 1;

    // Common settings to create the socket
    this->commonInitiateConnection();

    // Forcefully attaching NetworkSocket to the port
    if (setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        throw invalid_argument("Cannot configure the socket");
    }

    // Setting up the server socket
    (this->address).sin_addr.s_addr = INADDR_ANY;

    // Forcefully attaching NetworkSocket to the port provided by the user
    if (bind(this->sock, (struct sockaddr *) &(this->address), sizeof(this->address)) < 0) {
        throw invalid_argument("Cannot bind the socket");
    }

    // Prepare to lister for incoming connections.
    if (listen(this->sock, 3) < 0) {
        throw invalid_argument("Cannot make the socket to listen");
    }
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
        if (0 < read(userSocket, buffer, SOCKET_BUFFER_SIZE)) {
            // Convert the buffer to string
            string command(buffer, SOCKET_BUFFER_SIZE);

            // My command interpreter
            if (0 == strcmp("exit", buffer)) {
                cout << "Exiting the client..." << endl;
                stopFlag = true;
            } else if (0 == strncmp(buffer, "put", 3)) {
                // Get the filename and the size
                string removePut = command.substr(command.find(" ") + 1);
                string filename = removePut.substr(0, removePut.find(" "));
                int size = std::stoi(removePut.substr(removePut.find(" ") + 1));

                // TODO : throw exception if expected values are not present

                // On upload we have to start a new thread and a new NetworkSocket

                // First we send to the client the port number
                // Generate random port
                // TODO : check if port is usable or not
                int portNumber = 10000 + (std::rand() % (42420 - 10000 + 1));
                string message = "put port: " + to_string(portNumber);

                // Send it to the client
                sendToClient(userSocket, message);

                // Then we start a new thread to receive it
                thread t1(Server::receiveFileUpload, filename, size, portNumber);
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
                // Generate random port
                // TODO : check if port is usable or nto
                int portNumber = 10000 + (std::rand() % (42420 - 10000 + 1));
                string message = "get port: " + to_string(portNumber) + " size: " + to_string(fileReader.fileSize());

                // Send it to the client
                sendToClient(userSocket, message);

                // Then we start a new thread to receive it
                thread t1(Server::sendFile, filename, portNumber);
                t1.detach();
            } else {
                cout << "Command received : " << buffer << endl;
                // Execute the command
                int permission_level = 2;
                int i = exec_command(command, permission_level);
            }
        } else {
            cout << "Error while reading from the NetworkSocket" << endl;
        }
    }
}

void Server::receiveFileUpload(string filename, int size, int port) {
    cout << "Starting a new thread for the receiving server on port " << port << ". The size of the file is : " << size
         << endl;

    Server receivingServer(port);

    receivingServer.initiateConnection();

    cout << "New thread instantiated, waiting for the client to connect..." << endl;

    int receivingSocket = receivingServer.allocateSocketClient();

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
        throw invalid_argument("Cannot allocate the buffer");
    } else {
        memset(buffer, 0, size);
        // Now we can read the data
        // TODO : check if read does not return 0 or -1
        read(receivingSocket, buffer, size);

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

void Server::sendToClient(int socket, string message) {
    this->sendTo(socket, message);
}

void Server::sendFile(string filename, int port) {
    cout << "Starting new thread to send the file to the client" << endl;
    Server server(port);

    server.initiateConnection();

    // Should be ok, but we just check if the sock was properly created
    if (!server.isSocketInitiated()) {
        throw invalid_argument("The NetworkSocket was not properly created");
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

int Server::allocateSocketClient() {
    // The client socket
    int userSocket;

    int addrlen = sizeof(this->address);
    // Waiting for a client to conenct
    if ((userSocket = accept(this->sock, (struct sockaddr *) &(this->address),
                             (socklen_t * ) & addrlen)) < 0) {
        throw Exception(ERR_ERR_NOT_FOUND);
    }

    // Returning the socket
    return userSocket;
}