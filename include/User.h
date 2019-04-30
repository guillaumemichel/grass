/**
 * User.h
 * Defines the notion of user within the system.
 * @author: Xavier Pantet
 */

#include "commands.h"

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

    /**
     * Set the current path of the user
     * @param  new_path the new path that will replace the old once
     */
    void setPath(string new_path);

    /**
     * Get the current path of the user
     * @return the current path of the user
     */
    string getPath();

private:
    bool authenticated;
    const string name;
    string path;
};
#endif
