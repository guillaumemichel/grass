/**
 * File.h
 * Header file for File.cpp.
 *
 * @author Alexandre Chambet
 */

#include <string>
#include <fstream>
#include <vector>

using namespace std;

#ifndef ASS_ON_THE_GRASS_FILE_H
#define ASS_ON_THE_GRASS_FILE_H


// TODO : replace by constant
#define DOWNLOAD_BASEPATH "./download/"

class File {
public:
    /**
     * Computes and returns the size of the file attached to the class.
     *
     * @return the size of the file.
     */
    size_t fileSize() const;
protected:
    /**
     * String storing the filename of the file.
     */
    string filename;
};


#endif //ASS_ON_THE_GRASS_FILE_H
