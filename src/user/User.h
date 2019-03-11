class User {
public:
    User();
    ~User();

    bool authenticated;
    bool setAuthenticated(bool);
    bool isAuthenticated();
};