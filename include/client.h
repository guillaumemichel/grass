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
#include "client_socket.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "exception.h"

using namespace std;

class ClientLauncher {
public:
    static void downloadFile(string filename, int size, int port);

    static void uploadFile(string filename, string size, int port);

    static void fileTransferConnect(Client* client);

    void startClient();
};