#include <string>
#include <fstream>
#include <vector>

using namespace std;

#ifndef ASS_ON_THE_GRASS_FILE_H
#define ASS_ON_THE_GRASS_FILE_H


// TODO : replace by constant
#define BASEPATH "/home/alex/Documents/EPFL/SoftSec/Project/ass_on_the_grass/"
#define UPLOAD_BASEPATH "/home/alex/Documents/EPFL/SoftSec/Project/ass_on_the_grass/upload/"
#define DOWNLOAD_BASEPATH "/home/alex/Documents/EPFL/SoftSec/Project/ass_on_the_grass/download/"

class File {
public:
    size_t fileSize() const;

protected:
    string filename;
};


#endif //ASS_ON_THE_GRASS_FILE_H
