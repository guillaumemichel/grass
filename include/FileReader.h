#include <string>
#include <vector>

#ifndef ASS_ON_THE_GRASS_FILEREADER_H
#define ASS_ON_THE_GRASS_FILEREADER_H

using namespace std;

class FileReader {
public:
    explicit FileReader(string filename);

    void readFile(char * buffer) const;

    void readFileVector(vector<string> & file) const;

    string fileToString() const;

    size_t fileSize() const;

private:
    string filename;


};


#endif //ASS_ON_THE_GRASS_FILEREADER_H
