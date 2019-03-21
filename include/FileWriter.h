#include <string>
#include <vector>
#include "File.h"

#ifndef ASS_ON_THE_GRASS_FILEWRITER_H
#define ASS_ON_THE_GRASS_FILEWRITER_H

using namespace std;

class FileWriter: public File {
public:
    explicit FileWriter(string filename);

    void write(std::vector<std::string> data);

    void writeLine(string line);

    void clearFile();
};


#endif //ASS_ON_THE_GRASS_FILEWRITER_H
