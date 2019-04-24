/**
 * ClientLauncher.cpp
 * Implementation of the ClientLauncher.h file. Launches and runs the client to connect it to the server. Also does all tasks related to the user interface such as reading commands and also uploading / downloading files.
 *
 * @author Alexandre Chambet
 */

#include "../include/ClientLauncher.h"

using namespace std;

void ClientLauncher::downloadFile(string filename, unsigned int size, unsigned int port) {
    cout << "Starting new thread to receive the file from the server" << endl;
    ClientSocket client(port);

    ClientLauncher::fileTransferConnect(&client);

    client.downloadFile(filename, size);

    // Close the NetworkSocket
    client.closeConnection();
}


void ClientLauncher::uploadFile(string filename, string size, unsigned int port) {
    cout << "Starting new thread to send the file to the server" << endl;
    ClientSocket client(port);

    ClientLauncher::fileTransferConnect(&client);

    client.uploadFile(filename);

    // Close the NetworkSocket
    client.closeConnection();
}

void ClientLauncher::fileTransferConnect(ClientSocket *client) {
    // Loop while the server is ready
    int maxTries = 10;
    bool connected = false;
    while (maxTries > 0 && !connected) {
        // Tries to connect
        try {
            client->initiateConnection();
            connected = true;
        } catch (exception &e) {
            cout << "Cannot connect to the server... Attempting again..." << endl;
            maxTries--;
            // Wait an exponential amount of time until the server is ready
            usleep(100 * (10 - maxTries));
        }
    }

    if (maxTries == 0) {
        throw Exception(ERR_NETWORK_CONNECTION_SERVER_FAILED);
    }

    // Should be ok, but we just check if the sock was properly created
    if (!(client->isSocketInitiated())) {
        throw Exception(ERR_NETWORK_SOCKET_NOT_CREATED);
    }
}

void ClientLauncher::startClient(unsigned int serverPort) {
    // Instantiate a new client
    ClientSocket client(serverPort);

    client.initiateConnection();

    string command;
    string returned = "";

    // Loop while command is not exit
    do {
        // Read the command
        command = client.readCommand();

        // TODO : refactor put & get command (same stuff)
        // TODO : create constant for command names
        // If a file must be upload
        if (command.substr(0, 3) == "put") {
            string removePut = command.substr(command.find(" ") + 1);
            string filename = removePut.substr(0, removePut.find(" "));
            string size = removePut.substr(
                    removePut.find(" ") + 1); // TODO : what if the size is not the same size of the file?

            // Send the command to the server
            client.sendToServer(command);

            // Wait for the server answer
            string read = client.readFromServer();

            // Parse the port number
            cout << read << endl;
            int port = stoi(read.substr(read.find(":") + 2));

            // Upload the file
            thread t1(ClientLauncher::uploadFile, filename, size, port);
            t1.detach();
        } else if (command.substr(0, 3) == "get") {
            string removePut = command.substr(command.find(" ") + 1);
            string filename = removePut.substr(0, removePut.find(" "));

            // Send the command to the server
            client.sendToServer(command);

            // Wait for the server answer
            string read = client.readFromServer();

            cout << read << endl;
            // Check if the message has the expected form
            if (read.substr(0, 3) == "get") {
                int port = stoi(read.substr(read.find(":") + 2));
                string withoutPort = read.substr(read.find(":") + 1);
                int size = stoi(withoutPort.substr(withoutPort.find(":") + 2));

                // Download the file
                thread t1(ClientLauncher::downloadFile, filename, size, port);
                t1.detach();
            }
        } else {
            // Send the command to the server
            client.sendToServer(command);

            // Read and print the result from the server
            returned = client.readFromServer();
            cout << returned;
        }
    } while (returned.compare(str_bye));

}

int main(void) {
    // Creates a client starter
    ClientLauncher launcher;

    // Parses the configuration file
    Configuration conf = Configuration("grass.conf");

    try {
        // Launches a new client with the server port as a target destination
        launcher.startClient(conf.getPort());
    } catch (Exception &e) {
        e.print_error();
    }

    //cout << "Exiting the client" << endl;

    return 0;
}
