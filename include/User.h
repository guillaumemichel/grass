class User {
public:
    User();
    ~User();

    bool setAuthenticated(bool);
    bool isAuthenticated() const;

private:
    bool authenticated;
};