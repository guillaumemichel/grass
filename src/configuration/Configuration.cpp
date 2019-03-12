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

string Configuration::getBase() {
    vector<string> filtered = getEntriesWithKey("base");
    if(filtered.size() > 0 && filtered[0].rfind("base", 0) == 0 && filtered[0].find(" ", 0) == 4 && filtered[0].size() > 5)
        return filtered[0].substr(5);
    return "NOT FOUND";
}

unsigned int Configuration::getPort() {
    vector<string> filtered = getEntriesWithKey("port");
    if(filtered.size() > 0 && filtered[0].rfind("port", 0) == 0 && filtered[0].find(" ", 0) == 4 && filtered[0].size() > 5)
        return stoul(filtered[0].substr(5), NULL, 10);
}

//map<string, string> Configuration::getUsers() { map<string> m; return m; }

