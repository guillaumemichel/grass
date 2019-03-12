class User {
public:
    User();
    ~User();

    bool setAuthenticated(bool);
    bool isAuthenticated();

private:
    bool authenticated;
};