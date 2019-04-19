#ifndef ASS_ON_THE_GRASS_SERVER_H
#define ASS_ON_THE_GRASS_SERVER_H

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <thread>
#include "FileWriter.h"
#include "FileReader.h"
#include "NetworkSocket.h"
#include "exception.h"

using namespace std;

class Server: public NetworkSocket {
public:
    explicit Server(unsigned int);

    void initiateConnection();

    void readFromUserSocket(int userSocket);

    void sendToClient(int socket, string message);

    int allocateSocketClient();
private:
    static void receiveFileUpload(string filename, int size, int port);

    static void sendFile(string filename, int port);

    int getRandomPort();

    const int MAX_WRONG_READ = 10;
};


#endif //ASS_ON_THE_GRASS_SERVER_H
