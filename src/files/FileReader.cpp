#include <utility>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;

#include "../../include/FileReader.h"

FileReader::FileReader(string filename) {
    // Append the basepath to the filename
    // TODO : check if file is accessible (like not something ../../../info.root)

    // Check if file exists
    std::ifstream ifile(filename.c_str());
    if (!(bool) ifile) {
        // File doesn't exist, we throw an exception
        throw invalid_argument("File does not exist");
    }

    this->filename = std::move(filename);
}

void FileReader::readFileVector(std::vector<std::string> &file) const {
    // Open the file
    std::ifstream in(this->filename.c_str());

    // Check if object is valid
    if (!in) {
        throw invalid_argument("Cannot open the file");
    }

    std::string str;

    // Read the next line from File until it reaches the end.
    while (std::getline(in, str)) {
        file.push_back(str);
    }

    //Close The File
    in.close();
}
/*
void FileReader::readFile(char *buffer) const {
    // Malloc the buffer for the file
    size_t fileSize = this->fileSize();
    buffer = (char *) malloc(fileSize);

    // TODO : FOR MALLOC, ASSERT OR EXCEPTION ?
    if (buffer == nullptr) {
        cout << "Error while reading the file";
        throw logic_error("Cannot allocate the buffer to read the file");
    } else {
        ifstream infile;
        infile.open("test.txt");

        cout << "Reading from the file :" << endl;
        infile.getline(buffer, fileSize);

        // write the data at the screen.
        cout << buffer << endl;
        // close the opened file.
        infile.close();
    }
}*/

string FileReader::fileToString() const {
    string str;

    vector<string> vecOfStr;
    this->readFileVector(vecOfStr);

    vector<string>::iterator it;
    for (it = vecOfStr.begin(); it != vecOfStr.end(); ++it) {
        cout << *it << endl;
        str += *it;
    }

    return str;
}
