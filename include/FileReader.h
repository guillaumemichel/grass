#include <string>
#include <vector>
#include "File.h

#ifndef ASS_ON_THE_GRASS_FILEREADER_H
#define ASS_ON_THE_GRASS_FILEREADER_H

using namespace std;

class FileReader: public File {
public:
    explicit FileReader(string filename);

    void readFile(char * buffer) const;

    void readFileVector(vector<string> & file) const;

    string fileToString() const;
};


#endif //ASS_ON_THE_GRASS_FILEREADER_H
