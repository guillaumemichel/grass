/**
 * ClientLauncher.cpp
 * Implementation of the ClientLauncher.h file. Launches and runs the client to connect it to the server. Also does all tasks related to the user interface such as reading commands and also uploading / downloading files.
 *
 * @author Alexandre Chambet
 */

#include "../include/ClientLauncher.h"

using namespace std;

void ClientLauncher::downloadFile(string filename, unsigned int size, string serverIP, unsigned int port) {
    cout << "Starting new thread to receive the file from the server" << endl;
    ClientSocket client(serverIP, port);

    ClientLauncher::fileTransferConnect(&client);

    client.downloadFile(filename, size);

    // Close the NetworkSocket
    client.closeConnection();
}


void ClientLauncher::uploadFile(string filename, string size, string serverIP, unsigned int port) {
    cout << "Starting new thread to send the file to the server" << endl;
    ClientSocket client(serverIP, port);

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

bool ClientLauncher::isValidIpAddress(char *ipAddress) {
    struct sockaddr_in sa;

    // Uses the inet_pton function to check if the IP is correct
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}


// TODO : refactor this with the automated stuff
void ClientLauncher::startClient(string serverIP, unsigned int serverPort) {
    // Instantiate a new client
    ClientSocket client(serverIP, serverPort);

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
            thread t1(ClientLauncher::uploadFile, filename, size, serverIP, port);
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
                thread t1(ClientLauncher::downloadFile, filename, size, serverIP, port);
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


vector<string> ClientLauncher::startClientAutomated(string serverIP, unsigned int serverPort, vector <string> commands) {
    cout << "Starting the client in the automated mode" << endl;

    // Instantiate a new client
    ClientSocket client(serverIP, serverPort);

    client.initiateConnection();

    // Iterator for the commands
    vector<string>::iterator it;

    // The string returned from the server
    vector <string> returned;

    for (it = commands.begin(); it != commands.end(); ++it) {
        // Get the command
        string command = *it;

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
            thread t1(ClientLauncher::uploadFile, filename, size, serverIP, port);
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
                thread t1(ClientLauncher::downloadFile, filename, size, serverIP, port);
                t1.detach();
            }
        } else {
            // Send the command to the server
            client.sendToServer(command);

            // Read and print the result from the server
            string fromServer = client.readFromServer();

            // Remove the "\n" (i.e. shitty fix)
            fromServer = fromServer.substr(0, fromServer.size()-1);

            // Append it the vector
            returned.push_back(std::move(fromServer));
        }
    }

    cout << "Exiting the automated mode" << endl;

    return returned;
}

int main(int argc, char *argv[]) {
    // Checks the number of arguments
    if (argc != 3 && argc != 5) {
        cout << "Number of arguments specified is not correct" << endl;
        return -1;
    }

    // Get the first 2 arguments
    char *ip = argv[1];
    int port = atoi(argv[2]);

    // Checks the port
    if (port <= 0) {
        cout << "Port is not valid" << endl;
        return -1;
    }

    // Casts the port to an unsigned int
    unsigned int serverPort = (unsigned int) port;

    // Checks the IP
    if (!ClientLauncher::isValidIpAddress(ip)) {
        cout << "Server IP provided is not valid" << endl;
        return -1;
    }

    string serverIP(ip);

    // Create the object to launch the client
    ClientLauncher clientLauncher;

    // If the infile and outfile for automation are also specified
    if (argc == 5) {
        // Get the infile and outfile
        string inFile(argv[3]);
        string outFile(argv[4]);

        try {
            // Checks if infile exists
            FileReader fr(BASEPATH + inFile);
            FileWriter fw(BASEPATH + outFile);

            // Clear the output file
            fw.clearFile();

            // We read all the commands
            vector <string> commands;
            fr.readFileVector(commands);

            // Launches the client in automated mode
            vector <string> output = clientLauncher.startClientAutomated(serverIP, serverPort, commands);

            fw.write(output);

        } catch (Exception &e) {
            e.print_error();
            return -1;
        }
    } else {
        // Launches the client in normal mode
        try {
            clientLauncher.startClient(serverIP, serverPort);
        } catch (Exception &e) {
            e.print_error();
        }
    }
    return 0;
}