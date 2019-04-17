#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <arpa/inet.h>

#ifndef ASS_ON_THE_GRASS_SOCKET_H
#define ASS_ON_THE_GRASS_SOCKET_H

using namespace std;

class NetworkSocket {
public:
    // TODO : how to initialize it?
    struct sockaddr_in address;

    explicit NetworkSocket(uint16_t port);

    void closeConnection();

    bool isSocketInitiated();

    int getSocket();

protected:
    string readFrom(int socket);

    void commonInitiateConnection();

    void sendTo(int socket, string msg);

    int sock = 0;

    uint16_t port;

    const int SOCKET_BUFFER_SIZE = 128;
};


#endif //ASS_ON_THE_GRASS_SOCKET_H
