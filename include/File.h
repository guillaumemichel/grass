#include <string>
#include <fstream>
#include <vector>

using namespace std;

#ifndef ASS_ON_THE_GRASS_FILE_H
#define ASS_ON_THE_GRASS_FILE_H


class File {
protected:
    string filename;

    size_t fileSize() const;
};


#endif //ASS_ON_THE_GRASS_FILE_H
