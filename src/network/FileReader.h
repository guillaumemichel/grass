#ifndef ASS_ON_THE_GRASS_FILEREADER_H
#define ASS_ON_THE_GRASS_FILEREADER_H


class FileReader {
public:
    explicit FileReader(string filename);

    void readFile(char * buffer);

    void readFileVector(vector<string> & file);

    size_t fileSize();

private:
    string filename;


};


#endif //ASS_ON_THE_GRASS_FILEREADER_H
