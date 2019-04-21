#ifndef USER
#define USER

#include <string>
using namespace std;

class User {
public:
    User(const string name);
    void setAuthenticated(bool);
    bool isAuthenticated() const;
    string getName() const;

private:
    bool authenticated;
    const string name;
};
#endif