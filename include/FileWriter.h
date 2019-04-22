/**
 * FileWriter.h
 * Header file for FileWriter.cpp
 *
 * @author Alexandre Chambet
 */

#include <string>
#include <vector>
#include "File.h"
#include "exception.h"

#ifndef ASS_ON_THE_GRASS_FILEWRITER_H
#define ASS_ON_THE_GRASS_FILEWRITER_H

using namespace std;

class FileWriter: public File {
public:
    /**
     * Constructor for the class.
     *
     * @param filename of the file to be written
     */
    explicit FileWriter(string filename);

    /**
     * Writes into the file the content of the vector.
     *
     * @param data to be written
     */
    void write(std::vector<std::string> data);

    /**
     * Appends a line to the file.
     *
     * @param line to be written
     */
    void writeLine(string line);

    /**
     * Empties the file.
     */
    void clearFile();
};


#endif //ASS_ON_THE_GRASS_FILEWRITER_H
