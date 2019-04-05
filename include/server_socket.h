#ifndef ASS_ON_THE_GRASS_SERVER_H
#define ASS_ON_THE_GRASS_SERVER_H

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <thread>
#include "commands.h"
#include "FileWriter.h"
#include "FileReader.h"

using namespace std;

class Server {
public:
    explicit Server(uint16_t port);

    int initiateConnection();

    int getSocket();

    bool isSocketInitiated();

    struct sockaddr_in address;

    void readFromUserSocket(int userSocket);

    void closeConnection();

    void sendToClient(int socket, string message);
private:
    int sock = 0;

    uint16_t port;

    const int SOCKET_BUFFER_SIZE = 128;

    static void receiveFileUpload(string filename, int size, int port);

    static void sendFile(string filename, int port);

};


#endif //ASS_ON_THE_GRASS_SERVER_H
