#include <utility>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;

#include "FileReader.h"

FileReader::FileReader(string filename) {
    // Check if file exists
    std::ifstream ifile(filename.c_str());
    if (!(bool) ifile) {
        // File doesn't exist, we throw an exception
        throw invalid_argument("File does not exist");
    }

    this->filename = std::move(filename);
}

void FileReader::readFileVector(std::vector<std::string> &file) {
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

void FileReader::readFile(char *buffer) {
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
}

size_t FileReader::fileSize() {
    // File size
    streampos fsize = 0;
    ifstream file(filename, ios::binary);

    fsize = file.tellg();
    file.seekg(0, ios::end);
    fsize = file.tellg() - fsize;
    file.close();

    return (size_t) fsize;
}

string FileReader::fileToString() {
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
