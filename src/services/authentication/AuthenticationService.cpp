#include <map>
#include <string>
#include <mutex>
#include "../../../include/AuthenticationService.h"
#include "../../../include/exception.h"

using namespace std;

mutex mtx;

AuthenticationService::AuthenticationService(const Configuration &config): config(config) {
    users = {};
}

void AuthenticationService::registerUser(unsigned int socketID, string name) {
   if(users.find(socketID) != users.end()) { logout(socketID); }
   User u(name);
   u.setAuthenticated(false);
   mtx.lock();
   users.insert({socketID, u});
   mtx.unlock();
}

bool AuthenticationService::login(const unsigned int socketID, const string username, const string passwd) {
    map<string, string> usersDB = config.getUsers();
    if(users.find(socketID) != users.end() && usersDB.find(username) != usersDB.end()) {
        if(usersDB[username] == passwd) {
            mtx.lock();
            users.find(socketID)->second.setAuthenticated(true);
            mtx.unlock();
            return true;
        }
    }
    return false;
}

void AuthenticationService::logout(const int socketID) {
    mtx.lock();
    users.erase(socketID);
    mtx.unlock();
}

User AuthenticationService::getUser(const unsigned int socketID) {
    if(users.find(socketID) != users.end()) {
        return users.find(socketID)->second;
    }
    return User("Anonymous");
}

vector<User> AuthenticationService::getAuthenticatedUsers() {
    vector<User> users_;
    for(auto const& entry: users) {
        if(entry.second.isAuthenticated())
            users_.push_back(entry.second);
    }
    return users_;
}