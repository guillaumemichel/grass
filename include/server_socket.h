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
#include "NetworkSocket.h"

using namespace std;

class Server: public NetworkSocket {
public:
    explicit Server(uint16_t port);

    void initiateConnection();

    struct sockaddr_in address;

    void readFromUserSocket(int userSocket);

    void sendToClient(int socket, string message);
private:
    static void receiveFileUpload(string filename, int size, int port);

    static void sendFile(string filename, int port);

};


#endif //ASS_ON_THE_GRASS_SERVER_H
