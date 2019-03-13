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
    vector<string> desiredLines(lines.size());
    copy_if(lines.begin(), lines.end(), desiredLines.begin(), [key](string str){ return str.rfind(key, 0) == 0; });
    return desiredLines;
}

string Configuration::extractStringValue(string key) {
    vector<string> filtered = getEntriesWithKey(key);
    if(filtered.size() > 0 && filtered[0].rfind(key, 0) == 0 && filtered[0].find(" ", 0) == key.size() && filtered[0].size() > key.size() + 1) {
        return filtered[0].substr(key.size() + 1);
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

//map<string, string> Configuration::getUsers() { map<string> m; return m; }

