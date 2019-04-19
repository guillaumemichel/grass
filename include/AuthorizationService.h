#include <string>
#include <map>

#include "AuthorizationPolicy.h"
#ifndef USER
#include "User.h"
#endif

using namespace std;

map<string, AuthorizationPolicy> init_policies();

class AuthorizationService {
    private:
        const User& user;

    public:
        // Constructor and destructor
        AuthorizationService(const User&);

        // Static attributes
        static map<string, AuthorizationPolicy> policies;

        // Class methods
        bool hasAccessTo(const string);
};
