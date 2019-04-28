/**
 * Server.cpp
 * Creates a server and makes it listen to the to passed port.
 *
 * @author Alexandre Chambet
 */

#include <ctype.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include "../include/grass.h"
#include "../include/ServerSocket.h"
#include "../include/Configuration.h"
#include "../include/commands.h"

using namespace std;

/**
 * Callback method when creating a thread to handle the new clients.
 *
 * @param userSocket the socket of the new client
 * @param server the instance of the server
 */
void connectClient(int userSocket, ServerSocket server, Commands &commands) {
    Configuration conf = Configuration("grass.conf");

    // Client's dir
    string dir = conf.getBase() + to_string(userSocket) + "/";

    // Creates a new directory for the client
    char command0[] = "/bin/mkdir";
    char *arg0 = &dir[0u];
    char * const argv0[] = {command0, arg0, NULL};
    char * const envp[] = {NULL};
    Commands::call_cmd(command0,argv0,envp);


    // This function exists when the "exit" command is received
    server.readFromUserSocket(userSocket, commands);

    cout << "Disconnecting client #" << userSocket << endl;

    // Remove the directory of the client
    dir = conf.getBase() + to_string(userSocket);
    char command1[] = "/bin/rm";
    char arg1[] = "-rf";
    char *arg2 = &dir[0u];
    char * const argv1[] = {command1, arg1, arg2, NULL};
    Commands::call_cmd(command1,argv1,envp);

}

int main() {
    try {
        // Parses the configuration file
        string path = Commands::cmd_pwd();
        //cout << path.substr(0,path.size()-1)<<"/grass.conf" << endl;
        //Configuration conf = Configuration(path.substr(0,path.size()-1)+"/grass.conf");
        Configuration conf = Configuration("grass.conf");
        Commands commands = Commands(conf);

        // Create a server object
        ServerSocket server(conf.getPort());

        // Create the server socket
        server.initiateConnection();

        cout << "Server started on port " << conf.getPort() << endl;

        while (true) {
            cout << "Listening for incoming connections..." << endl;
            int userSocket = server.allocateSocketClient();
            cout << "New client connected : " << userSocket << endl;

            // Start a new thread to handle the new client
            thread t(connectClient, userSocket, server, ref(commands));
            // Use detach() so that the server can receive and handle clients in parallel
            t.detach();
        }
    } catch (Exception &e) {
        cout << "Fatal error of the server : " << e.print_error() << endl;
        cout << "Closing the server..." << endl;
    }

    return 0;
}
