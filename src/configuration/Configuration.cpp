#include <vector>
#include <string>
#include <algorithm>
#include "../../include/Configuration.h"
#include "../../include/FileReader.h"

using namespace std;

Configuration::Configuration(const FileReader& fileReader): fileReader(fileReader) {}

vector<string> Configuration::getEntriesWithKey(const string key) {
    vector<string> lines;
    fileReader.readFileVector(lines);
    vector<string> desiredLines;
    for(auto const& line: lines) {
        if(line.rfind(key, 0) == 0 && line.size() > key.size() + 1 && line.find_first_of(" ") == key.size())
            desiredLines.push_back(line);
    }
    return desiredLines;
}

string Configuration::removeKeyInLine(string key, string line) {
    if(line.find(" ", 0) == key.size() && line.size() > key.size() + 1) {
        return line.substr(key.size() + 1);
    }
    return "NOT FOUND";
}

string Configuration::extractStringValue(string key) {
    vector<string> filtered = getEntriesWithKey(key);
    if(filtered.size() > 0) {
        return removeKeyInLine(key, filtered[0]);
    }
    return "NOT FOUND";
}

string Configuration::getBase() {
    return extractStringValue("base");
}

unsigned int Configuration::getPort() {
    string value = extractStringValue("port");
    if(value == "NOT FOUND")
        return -1;
    return stoul(value, NULL, 10);
}

map<string, string> Configuration::getUsers() {
    vector<string> filtered = getEntriesWithKey("user");
    map<string, string> users;
    for(auto const& u: filtered) {
        string userString = removeKeyInLine("user", u);
        size_t delimiterPos = userString.find(" ", 0);
        if(delimiterPos != string::npos && delimiterPos < userString.size() - 1) {
            users[userString.substr(0, delimiterPos)] = userString.substr(delimiterPos + 1);
        }
    }
    return users;
}

