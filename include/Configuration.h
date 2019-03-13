#include <map>
#include <string>
#include <vector>
#include "FileReader.h"

#ifndef ASS_ON_THE_GRASS_CONFIGURATION_H
#define ASS_ON_THE_GRASS_CONFIGURATION_H

using namespace std;

class Configuration {
private:
    const FileReader& fileReader;
    vector<string> getEntriesWithKey(const string key);
    string extractStringValue(string);

public:
    Configuration(const FileReader&);
    string getBase();
    unsigned int getPort();
    //map<string, string> getUsers();
};

#endif //ASS_ON_THE_GRASS_CONFIGURATION_H
