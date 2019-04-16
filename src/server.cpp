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

// Helper function to run commands in unix.
int run_command(string command, int sock) {
    //manage NetworkSocket
    //manage control access
    int permission_level = 2;
    int i = exec_command(command, permission_level);
    return i;
}


/*
 * Send a file to the client as its own thread
 *
 * fp: file descriptor of file to send
 * sock: NetworkSocket that has already been created.
 */
void send_file(int fp, int sock) {
}

/*
 * Send a file to the server as its own thread
 *
 * fp: file descriptor of file to save to.
 * sock: NetworkSocket that has already been created.
 * size: the size (in bytes) of the file to recv
 */
void recv_file(int fp, int sock, int size) {
}

// Server side REPL given a NetworkSocket file descriptor
void *connection_handler(void *sockfd) {
}

/*
 * search all files in the current directory
 * and its subdirectory for the pattern
 *
 * pattern: an extended regular expressions.
 * Output: A line seperated list of matching files' addresses
 */
void search(char *pattern) {
    // TODO
}

// Parse the grass.conf file and fill in the global variables
void parse_grass() {
}

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
