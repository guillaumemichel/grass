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
    vector<string> getEntriesWithKey(const string key) const;
    string removeKeyInLine(string, string) const;
    string extractStringValue(string) const;

public:
    Configuration(const FileReader&);
    string getBase() const;
    unsigned int getPort() const;
    map<string, string> getUsers() const;
};

#endif //ASS_ON_THE_GRASS_CONFIGURATION_H
