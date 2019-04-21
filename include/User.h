/**
 * User.h
 * Defines the notion of user within the system.
 * Author: Xavier Pantet
 */
#ifndef USER
#define USER

#include <string>
using namespace std;

class User {
public:
    User(const string name);

    /**
     * Sets the authentication status of the user.
     * @param status The new authentication status
     */
    void setAuthenticated(bool status);

    /**
     * Gets the authentication status of the user.
     * @return true iff the user has been authenticated
     */
    bool isAuthenticated() const;

    /**
     * Gets name of the user
     * @return User name
     */
    string getName() const;

private:
    bool authenticated;
    const string name;
};
#endif