/**
 * Client.h
 * Header file for Client.cpp.
 *
 * @author Alexandre Chambet
 */

#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include "grass.h"
#include "ClientSocket.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "exception.h"
#include "Configuration.h"
#include "commands.h"

using namespace std;

class ClientLauncher {
public:
    /**
     * Downloads a file from the server.
     *
     * @param filename the file to be downloaded
     * @param size the size of the file
     * @param port the port where the file will be sent by the server
     */
    static void downloadFile(string filename, unsigned int size, string serverIP, unsigned int port);

    /**
     * Uploads a file to the server.
     *
     * @param filename the file to be uploaded
     * @param size the size of the file
     * @param port the port where the file should be send
     */
    static void uploadFile(string filename, string size, string serverIP, unsigned int port);

    /**
     * Utils method to manage common stuff from upload and download such as connecting to the server, creating a new socket on the right port and much more.
     *
     * @param client the client socket object which will be used for the transfer
     */
    static void fileTransferConnect(ClientSocket *client);

    /**
     * Utils method that determines whether or not an string IP address is correct.
     *
     * @param ipAddress the string to be checked
     * @return true if the IP is correct, false otherwise
     */
    static bool isValidIpAddress(char *ipAddress) {
        struct sockaddr_in sa;

        // Uses the inet_pton function to check if the IP is correct
        int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
        return result != 0;
    }

    /**
     * Starts a new client and connects to the server on the given port.
     * Typically, this will be called in the main function to start the client and run it.
     *
     * @param serverIP the server's IP to connect to
     * @param port the server's port to connect to
     */
    void startClient(string serverIP, unsigned int port);
};
