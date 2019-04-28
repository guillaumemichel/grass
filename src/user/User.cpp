#include <string>
#include "../../include/User.h"

User::User(const string name, unsigned int socketID): name(name) {
    authenticated = false;
    path = "/" + to_string(socketID);
}

void User::setAuthenticated(bool status) {
    authenticated = status;
}

bool User::isAuthenticated() const {
    return authenticated;
}

string User::getName() const {
    return name;
}

void User::setPath(string new_path) {
    path = new_path;
}

string User::getPath(){
    return path;
}
