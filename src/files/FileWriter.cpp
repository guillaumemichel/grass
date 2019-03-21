#include "../../include/FileWriter.h"

FileWriter::FileWriter(string filename) {
    // TODO : what to do if the file already exists ? Destroy it and create a new one?
    this->filename = std::move(filename);
}

void FileWriter::write(std::vector <std::string> data) {
    // Basically write each line consecutively to the file (could find a better way to do it such as including \n and then write once for all)
    vector<string>::iterator it;

    for (it = data.begin(); it != data.end(); ++it) {
        this->writeLine(*it);
    }
}

void FileWriter::writeLine(string line) {
    // Create a file that is writable
    // Using the ios::app to make data appendable
    ofstream file(this->filename, ios::app);

    // Check the file was properly opened
    if (!file) {
        throw invalid_argument("Cannot open the file for writing");
    }

    // Add the return otherwise it just overwrites the file
    line += "\n";

    // Write it to the file
    file << line;

    file.close();
}

void FileWriter::clearFile() {
    // Clear the file but does not remove it
    ofstream file(this->filename);

    // Check the file was properly opened
    if (!file) {
        throw invalid_argument("Cannot open the file for writing");
    }

    file << "";

    file.close();
}


