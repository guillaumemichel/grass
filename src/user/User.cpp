#include <string>
#include "../../include/User.h"

using namespace std;

User::User(const string name, unsigned int socketID): name(name) {
    authenticated = false;
    path = "/" + to_string(socketID);
    files_path=path;
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
    this->path = new_path;
    cout << this->path << endl;
}

string User::getPath(){
    cout << this->path << endl;
    return this->path;
}

string User::getFilesPath() {
    return files_path;
}
