#include <map>
#include <string>
#include "../../../include/AuthenticationService.h"
#include "../../../include/exception.h"

using namespace std;

AuthenticationService::AuthenticationService(const Configuration &config): config(config) {
    users = {};
}

bool AuthenticationService::registerUser(unsigned int socketID, string name) {
   if(users.find(socketID) == users.end()) {
        User u(name);
        u.setAuthenticated(false);
        users.insert({socketID, u});
        return true;
   }
   return false;
}

bool AuthenticationService::login(const unsigned int socketID, const string username, const string passwd) {
    map<string, string> usersDB = config.getUsers();
    if(users.find(socketID) != users.end() && usersDB.find(username) != usersDB.end()) {
        if(usersDB[username] == passwd) {
            users.find(socketID)->second.setAuthenticated(true);
            return true;
        }
    }
    return false;
}

void AuthenticationService::logout(const int socketID) {
    users.erase(socketID);
}

User AuthenticationService::getUser(const unsigned int socketID) {
    if(users.find(socketID) != users.end()) {
        return users.find(socketID)->second;
    }
    throw Exception(ERR_INVALID_ARGS);
}

vector<User> AuthenticationService::getAuthenticatedUsers() {
    vector<User> users_;
    for(auto const& entry: users) {
        if(entry.second.isAuthenticated())
            users_.push_back(entry.second);
    }
    return users_;
}