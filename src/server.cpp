#include <ctype.h>
#include <iostream>
#include <string>
#include "../include/grass.h"
#include "../include/commands.h"
#include "../include/server_socket.h"

using namespace std;


static struct User **userlist;
static int numUsers;
static struct Command **cmdlist;
static int numCmds;
char port[7] = "31337";


void connectClient(int userSocket, Server server) {
    // This function exists when the "exit" command is received
    server.readFromUserSocket(userSocket);

    cout << "Disconnecting client #" << userSocket << endl;
}

int main() {
    // TODO:
    // Parse the grass.conf file
    // Listen to the port and handle each connection

    // Create a server object
    Server server(8080);

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
