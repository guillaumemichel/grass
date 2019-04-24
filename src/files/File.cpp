/**
 * File.cpp
 * Implementation of the File.h file. Super class providing some tools method for FileReader and FileWriter classes.
 *
 * @author Alexandre Chambet
 */

#include "../../include/File.h"

using namespace std;

size_t File::fileSize() const {
    // File size
    streampos fsize = 0;

    // Open the file
    ifstream file(filename, ios::binary);

    // Go to the beginning of the file and to the end of it to compute the distance between those 2
    fsize = file.tellg();
    file.seekg(0, ios::end);

    fsize = file.tellg() - fsize;

    file.close();

    // Finally returns the size of the file
    return (size_t) fsize;
}
