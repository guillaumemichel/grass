/**
 * Client.h
 * Header file for Client.cpp.
 *
 * @author Alexandre Chambet
 */

#include <stdio.h>
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
    static void downloadFile(string filename, unsigned int size, unsigned int port);

    /**
     * Uploads a file to the server.
     *
     * @param filename the file to be uploaded
     * @param size the size of the file
     * @param port the port where the file should be send
     */
    static void uploadFile(string filename, string size, unsigned int port);

    /**
     * Utils method to manage common stuff from upload and download such as connecting to the server, creating a new socket on the right port and much more.
     *
     * @param client the client socket object which will be used for the transfer
     */
    static void fileTransferConnect(ClientSocket *client);

    /**
     * Starts a new client and connects to the server on the given port.
     * Typically, this will be called in the main function to start the client and run it.
     *
     * @param port the server's port to connect to
     */
    void startClient(unsigned int port);
};
