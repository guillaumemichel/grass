#include <string>
#include "../../include/User.h"

User::User(const string name): name(name) {
    authenticated = false;
}

bool User::setAuthenticated(bool status) {
    authenticated = status;
}

bool User::isAuthenticated() const {
    return authenticated;
}

string User::getName() const {
    return name;
}