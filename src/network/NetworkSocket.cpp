#include "../../include/NetworkSocket.h"

using namespace std;

NetworkSocket::NetworkSocket(unsigned int port) {
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

void NetworkSocket::sendTo(int socket, string msg) {
    if (-1 == send(socket, msg.data(), msg.size(), 0)) {
        throw Exception(ERR_NETWORK_WRITE_SOCKET);
    }
}

void NetworkSocket::commonInitiateConnection() {
    // Create the socket
    if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw Exception(ERR_NETWORK_CREATE_SOCKET);
    }

    // Configuration of the socket
    (this->address).sin_family = AF_INET;
    (this->address).sin_port = htons(this->port);
}
