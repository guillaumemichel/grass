#ifndef ASS_ON_THE_GRASS_SERVER_H
#define ASS_ON_THE_GRASS_SERVER_H

#include "Server.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

using namespace std;

class Server {
public:
    int initiateConnection();

    int getSocket();

    bool isSocketInitiated();

    /**
     * Accepts a new socket and returns it.
     * Returns -1 if an error occured.
     *
     * @return int
     */
    int acceptSocket();

    struct sockaddr_in address;

    int readFromUserSocket(int userSocket);
private:
    int sock = 0;

    const uint16_t PORT = 8080;
};


#endif //ASS_ON_THE_GRASS_SERVER_H
