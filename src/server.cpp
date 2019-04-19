#include <ctype.h>
#include <iostream>
#include <string>
#include "../include/grass.h"
#include "../include/server_socket.h"
#include "../include/Configuration.h"

using namespace std;

void connectClient(int userSocket, Server server) {
    // This function exists when the "exit" command is received
    server.readFromUserSocket(userSocket);

    cout << "Disconnecting client #" << userSocket << endl;
}

int main() {
    Configuration conf = Configuration(FileReader("grass.conf"));

    // Create a server object
    Server server(conf.getPort());

    // Create the server socket
    server.initiateConnection();

    cout << "Server NetworkSocket initiated" << endl;

    while (true) {
        cout << "Listening for incoming connections..." << endl;
        int userSocket = server.allocateSocketClient();
        cout << "New client connected : " << userSocket << endl;

        thread t(connectClient, userSocket, server);
        t.detach();

        // TODO : add try catch around to print error message in case of
    }

    return 0;
}
