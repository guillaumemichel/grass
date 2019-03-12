#include <string>
#include <map>

#include "AuthorizationPolicy.h"
#include "User.h"

using namespace std;

map<string, AuthorizationPolicy> init_policies();

class AuthorizationService {
    private:
        User* user;

    public:
        // Constructor and destructor
        AuthorizationService(User* user);
        ~AuthorizationService();

        // Static attributes
        static map<string, AuthorizationPolicy> policies;

        // Class methods
        bool hasAccessTo(const string);
};
