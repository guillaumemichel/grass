#include <grass.h>
#include <ctype.h>
#include <iostream>
#include <string>
#include "../include/commands.h"

using namespace std;


static struct User **userlist;
static int numUsers;
static struct Command **cmdlist;
static int numCmds;
char port[7] = "31337";

// Helper function to run commands in unix.
int run_command(string command, int sock){
  //manage socket
  //manage control access
  int permission_level = 2;
  std:string response = exec_command(command, permission_level);
  cout << response << endl << endl;
  if (!response.compare(0, str_bye.length(), str_bye)){
    return 1;
  }
  return 0;
}


/*
 * Send a file to the client as its own thread
 *
 * fp: file descriptor of file to send
 * sock: socket that has already been created.
 */
void send_file(int fp, int sock) {
}

/*
 * Send a file to the server as its own thread
 *
 * fp: file descriptor of file to save to.
 * sock: socket that has already been created.
 * size: the size (in bytes) of the file to recv
 */
void recv_file(int fp, int sock, int size) {
}

// Server side REPL given a socket file descriptor
void *connection_handler(void* sockfd) {
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

int main() {
    // TODO:
    // Parse the rass.conf file
    // Listen to the port and handle each connection
    cout << "I am the server =D\n\n";
    //connect with client
    //infinite loop: wait for Commands
    string cmd;
    int end = 0;
    while(!end){
        cout << endl << ">>> ";
        getline(cin, cmd);
        cout << endl;
        end = run_command(cmd, 0);
    }
    //run commands and loop again
}
