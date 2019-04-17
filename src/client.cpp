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

void download(string filename, int size, int port) {
    cout << "Starting new thread to receive the file from the server" << endl;
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
        throw invalid_argument("The NetworkSocket was not properly created");
    }

    client.downloadFile(filename, size);

    // Close the NetworkSocket
    client.closeConnection();

    cout << "Closing the upload thread" << endl;
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
        throw invalid_argument("The NetworkSocket was not properly created");
    }

    client.uploadFile(filename);

    // Close the NetworkSocket
    client.closeConnection();

    //cout << "Closing the upload thread" << endl;
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
            thread t1(ouba, filename, size, port);
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
                thread t1(download, filename, size, port);
                t1.detach();
            }
        } else {
            // Send the command to the server
            client.sendToServer(command);
        }
    } while (command != Client::EXIT_CMD);

    cout << "Exiting the client" << endl;

    return 0;
}
