#include <string>
#include <map>
#include <iostream>

#include "AuthorizationPolicy.h"
#include "User.h"


using namespace std;

class AuthorizationService {

private:
    User* user_;

public:
    static map<string, AuthorizationPolicy> policies;
    AuthorizationService(User* u) { user_ = u; }
    bool hasAccessTo(const string);
};

map<string, AuthorizationPolicy> init_policies(){
    map<string, AuthorizationPolicy> m;
    m["login"] = anonymous;
    m["pass"] = anonymous;
    m["ping"] = anonymous;
    m["ls"] = user;
    m["cd"] = user;
    m["mkdir"] = user;
    m["rm"] = user;
    m["get"] = user;
    m["put"] = user;
    m["grep"] = user;
    m["date"] = user;
    m["whoami"] = user;
    m["w"] = user;
    m["logout"] = user;
    m["exit"] = user;
    return m;
}

map<string, AuthorizationPolicy> AuthorizationService::policies = init_policies();

bool AuthorizationService::hasAccessTo(const string command){
    if(policies.find(command) != policies.end()){
        return policies.at(command) == anonymous || (policies.at(command) == user && user_->isAuthenticated());
    }
    return false;
}

int main(void){
    User* u = new User();
    AuthorizationService* authService = new AuthorizationService(u);
    cout << authService->hasAccessTo("cmd");
    return 0;
}