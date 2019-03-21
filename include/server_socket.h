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

using namespace std;

class Server {
public:
    explicit Server(uint16_t port);

    int initiateConnection();

    int getSocket();

    bool isSocketInitiated();

    struct sockaddr_in address;

    void readFromUserSocket(int userSocket);
private:
    int sock = 0;

    uint16_t port;

    static void receiveFileUpload(string filename);
};


#endif //ASS_ON_THE_GRASS_SERVER_H
