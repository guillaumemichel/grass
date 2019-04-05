#include "../../include/NetworkSocket.h"

using namespace std;

NetworkSocket::NetworkSocket(uint16_t port) {
    this->port = port;
}

void NetworkSocket::closeConnection() {
    close(this->sock);
}

bool NetworkSocket::isSocketInitiated() {
    return this->sock > 0;
}

int NetworkSocket::getSocket() {
    return this->sock;
}

string NetworkSocket::readFrom(int socket) {
    return NULL;
}

void NetworkSocket::sendTo(int socket, string msg) {
    if (-1 == send(socket, msg.data(), msg.size(), 0)) {
        throw invalid_argument("Error : cannot send the data to the server...");
    }
}


