#ifndef ASS_ON_THE_GRASS_CLIENT_H
#define ASS_ON_THE_GRASS_CLIENT_H

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include "FileReader.h"
#include "FileWriter.h"
#include "NetworkSocket.h"
#include "exception.h"

using namespace std;

class Client: public NetworkSocket {
public:
    explicit Client(uint16_t dstPort);

    /**
     * Read from the keyboard a command and returns it.
     *
     * @return string
     */
    string readCommand();

    void sendToServer(string toSend);

    void initiateConnection();

    string readFromServer();

    inline static const string EXIT_CMD = "exit";

    void uploadFile(string filename);

    void downloadFile(string filename, int size);
};


#endif //ASS_ON_THE_GRASS_CLIENT_H
