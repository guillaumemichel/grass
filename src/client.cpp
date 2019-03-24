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

#include "../include/grass.h"
#include "../include/client_socket.h"
#include "../include/FileReader.h"
#include "../include/FileWriter.h"

using namespace std;


/*
 * Send a file to the server as its own thread
 *
 * fp: file descriptor of file to send
 * d_port: destination port
 */
void send_file(int fp, int d_port) {
    // TODO
}

/*
 * Recv a file from the server as its own thread
 *
 * fp: file descriptor of file to save to.
 * d_port: destination port
 * size: the size (in bytes) of the file to recv
 */
void recv_file(int fp, int d_port, int size) {
    // TODO
}


/*
 * search all files in the current directory
 * and its subdirectory for the pattern
 *
 * pattern: an extended regular expressions.
 */
void search(char *pattern) {
    // TODO
}

void ouba(string filename, string size, int port) {
    cout << "Starting new thread to send the file to the server" << endl;
    Client client(port);

    // Loop while the server is ready
    int maxTries = 10;
    bool connected = false;
    while (maxTries > 0 && !connected) {
        // Tries to connect
        try {
            client.initiateConnection();
            connected = true;
        } catch (exception &e) {
            cout << "Cannot connect to the server... Attempting again..." << endl;
            maxTries--;
            // Wait an exponential amount of time until the server is ready
            usleep(100 * (10 - maxTries));
        }
    }


    if (maxTries == 0) {
        throw invalid_argument("Cannot connect to the server");
    }

    // Should be ok, but we just check if the sock was properly created
    if (!client.isSocketInitiated()) {
        throw invalid_argument("The socket was not properly created");
    }

    client.uploadFile(filename);

    // Close the socket
    client.closeConnection();

    cout << "Closing the upload thread" << endl;
}

int main(int argc, char **argv) {
    // TODO:
    // Make a short REPL to send commands to the server
    // Make sure to also handle the special cases of a get and put command
    // Instantiate a new client
    Client client(8080);

    client.initiateConnection();

    string command;

    // Loop while command is not exit
    do {
        // Read the command
        command = client.readCommand();

        // TODO : create constant for command names
        // If a file must be uploaded
        if (command.substr(0, 3) == "put") {
            string removePut = command.substr(command.find(" ") + 1);
            string filename = removePut.substr(0, removePut.find(" "));
            string size = removePut.substr(removePut.find(" ") + 1); // TODO : what to do with the size?

            // Send the command to the server
            client.sendToServer(command);

            // Wait for the server answer
            string read = client.readFromServer();

            // Parse the port number
            cout << read << endl;
            int port = stoi(read.substr(read.find(":") + 2));

            // Upload the file
            thread t1(ouba, filename, size, port);
            t1.join();
        } else {
            // Send the command to the server
            client.sendToServer(command);
        }
    } while (command != Client::EXIT_CMD);

    cout << "Exiting the client" << endl;

    return 0;
}
