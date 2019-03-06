#ifndef ASS_ON_THE_GRASS_CLIENT_H
#define ASS_ON_THE_GRASS_CLIENT_H


class Client {
public:
    /**
     * Read from the keyboard a command and returns it.
     * @return string
     */
    string readCommand();

    void sendToServer(string toSend);

    int initiateConnection();

    int getSock();

    bool isSocketInitiated();

    void readFromServer();

private:
    int sock = 0;

};


#endif //ASS_ON_THE_GRASS_CLIENT_H
