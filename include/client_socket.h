#ifndef ASS_ON_THE_GRASS_CLIENT_H
#define ASS_ON_THE_GRASS_CLIENT_H

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <thread>
#include "commands.h"

using namespace std;

class Client {
public:
    explicit Client(uint16_t dstPort);
    /**
     * Read from the keyboard a command and returns it.
     *
     * @return string
     */
    string readCommand();

    void sendToServer(string toSend);

    void sendToServerWithoutSize(string toSend);

    void initiateConnection();

    int getSocket();

    bool isSocketInitiated();

    void readFromServer();

    inline static const string EXIT_CMD = "exit";

    void uploadFile();

private:
    int sock = 0;

    uint16_t port;
};


#endif //ASS_ON_THE_GRASS_CLIENT_H
