#include <map>
#include "Configuration.h"

#ifndef USER
#include "User.h"
#endif

#ifndef ASS_ON_THE_GRASS_AUTHENTICATIONSERVICE_H
#define ASS_ON_THE_GRASS_AUTHENTICATIONSERVICE_H

class AuthenticationService {
private:
    const Configuration& config;
    map<unsigned int, User> users;

public:
    AuthenticationService(const Configuration&);
    bool registerUser(const unsigned int socketID, const string name);
    bool login(const unsigned int socketID, const string, const string);
    void logout(const int socketID);
    User getUser(const unsigned int);
    vector<User> getAuthenticatedUsers();
};

#endif //ASS_ON_THE_GRASS_AUTHENTICATIONSERVICE_H
