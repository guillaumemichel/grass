#include "../../include/User.h"

User::User() {
    authenticated = false;
}

User::~User() {}

bool User::setAuthenticated(bool status) {
    authenticated = status;
}

bool User::isAuthenticated() const {
    return authenticated;
}
